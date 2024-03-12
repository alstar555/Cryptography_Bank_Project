#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sstream>


using namespace std;


// Bank Server Instance
class Bank {
private:
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


   

public:
    Bank() {
       
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
            if (tokens[0] == "LOGIN") {
                string bank_card = tokens[1];
                string pass = tokens[2]; 
                cout << "bank_card: " << bank_card << endl;
                cout << "pass: " << pass << endl;
                const char* response = "APPROVED";
                send(client, response, strlen(response), 0);
                cout << "Login Successful.\n";
            }
            else if (tokens[0] == "DEPOSIT") {
                cout << "Deposit Approved.\n";
            }
            else if (tokens[0] == "WITHDRAW") {
                cout << "Withdraw Approved.\n";
            }
            else if (tokens[0] == "BALANCE") {
                cout << "Balance Amount:\n";
            }
        }


        // Close client socket
        close(client);
    }

}



  

    void run() {

        run_server();
        
    }
};


int main() {
  
    Bank bank;
    bank.run();

  
    return 0;
}
