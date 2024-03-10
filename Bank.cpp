#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

    void run_server(){

        //create socket
        int client = socket(AF_INET, SOCK_STREAM, 0);
        if (client < 0 ) {
            std::cerr << "Error connecting.\n";
            exit(1);
        }
        cout << "Connection established.\n";

        //bind socket
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(client, (struct sockaddr *)&address, sizeof(address)) == -1) {
            std::cerr << "Error binding.\n";
            exit(1);
        }

        socklen_t size = sizeof(address);
        std::cout << "Searching client...\n" << std::endl;

        //listen
        listen(client, 1);

        //accept connection
        int server = accept(client, (struct sockaddr*)&address, &size);

        //close socket
        close(client);
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
