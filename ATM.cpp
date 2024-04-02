#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "SHA-1.h"
#include <boost/multiprecision/cpp_int.hpp>

// ATM Client Instance
class ATM {
private:

    int port = 1500;
    std::string ip = "127.0.0.1";
    int client;

    std::string public_key = "987654321";

    std::string encrypt(const std::string& msg) {
        std::string encrypted_msg = msg;
        return encrypted_msg;
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

    // Connect to server
    if (connect(client, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error connecting to server.\n";
        exit(1);
    }

    std::cout << "Connected to server.\n";

    // Receive message from server
    recvBankMsg();

    // Send message to server
    sendBankMsg("Message from ATM Client.\n");

}

int login(const std::string& bank_card, const std::string& pass) {
    std::string message = "LOGIN " + hash_msg(bank_card) + " " + hash_msg(pass);
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
    }else{
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
    std::string encryptedMsg = encrypt(msg);
    int bytes_sent = send(client, encryptedMsg.c_str(), encryptedMsg.length(), 0);

    if (bytes_sent == -1) {
        std::cerr << "Error sending message.\n";
        return -1;
    }
    return 1;
}

std::string recvBankMsg () {
    char buffer[1024] = {0};
    recv(client, buffer, sizeof(buffer), 0);
    std::cout << "Server: " << buffer << std::endl;
    return std::string(buffer);
}


   

public:
    ATM() {
       
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
                    std::cout << "Your Current Balance is: $" << amount << std::endl;
                    break;
                case 4:
                    std::cout << "Exiting ATM.\n" << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice, please try again.\n" << std::endl;
            }
        }
    }
};


int main() {
  
    ATM atm;
    atm.run();

  
    return 0;
}
