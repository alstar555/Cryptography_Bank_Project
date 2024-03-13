#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sstream>
#include <map>
#include "SHA-1.cpp"

// Bank Server Instance
class Bank {
private:
    std::map<std::string, std::string> credentials_database; //stores bank card and pin
    std::map<std::string, int> bank_account_database; //stores account balance of users

    int server;
    int client;
    int port = 1500;
    std::string private_key = "123456789";

    std::string decrypt(const std::string& msg) {
        std::string decrypted_msg = msg;
        return decrypted_msg;
    }

    std::string recvATMMsg() {
        char buffer[1024] = {0};
        memset(buffer, 0, sizeof(buffer));
        int bytes_recv = recv(client, buffer, sizeof(buffer), 0);
        if (bytes_recv <= 0) {
            return ""; 
        }
        return std::string(buffer);
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
        bank_account_database[bank_card] += amount;
    }

     void withdraw(const int amount, const std::string& bank_card){
        bank_account_database[bank_card] -= amount;
    }

     int get_balance(const std::string& bank_card){
        return bank_account_database[bank_card] ;
    }

    std::string hash_msg(const std::string& msg) {
        return msg;
    }

    void run_server() {
        // Create socket
        server = socket(AF_INET, SOCK_STREAM, 0);
        if (server < 0) {
            std::cerr << "Error creating socket.\n";
            exit(1);
        }

        // Address setup
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Bind socket
        if (bind(server, (struct sockaddr *)&address, sizeof(address)) == -1) {
            std::cerr << "Error binding.\n";
            exit(1);
        }

        // Listen
        listen(server, 1);
        std::cout << "Server is listening...\n";

        // Accept connection
        std::string bank_card;
        while (true) {
            socklen_t size = sizeof(address);
            client = accept(server, (struct sockaddr *)&address, &size);
            if (client < 0) {
                std::cerr << "Error accepting connection.\n";
                continue;
            }

            std::cout << "Client connected.\n";

            // Send message to client
            char buffer[1024] = "Message from Bank Server.\n";
            send(client, buffer, strlen(buffer), 0);

            // Receive messages from client
            while (true) {
                // Read the length of the incoming message
                std::string buffer_str = recvATMMsg();
                
                // Check if the client disconnected
                if (buffer_str == "") {
                    std::cout << "Client disconnected.\n";
                    break;
                }

                // Decrypt request
                std::string request = decrypt(buffer_str);

                std::cout << "Client: " << request << std::endl;

                // Process Client requests
                std::istringstream iss(request);
                std::vector<std::string> tokens;
                std::string token;
                while (std::getline(iss, token, ' ')) {
                    tokens.push_back(token);
                }
                const char* response;
                int authenticated = 0;
                if (tokens[0] == "LOGIN") {
                    bank_card = tokens[1];
                    std::string pass = tokens[2];
                    std::cout << "bank_card: " << bank_card << std::endl;
                    std::cout << "pass: " << pass << std::endl;
                    authenticated = validate_login(bank_card, pass);
                    if(authenticated){
                        response = "APPROVED";
                        std::cout << "Login Successful.\n";
                    }else{
                        std::cout << "Login not Approved.\n";
                        response = "BAD LOGIN";
                    }
                    send(client, response, strlen(response), 0);
                }
                else if (tokens[0] == "DEPOSIT") {
                    int amount = stoi(tokens[1]);
                    deposit(amount, bank_card);
                    response = "APPROVED";
                    send(client, response, strlen(response), 0);
                }
                else if (tokens[0] == "WITHDRAW") {
                    int amount = stoi(tokens[1]);
                    withdraw(amount, bank_card);
                    response = "APPROVED";
                    send(client, response, strlen(response), 0);
                }
                else if (tokens[0] == "BALANCE") {
                    int balance = get_balance(bank_card);
                    response = std::to_string(balance).c_str();
                    send(client, response, strlen(response), 0);
                }
            }


            // Close client socket
            close(client);
        }

    }



public:
    Bank() {
       
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


int main() {
  
    Bank bank;

    //Create some customers
    bank.create_customer_credentials("1234", "4321");

    bank.run();

  
    return 0;
}
