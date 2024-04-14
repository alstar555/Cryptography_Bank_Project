#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "crypto/SHA-1.h"
#include "crypto/RSAPublicKey.h"
#include <fstream>
#include "crypto/RSA.h"
#include "proto/messages.pb.h"
#include <boost/multiprecision/cpp_int.hpp>
#include "crypto/DH.h"
#include "crypto/DESKey.h"
#include "Utils.h"
#include "crypto/DES.h"
#include "crypto/HMAC.h"

// ATM Client Instance
class ATM {
private:

    int port = 1501;
    std::string ip = "127.0.0.1";
    int client;

    RSAPublicKey public_key;
    DESKey des_key;
    std::string hmac_key;
    HMAC hmac;
    unsigned int seq_num;


    std::string encrypt_msg(const std::string& msg) {
        seq_num++; // must increase
        UnencryptedMessage um;
        um.set_inner_msg(msg);
        um.set_seq_num(seq_num);

        auto um_str = um.SerializeAsString();
        std::vector<uint8_t> bytes(um_str.begin(), um_str.end());

        auto iv = random_iv();
        auto enc_longs = DES::encrypt3_cbc(
                longs_from_int(int_from_bytes(bytes)),
                des_key,
                iv
                );
        auto enc_bytes = bytes_from_int(int_from_longs(enc_longs));

        EncryptedMessage* em = new EncryptedMessage;

        em->set_iv(iv);
        em->set_inner_msg(enc_bytes.data(), enc_bytes.size());

        MACMessage mm;

        mm.set_allocated_inner_msg(em);

        mm.set_mac(hmac(hmac_key, mm.inner_msg().SerializeAsString()));

        return mm.SerializeAsString();
    }

    std::pair<bool, std::string> decrypt_msg(void* msg, int len) {
        MACMessage mm;
        if (!mm.ParseFromArray(msg, len)) {
            return std::make_pair(false, "");
        }

        if (hmac(hmac_key, mm.inner_msg().SerializeAsString()) != mm.mac()) {
            return std::make_pair(false, "");
        }

        EncryptedMessage em = mm.inner_msg();

        std::vector<uint8_t> enc_bytes(em.inner_msg().begin(), em.inner_msg().end());

        auto dec_longs = DES::decrypt3_cbc(
                longs_from_int(int_from_bytes(enc_bytes)),
                des_key,
                em.iv()
                );

        auto dec_bytes = bytes_from_int(int_from_longs(dec_longs));

        UnencryptedMessage um;

        if (!um.ParseFromArray(dec_bytes.data(), dec_bytes.size())) {
            return std::make_pair(false, "");
        }

        if (um.seq_num() < seq_num) {
            // repeat of previous message is not allowed!
            return std::make_pair(false, "");
        }
        seq_num = um.seq_num();

        return std::make_pair(true, um.inner_msg());
    }

    std::string hash_msg(const std::string& msg) {
        SHA1 sha1;
        return sha1(msg);
    }

    void run_client() {
        // Create socket
        client = socket(AF_INET, SOCK_STREAM, 0);
        if (client < 0) {
            std::cerr << "Error creating socket.\n";
            exit(1);
        }

        // Address setup
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);
        server_address.sin_addr.s_addr = INADDR_ANY;

        // Connect to server_fd
        if (connect(client, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
            std::cerr << "Error connecting to server_fd.\n";
            exit(1);
        }

        std::cout << "Connected to server_fd.\n";

        // Start "TLS" negotiation

        DH diffie_hellman;

        // receive DH value from server

        char buffer[8192] = {0};
        int len = recv(client, buffer, sizeof(buffer), 0);

        ServerHello sh;

        if (!sh.ParseFromArray(buffer, len)) {
            std::cerr << "Failed to parse ServerHello from Bank. Terminating..." << std::endl;
            return;
        }

        std::vector<uint8_t> sig_bytes(sh.signature().begin(), sh.signature().end());

        boost::multiprecision::cpp_int signature;
        boost::multiprecision::import_bits(signature, sig_bytes.begin(), sig_bytes.end());

        boost::multiprecision::cpp_int val;
        boost::multiprecision::import_bits(val, sh.inner_msg().begin(), sh.inner_msg().end());

        RSA rsa;

        // verify msg is signed by RSA
        if (val != rsa.decrypt(signature, public_key.getE(), public_key.getN())) {
            std::cerr << "Wrong signature on ServerHello message from Bank. Terminating..." << std::endl;
            return;
        }

        ServerHelloContent shc;
        shc.ParseFromString(sh.inner_msg());

        std::vector<uint8_t> dh_val_bytes(shc.diffie_hellman_value().begin(), shc.diffie_hellman_value().end());

        boost::multiprecision::cpp_int imported_dh_value;
        boost::multiprecision::import_bits(imported_dh_value, dh_val_bytes.begin(), dh_val_bytes.end());

        diffie_hellman.build_shared_secret(imported_dh_value);

        std::cout << "INFO: Received successful ServerHello from Bank" << std::endl;

        // send "our" DH value to server
        std::vector<uint8_t> dh_send_bytes;
        boost::multiprecision::export_bits(diffie_hellman.get_send_value(), std::back_inserter(dh_send_bytes), 8);

        ClientHello ch;
        ch.set_diffie_hellman_value(dh_send_bytes.data(), dh_send_bytes.size());

        std::string msg = ch.SerializeAsString();

        send(client, msg.data(), msg.size(), 0);

        std::cout << "INFO: Sent ClientHello to Bank" << std::endl;

        auto shared_secret = diffie_hellman.get_shared_secret();
        boost::multiprecision::cpp_int one = 1;
        boost::multiprecision::cpp_int tmp = shared_secret & ((one << 64) - 1);
        unsigned long key1 = tmp.convert_to<unsigned long>();
        tmp = shared_secret >> 64;
        tmp = tmp & ((one << 64) - 1);
        unsigned long key2 = tmp.convert_to<unsigned long>();
        tmp = shared_secret >> 128;
        tmp = tmp & ((one << 64) - 1);
        unsigned long key3 = tmp.convert_to<unsigned long>();
        des_key = DESKey(key1, key2, key3);

//        std::cout << diffie_hellman.get_shared_secret() << std::endl;

        std::fill_n(buffer, 8192, 0);
        len = recv(client, buffer, sizeof(buffer), 0);

        HMACSend hs;
        if (!hs.ParseFromArray(buffer, len)) {
            std::cerr << "Failed to parse HMACSend message. Terminating..." << std::endl;
            return;
        }

        std::vector<uint8_t> enc_bytes(hs.encrypted_hmac().begin(), hs.encrypted_hmac().end());

        auto decrypted_bytes = bytes_from_int(
                int_from_longs(
                            DES::decrypt3_cbc(
                                    longs_from_int(int_from_bytes(enc_bytes)),
                                    des_key,
                                    hs.iv()
                                    )
                        )
                );

        hmac_key = std::string((char*)decrypted_bytes.data(), decrypted_bytes.size());

//        while (true) {
//            std::string cmd;
//            std::getline(std::cin, cmd);
//
//            // send command to bank
//            sendBankMsg(cmd);
//        }


        // receive HMAC key
        // verify msg is signed by RSA

//        // Receive message from server_fd
//        recvBankMsg();
//
//        // Send message to server_fd
//        sendBankMsg("Message from ATM Client.\n");

    }

