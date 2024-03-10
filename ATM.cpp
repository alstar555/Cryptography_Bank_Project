#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>


using namespace std;


// ATM Client Instance
class ATM {
private:

    int port = 1500;
    string ip = "127.0.0.1";
    int client;

    string public_key = "987654321";

    string encrypt_msg(const string& msg) {
        string encrypted_msg = msg + public_key;
        return encrypted_msg;
    }

    string hash_msg(const string& msg) {
        string hashed_msg = msg + "_temp_hashed\n" ;
        return hashed_msg;
    }

    void run_client() {
    // Create socket
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        cerr << "Error creating socket.\n";
        exit(1);
    }

    // Address setup
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(client, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        cerr << "Error connecting to server.\n";
        exit(1);
    }

    cout << "Connected to server.\n";

    // Receive message from server
    char buffer[1024] = {0};
    recv(client, buffer, sizeof(buffer), 0);
    cout << "Server: " << buffer << endl;

    // Send message to server
    strcpy(buffer, "Message from ATM Client.\n");
    send(client, buffer, strlen(buffer), 0);

    // Close socket
    // close(client);
}

   

public:
    ATM() {
       
    }
    ~ATM() {
        close(client);
    }

    int login(const string& bank_card, const string& pass) {
        sendBankMsg(hash_msg(bank_card));
        sendBankMsg(hash_msg(pass));
        return 0;
      
    }

    int sendBankMsg (const string& msg) {
        const char* buffer = msg.c_str();
        int bytes_sent = send(client, buffer, strlen(buffer), 0);
        if (bytes_sent == -1) {
            cerr << "Error sending message.\n";
            return -1; 
        }
        return 1;
    }


    void displayMenu() {
        std::cout << "ATM" << std::endl;
        std::cout << "1. Withdraw" << std::endl;
        std::cout << "2. Transfer" << std::endl;
        std::cout << "3. Exit" << std::endl;
    }

    void run() {

        run_client();
        

        std::string bankCardNumber, pin;
        int authenticated=0;
        while(authenticated==0){
            std::cout << "\nEnter your bank card number: ";
            std::cin >> bankCardNumber;
            std::cout << "Enter your PIN: ";
            std::cin >> pin;
            authenticated = login(bankCardNumber, pin);
            if(authenticated){
                cout << "Logged In.\n";
                break;
            }
            cerr << "Incorrect Credentials, Try Again.\n";

        }

        cout << "Logged In.\n";

        while (authenticated) {
            displayMenu();
            std::cout << "Enter your choice: ";
            int choice;
            std::cin >> choice;
            switch (choice) {
                case 1: {
                    
                    break;
                }
                case 2:

                    break;
                case 3:
                    std::cout << "Exiting ATM." << std::endl;
                    return;
                default:
                    std::cout << "Invalid choice, please try again." << std::endl;
            }
        }
    }
};


int main() {
  
    ATM atm;
    atm.run();

  
    return 0;
}
