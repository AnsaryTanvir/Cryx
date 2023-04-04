# Cryx
Cryx is a simple software to demonstrate encryption and decryption of binary file using symmetric-key algorithm.


## Encryption

The software uses the following steps below to encrypt a binary file.

* Open a binary file for reading.
* Determine the number of bytes the binary file has i.e file size.
* Create a byte array to store all the bytes of the binary file.
* Read all the bytes from the binary file and store them into the byte array.
* Close the binary file and delete it.
#
* Using XOR cipher, encrypt each and every byte of the byte array.
* Create a new binary file to write all the encrypted bytes and additional information.
* Write the name of our software in the first few bytes of the file.
* Next write the hash of the password in the next few bytes of the file.
* Lastly write all the encrypted bytes in the next available space.
* Close the file.



## Decryption

The software uses the following steps below to decrypt a binary file encrypted by Cryx.

* Open an encrypted binary file for reading.
* Read the first few bytes to check if it was encrypted by Cryx.
* Read the next few byte to get the hash of the password used to encrypt the file.
* Determine the number of encrypted bytes the file has.
* Create a byte array to store all the encrypted bytes of the file.
* Read all the encrypted bytes from the file and store them into the byte array.
* Close the file.
#
* Now, match the hash of given password with the hash read from the encrypted file.
* Using XOR cipher, decrypt each and every byte of the byte array.
* Create a new binary file to write all the decrypted bytes.
* Write all the decrypted bytes in the file.
* Close the new file and remove the encrypted file also.
