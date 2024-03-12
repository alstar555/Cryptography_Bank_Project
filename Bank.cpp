#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sstream>
#include <map>


using namespace std;


// Bank Server Instance
class Bank {
private:
    map<string, string> credentials_database;

    int server;
    int client;
    int port = 1500;
    string private_key = "123456789";

    string decrypt(const string& msg) {
        string decrypted_msg = msg;
        return decrypted_msg;
    }

    string recvATMMsg() {
        char buffer[1024] = {0};
        memset(buffer, 0, sizeof(buffer));
        int bytes_recv = recv(client, buffer, sizeof(buffer), 0);
        if (bytes_recv <= 0) {
            return ""; 
        }
        return string(buffer);
    }

    int validate_login(const string& bank_card, const string& pass){
        if (credentials_database.find(bank_card) == credentials_database.end()){
            return 0;
        }else{
            if(credentials_database[bank_card] == pass){
                return 1;
            }
        }
        return 0;
    }



void run_server() {
    // Create socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        cerr << "Error creating socket.\n";
        exit(1);
    }

    // Address setup
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind socket
    if (bind(server, (struct sockaddr *)&address, sizeof(address)) == -1) {
        cerr << "Error binding.\n";
        exit(1);
    }

    // Listen
    listen(server, 1);
    cout << "Server is listening...\n";

    // Accept connection
    while (true) {
        socklen_t size = sizeof(address);
        client = accept(server, (struct sockaddr *)&address, &size);
        if (client < 0) {
            cerr << "Error accepting connection.\n";
            continue;
        }

        cout << "Client connected.\n";

        // Send message to client
        char buffer[1024] = "Message from Bank Server.\n";
        send(client, buffer, strlen(buffer), 0);

        // Receive messages from client
        while (true) {
            // Read the length of the incoming message
            string buffer_str = recvATMMsg();
            
            // Check if the client disconnected
            if (buffer_str == "") {
                cout << "Client disconnected.\n";
                break;
            }

            // Decrypt request
            string request = decrypt(buffer_str);

            cout << "Client: " << request << endl;

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
                string bank_card = tokens[1];
                string pass = tokens[2]; 
                cout << "bank_card: " << bank_card << endl;
                cout << "pass: " << pass << endl;
                authenticated = validate_login(bank_card, pass);
                if(authenticated){
                    response = "APPROVED";
                    cout << "Login Successful.\n";
                }else{
                    cout << "Login not Approved.\n";
                    response = "BAD LOGIN";
                }
                send(client, response, strlen(response), 0);
            }
            else if (tokens[0] == "DEPOSIT") {
                response = "Deposit Approved.\n";
                send(client, response, strlen(response), 0);
            }
            else if (tokens[0] == "WITHDRAW") {
                response = "Withdraw Approved.\n";
                send(client, response, strlen(response), 0);
            }
            else if (tokens[0] == "BALANCE") {
                response = "Balance Amount:\n";
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

    void create_customer_credentials(const string& bank_card, const string& pass){
        if (credentials_database.find(bank_card) == credentials_database.end()){
            credentials_database[bank_card] = pass;
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
