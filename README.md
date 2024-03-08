## Cryptography Final Bank Project

### Outline:
# Customer:
1. **Authentication:**
   - Has a bank card and password.
   - Upon login, the customer provides the bank card and password.
   - The bank card is used to retrieve the corresponding private key from the bank.
   - Password is hashed using SHA-2 before transmission.
   
2. **Communication with ATM:**
   - Utilizes a user interface provided by the ATM for login and transactions.
   - Transactions include query, withdraw, and transfer.
   - The ATM stores the bank's public key for encryption purposes.
   - Messages exchanged between customer and ATM are encrypted using shared key cryptography (e.g., AES).
   - Authentication of the customer to the ATM is ensured through a secure protocol (e.g., SSL/TLS).

# ATM:
1. **User Interface:**
   - Provides functionalities for login and various transactions.
   - Ensures a secure login process using the customer's bank card and password.

2. **Communication with Bank:**
   - Stores the bank's public key for encryption of messages.
   - Utilizes shared key cryptography for sending encrypted messages to the bank.
   - Implements a secure communication protocol with the bank to ensure confidentiality and integrity.
   - Authenticates the bank to the ATM to prevent man-in-the-middle attacks.

# Bank:
1. **Security Measures:**
   - Stores the private key corresponding to the ATM securely.
   - Maintains a log of activity history for auditing purposes.
   - Manages bank cards and their associated public/private keys.

2. **Communication with ATM:**
   - Provides a method to retrieve the bank's public key upon request from the ATM.
   - Uses RSA asymmetric encryption for secure communication with the ATM.
   - Ensures message integrity and authenticity through digital signatures using RSA.
   - Utilizes secure hashing algorithms (e.g., SHA-2) for additional security measures.

# Encryption Methods:
1. **RSA Asymmetric Encryption:**
   - Utilized for secure key exchange and communication between the bank and ATM.
   - Ensures confidentiality and integrity of messages exchanged between the entities.

2. **AES Symmetric Encryption:**
   - Used for encrypting messages between the customer and ATM due to its efficiency in bulk data encryption.
   - Provides confidentiality of transaction details during communication.

3. **SHA-2 Hashing:**
   - Employed for hashing sensitive data such as passwords and ensuring data integrity.
   - Used in combination with encryption methods to enhance security measures.

Resources:
https://www.cs.cornell.edu/courses/cs513/2000SP/proj.02.bankATM.html