    int login(const std::string& bank_card, const std::string& pass) {
        std::string message = "LOGIN " + bank_card + " " + pass;
        sendBankMsg(message.c_str());
        std::string response = recvBankMsg();
        if (response == "APPROVED") {
            return 1;
        }else{
            return 0;
        }
    }

    int deposit(const std::string& amount) {
        std::string message = "DEPOSIT " + amount;
        sendBankMsg(message.c_str());
        std::string response = recvBankMsg();
        if (response == "APPROVED") {
            std::cout << "Deposit Approved" << std::endl;
            return 1;
        }else if (response == "LIMIT") {
            std::cout << "Bank's balance limit reached" << std::endl;
            return 0;
        }
        else{
            std::cerr << "Deposit Not Approved" << std::endl;
            return 0;
        }
    }

    int withdraw(const std::string& amount) {
        sendBankMsg("WITHDRAW "+amount);
        std::string response = recvBankMsg();
        if (response == "APPROVED") {
            std::cout << "Withdraw Approved" << std::endl;
            return 1;
        }else{
            std::cerr << "Withdraw Not Approved" << std::endl;
            return 0;
        }
    }


    std::string balance() {
        sendBankMsg("BALANCE");
        std::string response = recvBankMsg();
        return response;
    }

    int sendBankMsg (const std::string& msg) {
        std::string encryptedMsg = encrypt_msg(msg);
        int bytes_sent = send(client, encryptedMsg.c_str(), encryptedMsg.length(), 0);

        if (bytes_sent == -1) {
            std::cerr << "Error sending message.\n";
            return -1;
        }
        return 1;
    }

    std::string recvBankMsg () {
        char buffer[8192] = {0};
        int len = recv(client, buffer, sizeof(buffer), 0);
        auto msg = decrypt_msg(buffer, len);
//        std::cout << "Server: " << msg.second << std::endl;
        return std::string(msg.second);
    }


   

public:
    ATM(int in_port) {
        std::ifstream in("keys/public.txt");
        public_key = RSA::parsePublicKey(in);
        seq_num = 0;
        port = in_port;
    }
    ~ATM() {
        close(client);
    }

    


    void displayMenu() {
        std::cout << "\nATM" << std::endl;
        std::cout << "1. Deposit" << std::endl;
        std::cout << "2. Withdraw Money" << std::endl;
        std::cout << "3. Check Balance" << std::endl;
        std::cout << "4. Exit" << std::endl;
    }

    void run() {

        run_client();
        

        std::string bankCardNumber, pin;
        int authenticated=0;
        int attempts = 0;
        while(authenticated==0){
            std::cout << "\nEnter your bank card number: ";
            std::cin >> bankCardNumber;
            std::cout << "Enter your PIN: ";
            std::cin >> pin;
            authenticated = login(bankCardNumber, pin);
            if(authenticated){
                break;
            }
            std::cerr << "Incorrect Credentials, Try Again.\n";
            if(attempts>=3){
                std::cerr << "Too Many Attempts. Exiting...\n";
                exit(1);
            }
            attempts++;

        }

        std::cout << "Logged In.\n";

        std::string amount;
        while (authenticated) {
            displayMenu();
            std::cout << "Enter your choice: ";
            int choice;
            std::cin >> choice;
            switch (choice) {
                case 1: {
                    std::cout << "\nEnter amount to deposit: $";
                    std::cin >> amount;
                    std::cout << std::endl;
                    deposit(amount);
                    break;
                }
                case 2:
                    std::cout << "\nEnter amount to withdraw: $";
                    std::cin >> amount;
                    std::cout << std::endl;
                    withdraw(amount);
                    break;
                case 3:
                    amount = balance();
                    std::cout << "\nYour Current Balance is: $" << amount << std::endl;
                    break;
                case 4:
                    std::cout << "Exiting ATM.\n" << std::endl;
                    exit(1);
                default:
                    std::cout << "Invalid choice, please try again.\n" << std::endl;
            }
        }
    }
};


int main(int argc, char** argv) {

    if (argc != 2) {
        std::cout << "Provide port number!" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);
  
    ATM atm(port);
    atm.run();

  
    return 0;
}
