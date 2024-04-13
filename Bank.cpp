#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sstream>
#include <map>
#include "crypto/SHA-1.h"
#include "crypto/RSA.h"
#include "proto/messages.pb.h"
#include "Client.h"
#include <fstream>
#include "Utils.h"
#include "crypto/DES.h"
#include "crypto/HMAC.h"

// Bank Server Instance
class Bank {
private:
    std::map<std::string, std::string> credentials_database; //stores bank card and pin
    std::map<std::string, int> bank_account_database; //stores account balance of users
    std::map<int, Client> fd_clients;

    int server_fd;
    int client_fd;
    int port = 1501;
    Client client;
    RSAPrivateKey private_key;
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
                client.getDESKey(),
                iv
        );
        auto enc_bytes = bytes_from_int(int_from_longs(enc_longs));

        EncryptedMessage* em = new EncryptedMessage;

        em->set_iv(iv);
        em->set_inner_msg(enc_bytes.data(), enc_bytes.size());

        MACMessage mm;

        mm.set_allocated_inner_msg(em);
        mm.set_mac(hmac(client.getHMACKey(), em->SerializeAsString()));

        return mm.SerializeAsString();
    }

    std::pair<bool, std::string> decrypt_msg(void* msg, int len) {
        MACMessage mm;
        if (!mm.ParseFromArray(msg, len)) {
            return std::make_pair(false, "1");
        }

//        SHA1 sha;

//        SHA1 sha;
//
//        std::cout << sha.hex_rep(mm.inner_msg().SerializeAsString()) << std::endl;
//
//        std::cout << convert_byte_str_to_hex(hmac(client.getHMACKey(), mm.inner_msg().SerializeAsString())) << std::endl;
//
//        std::cout << convert_byte_str_to_hex(mm.mac()) << std::endl;

        if (hmac(client.getHMACKey(), mm.inner_msg().SerializeAsString()) != mm.mac()) {
            return std::make_pair(false, "2");
        }

        EncryptedMessage em = mm.inner_msg();

        std::vector<uint8_t> enc_bytes(em.inner_msg().begin(), em.inner_msg().end());

        auto dec_longs = DES::decrypt3_cbc(
                longs_from_int(int_from_bytes(enc_bytes)),
                client.getDESKey(),
                em.iv()
        );

        auto dec_bytes = bytes_from_int(int_from_longs(dec_longs));

        UnencryptedMessage um;

        if (!um.ParseFromArray(dec_bytes.data(), dec_bytes.size())) {
            return std::make_pair(false, "3");
        }

        if (um.seq_num() < seq_num) {
            // repeat of previous message is not allowed!
            return std::make_pair(false, "4");
        }
        seq_num = um.seq_num();

        return std::make_pair(true, um.inner_msg());
    }

    // std::string recvATMMsg() {
    //     char buffer[8192] = {0};
    //     int len = recv(client_fd, buffer, 8192, 0);

    //     // verify HMAC

    //     // decrypt with DES

    //     //
    // }

    void sendATMMsg(const std::string& msg) {
        std::string enc = encrypt_msg(msg);
        send(client_fd, enc.data(), enc.size(), 0);
    }

    int validate_login(const std::string& bank_card, const std::string& pass){
        if (credentials_database.find(bank_card) == credentials_database.end()){
            return 0;
        }else{
            if(credentials_database[bank_card] == pass){
                return 1;
            }
        }
        return 0;
    }

    void deposit(const int amount, const std::string& bank_card){
        if (credentials_database.find(bank_card) == credentials_database.end()){
            std::cout << "ERROR: Account not found.";
            exit(1);
        }else{
            bank_account_database[bank_card] += amount;
        }
    }

     void withdraw(const int amount, const std::string& bank_card){
        bank_account_database[bank_card] -= amount;
    }

     int get_balance(const std::string& bank_card){
        return bank_account_database[bank_card] ;
    }

    std::string hash_msg(const std::string& msg) {
        SHA1 sha1;
        return sha1(msg);
    }

    void run_server() {
        // Create socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            std::cerr << "Error creating socket.\n";
            exit(1);
        }

        // Address setup
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Bind socket
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
            std::cerr << "Error binding.\n";
            exit(1);
        }

        // Listen
        listen(server_fd, 1);
        std::cout << "Server is listening...\n";

        // Accept connection
        std::string bank_card;
        while (true) {
            socklen_t size = sizeof(address);
            client_fd = accept(server_fd, (struct sockaddr *)&address, &size);
            if (client_fd < 0) {
                std::cerr << "Error accepting connection.\n";
                continue;
            }

            std::cout << "Client connected.\n";

            // Make new client object
            // This establishes fresh DH secrets
            client = Client();

            // Start "TLS" negotiation

            // send DH value signed by RSA key
            ServerHelloContent shc;

            std::vector<uint8_t> dh_send_bytes;
            export_bits(client.getDH().get_send_value(), std::back_inserter(dh_send_bytes), 8);

            shc.set_diffie_hellman_value(dh_send_bytes.data(), dh_send_bytes.size());

            std::string shc_str = shc.SerializeAsString();

            boost::multiprecision::cpp_int shc_str_int;
            boost::multiprecision::import_bits(shc_str_int, shc_str.begin(), shc_str.end());

            RSA rsa;

            // connection should just fail here since we will get wrong values after this
            assert(shc_str_int < private_key.getN());

            auto signature = rsa.encrypt(shc_str_int, private_key.getN(), private_key.getD());

            std::vector<uint8_t> sig_bytes;
            boost::multiprecision::export_bits(signature, std::back_inserter(sig_bytes), 8);

            ServerHello sh;

            sh.set_inner_msg(shc_str);
            sh.set_signature(sig_bytes.data(), sig_bytes.size());

            std::string msg = sh.SerializeAsString();

            send(client_fd, msg.data(), msg.size(), 0);

            std::cout << "INFO: Sent ServerHello to ATM" << std::endl;

            // receive DH value by other end and compute shared secret

            char buffer[8192] = {0};
            int len = recv(client_fd, buffer, sizeof(buffer), 0);

            ClientHello ch;

            if (!ch.ParseFromArray(buffer, len)) {
                std::cerr << "Failed to parse ClientHello message. Aborting connection..." << std::endl;
                close(client_fd);
                continue;
            }

            std::vector<uint8_t> bytes(ch.diffie_hellman_value().begin(), ch.diffie_hellman_value().end());

            boost::multiprecision::cpp_int imported_dh_value;
            boost::multiprecision::import_bits(imported_dh_value, bytes.begin(), bytes.end());

            client.getDH().build_shared_secret(imported_dh_value);
            client.build_des_key_from_dh();

            std::cout << "INFO: Successfully received ClientHello from ATM" << std::endl;

            // send DES encrypted message containing an HMAC key to use (sign this message too with RSA)
//            std::cout << client.getDH().get_shared_secret() << std::endl;
            HMACSend hs;
            auto random_bytes = get_random_bytes(20);
            auto hmac_key = std::string((char*) random_bytes.data(), random_bytes.size());
            client.setHMACKey(hmac_key);

            auto random_bytes_as_longs = longs_from_int(int_from_bytes(random_bytes));

            auto iv = random_iv();

            auto enc = DES::encrypt3_cbc(random_bytes_as_longs, client.getDESKey(), iv);

            auto encrypted_bytes = bytes_from_int(int_from_longs(enc));

            hs.set_encrypted_hmac(encrypted_bytes.data(), encrypted_bytes.size());
            hs.set_iv(iv);

            msg = hs.SerializeAsString();

            send(client_fd, msg.data(), msg.size(), 0);

            // handle fully encrypted messages from here on our

            std::string bank_card;
            std::string pin;
            std::string response;
            std::string _;
            int authenticated = 0;

            while (true) {
                // recv a msg
                char buffer[8192] = {0};
                int len = recv(client_fd, buffer, 8192, 0);

                auto res = decrypt_msg(buffer, len);

                if (!res.first) {
                    std::cerr << "Failed to receive message from ATM. Aborting connection..." << std::endl;
                    std::cerr << res.second << std::endl;
                    break;
                }

                std::cout << "INFO: Received msg " << res.second << std::endl;

                std::string cmd = res.second;
                std::stringstream ss(cmd);
                int amount;

                if (cmd.find("LOGIN") == 0) {
                    ss >> _ >> bank_card >> pin;
                    bank_card = hash_msg(bank_card);
                    pin = hash_msg(pin);
                    authenticated = validate_login(bank_card, pin);
                    if(authenticated){
                       response = "APPROVED";
                       std::cout << "Login Successful.\n";
                    }else{
                       std::cout << "Login not Approved.\n";
                       response = "BAD LOGIN";
                    }
                    sendATMMsg(response);
                }
                else if (cmd.find("DEPOSIT") == 0) {
                    ss >> _ >> amount;
                    deposit(amount, bank_card);
                    response = "APPROVED";
                    sendATMMsg(response);
               }
               else if (cmd.find("WITHDRAW") == 0) {
                   ss >> _ >> amount;
                   withdraw(amount, bank_card);
                   response = "APPROVED";
                   sendATMMsg(response);
               }
               else if (cmd.find("BALANCE") == 0) {
                   int balance = get_balance(bank_card);
                   response = std::to_string(balance).c_str();
                   sendATMMsg(response);
               }

            }

            // Close client_fd socket
            close(client_fd);
        }

    }



public:
    Bank(int in_port) {
        std::ifstream in("keys/private.txt");
        private_key = RSA::parsePrivateKey(in);
        seq_num = 0;
        port = in_port;
    }

    void create_customer_credentials(const std::string& bank_card, const std::string& pass){
        std::string hashed_bank_card = hash_msg(bank_card);
        std::string hashed_pass = hash_msg(pass);
        if (credentials_database.find(hashed_bank_card) == credentials_database.end()){
            credentials_database[hashed_bank_card] = hashed_pass;
            //Init bank amount to 0
            bank_account_database[hashed_bank_card] = 0;
            std::cout << "Credentials created for bank card: " << bank_card << std::endl;
        } else {
            std::cerr << "Bank card already exists in the database." << std::endl;
        }
    }

    void run() {

        run_server();
        
    }
};


int main(int argc, char** argv) {

    if (argc != 2) {
        std::cout << "Provide port number!" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);
  
    Bank bank(port);

    //Create some customers
    bank.create_customer_credentials("1234", "4321");

    bank.run();

  
    return 0;
}
