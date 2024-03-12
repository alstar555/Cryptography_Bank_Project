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

    string encrypt(const string& msg) {
        string encrypted_msg = msg;
        return encrypted_msg;
    }

    string hash_msg(const string& msg) {
        string hashed_msg = msg;
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
    recvBankMsg();

    // Send message to server
    sendBankMsg("Message from ATM Client.\n");

}

int login(const string& bank_card, const string& pass) {
    string message = "LOGIN " + hash_msg(bank_card) + " " + hash_msg(pass);
    sendBankMsg(message.c_str());
    string response = recvBankMsg();
    if (response == "APPROVED") {
        return 1;
    }else{
        return 0;
    }
}

int deposit(const string& amount) {
    string message = "DEPOSIT " + amount;
    sendBankMsg(message.c_str());
    return 0;
}

int withdraw(const string& amount) {
    sendBankMsg("WITHDRAW "+amount);
    return 0;
}


int balance() {
    sendBankMsg("BALANCE");
    return 0;
}

int sendBankMsg (const string& msg) {
    std::string encryptedMsg = encrypt(msg);
    int bytes_sent = send(client, encryptedMsg.c_str(), encryptedMsg.length(), 0);

    if (bytes_sent == -1) {
        std::cerr << "Error sending message.\n";
        return -1;
    }
    return 1;
}

string recvBankMsg () {
    char buffer[1024] = {0};
    recv(client, buffer, sizeof(buffer), 0);
    cout << "Server: " << buffer << endl;
    return string(buffer);
}


   

public:
    ATM() {
       
    }
    ~ATM() {
        close(client);
    }

    


    void displayMenu() {
        std::cout << "ATM" << std::endl;
        std::cout << "1. Deposit" << std::endl;
        std::cout << "2. Withdraw Money" << std::endl;
        std::cout << "3. Check Balance" << std::endl;
        std::cout << "4. Exit" << std::endl;
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

        string amount;
        while (authenticated) {
            displayMenu();
            std::cout << "Enter your choice: ";
            int choice;
            std::cin >> choice;
            switch (choice) {
                case 1: {
                    std::cout << "\nEnter amount to deposit: $";
                    std::cin >> amount;
                    cout << endl;
                    deposit(amount);
                    break;
                }
                case 2:
                    std::cout << "\nEnter amount to withdraw: $";
                    std::cin >> amount;
                    cout << endl;
                    withdraw(amount);
                    break;
                case 3:
                    balance();
                    break;
                case 4:
                    std::cout << "Exiting ATM.\n" << std::endl;
                    return;
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
