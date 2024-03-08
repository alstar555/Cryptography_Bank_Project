## Cryptography Final Bank Project

### Outline:
#### Customer:
1. **Authentication:**
   - Has a bank card and password for login
   - The bank card is used to retrieve the corresponding private key from the bank.
   - Password is hashed using SHA-2 before transmission.
   
2. **Communication with ATM:**
   - Utilizes a user interface provided by the ATM for login and transactions.
   - Transactions include query, withdraw, and transfer.
   - The ATM stores the bank's public key for encryption.
   - Messages exchanged between customer and ATM are encrypted using shared key cryptography (AES).
   - Authentication of the customer to the ATM is ensured through a secure protocol (SSL/TLS).

#### ATM:
1. **User Interface:**
   - Provides functionalities for login and transactions.

2. **Communication with Bank:**
   - Stores the bank's public key for encryption of messages.
   - Shared key cryptography for sending encrypted messages to the bank.
   - Secure communication protocol with the bank.
   - Authenticates bank to the ATM to prevent man-in-the-middle attacks.

#### Bank:
1. **Security Measures:**
   - Stores the private key corresponding to the ATM securely.
   - Maintains a log of activity history.
   - Manages bank cards and their associated public and private keys.

2. **Communication with ATM:**
   - Provides a method to retrieve the bank's public key to the ATM.
   - Uses RSA asymmetric encryption for secure communication with the ATM.
   - Digital signatures using RSA.
   - Uses hashing algorithms (SHA-2).
   - MAC is generated for each message.

#### Encryption Methods:
1. **RSA Asymmetric Encryption:**
   - Secure key exchange and communication between the bank and ATM.

2. **AES Symmetric Encryption:**
   - Encrypting messages between the customer and ATM.

3. **SHA-2 Hashing:**
   - Hashing data and passwords

#### Resources:
https://www.cs.cornell.edu/courses/cs513/2000SP/proj.02.bankATM.html

