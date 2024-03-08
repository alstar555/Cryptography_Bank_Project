## Cryptography Final Bank Project

### Outline:

Customer:
* has a private key corresponding to Bank's private key
* customer login using bank card and password

ATM: 
* provides a user interface: login, transaction (query, withdraw, transfer)
* stores bank's public key
* uses shared key cryptography for sending messages
* autheticates the customer to the bank
* use protocol to secure communication with bank

Bank:
* stores private key corresponding to ATM
* logs activity history
* bank cards
* get_public_key()

Encryption Methods:
* RSA asymmetric
* AES symmetric
* SHA-2 hashing

Resources:
https://www.cs.cornell.edu/courses/cs513/2000SP/proj.02.bankATM.html

