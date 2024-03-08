#include <iostream>
#include <string>
#include <vector>

using namespace std;


class ATM {
private:
   

public:
    ATM() {
       
    }

    int login(const string& bank_card, const string& pass) {
    	return 1;
      
    }


    void displayMenu() {
        std::cout << "ATM" << std::endl;
        std::cout << "1. Withdraw" << std::endl;
        std::cout << "2. Transfer" << std::endl;
        std::cout << "3. Exit" << std::endl;
    }

    void run() {

    	std::string bankCardNumber, pin;
        std::cout << "Enter your bank card number: ";
        std::cin >> bankCardNumber;
        std::cout << "Enter your PIN: ";
        std::cin >> pin;

        int authenticated = login(bankCardNumber, pin);

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
