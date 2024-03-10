#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

using namespace std;


// Bank Server Instance
class Bank {
private:

    int port = 1500;
    string private_key = "123456789";

    string decrypt_msg(const string& msg) {
        string decrypted_msg = msg + private_key;
        return decrypted_msg;
    }

   

public:
    Bank() {
       
    }
void run_server() {
    // Create socket
    int server = socket(AF_INET, SOCK_STREAM, 0);
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
        int client;
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
            memset(buffer, 0, sizeof(buffer));
            int bytes_recv = recv(client, buffer, sizeof(buffer), 0);
            if (bytes_recv <= 0) {
                cout << "Client disconnected.\n";
                break;
            }
            cout << "Client: " << buffer << endl;
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
