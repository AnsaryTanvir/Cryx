#include <iostream>
#include <string>
using namespace std;

/* Define unsigned char as byte*/
typedef unsigned char Byte;

const int header_size       =  4;
const int passwordHash_size = 20;
const int metadata_size     = 24;

/**
 * Computes the hash value of a given string.
 *
 * @param str The input string to be hashed.
 * @return The hash value of the input string as a 20-character string.
 */
string getStringHash(const string& str) {

    string hash = to_string(std::hash<string>{}(str));

    /* Pad the hash value with zeros to ensure a length of 20 characters */
    while (hash.size() < 20)
        hash.push_back('0');

    return hash;
}



int main( int argc, char* argv[] ){

    /* 
        If the argument count is not 2,
        It means either more than one file or no file is given in input.
        Therefore, exit the program.
    */
    if ( argc !=  2 ){
        cout << "Please drag a file to this program's executable." << endl;
        getchar();
        getchar();
        return -1;
    }

    /* 
        The name of the dragged file is stored in the argument vector argv[1].
        So let's try to open the binary file for reading.
        If it fails, exit the program.
    */
    FILE* binaryFile = fopen( argv[1] , "rb" );  /* The name of the dragged file is in argv[1]. */
    if ( binaryFile == NULL ){
        cerr << "Failed to open the dragged binary file." << endl;
        getchar();
        getchar();
        return -1;
    }

    /* Determine the file size. */
    fseek( binaryFile , 0 , SEEK_END);
    size_t fileSize = ftell(binaryFile);
    fseek( binaryFile, 0, SEEK_SET);

    /* Create a byte array using dynamic memory allocation. */
    Byte* byteArray = new Byte[ fileSize ];

    /*  Read all the bytes from the file into the byte array,
        and then close the dragged binary file.
    */
    fread( byteArray, fileSize, 1 , binaryFile );
    fclose(binaryFile);

    cout << "Enter 'E/e' for encryption or 'D/d' for Decryption " << endl;
    char choice; cin >> choice;

    if ( choice == 'E' || choice == 'e' ){

        /* Check if the file is already encrypted by Cryx. */
        if ( byteArray[0] == 'C' and byteArray[1] == 'r' and byteArray [2] == 'y' and byteArray[3] == 'x' ){
            cout << "This file is already encrypted by Cryx." << endl;
            getchar();
            getchar();
            return -1;
        }
        
        /* Show the selected file and prompt for a password. */
        cout << "Your selected file is: " << argv[1] << endl;
        cout << "Enter a password: ";
        string password; cin >> password;
        
        /* The encrypted file's extension is ".cryx" , so add it to the end of the file name.*/
        string fileName = argv[1] + string(".cryx");

        /* Create a new empty binary file for writing.*/
        FILE* newBinaryFile = fopen( fileName.c_str() , "wb");
        if ( newBinaryFile == NULL ){
            cerr << "Failed to create new binary file" << endl;
            getchar();
            getchar();
            return -1;
        }

        /* Encrypting bytes using the XOR cipher. */
        size_t length = password.length();
        for ( int i = 0; i < (int) fileSize; i++ )
            byteArray[i] = byteArray[i] ^ password[ i % length ];


        /*  Add the metadata in the first few bytes of the encrypted file.
            Metadata contains the header and the hash of the password, which is used to encrypt the file.
         */
        string header       = "Cryx";
        string passwordHash = getStringHash(password);
        string metadata     = header + passwordHash;

        /*  Now, as the encrypted file will contain both metadata and the encrypted bytes,
            So, we need to create a new array to hold both metadata and the encrypted bytes.
            
            Therefore, we needed to add the size of the metadata with the file size.*/
        size_t newSize     = metadata_size + fileSize;
        Byte* newByteArray = new Byte[ newSize ];

        /* First, store the metadata in the newByteArray.*/
        for ( int i = 0; i < (int)  metadata_size; i++ )
            newByteArray[i] = metadata[i];

        /* Next, just after the metadata, store the encrypted bytes in the newByteArray.*/
        for ( int i = metadata_size; i < (int) newSize; i++ )
            newByteArray[i] = byteArray[i-metadata_size];
        

        /*  Now write all the bytes into the newBinaryFile.
            Then newBinaryFile will contain metadata plus the encrypted bytes.
            Then close the newBinaryFile and free the memory allocated by the newByteArray.
        */
        fwrite( newByteArray, newSize, 1, newBinaryFile );
        fclose(newBinaryFile);
        delete newByteArray;

        /* Show the success message and the destination where the encrypted file is stored.*/
        cout << "File succesfully encrypted and stored at: " << endl;
        cout << fileName << endl;
        getchar();
        getchar();
        return 0;
    }

    else if ( choice == 'D' || choice == 'd' ){

        /* First, we need to analyze the file's metadata.*/
        string metadata;
        for ( int i = 0; i < (int) metadata_size; i++ )
            metadata.push_back(byteArray[i]);

        /* Split the metadata into the header and the hash of the password.*/
        string headerStoredInMetadata       = metadata.substr(0, header_size);
        string passwordHashStordInMetadata  = metadata.substr(header_size, passwordHash_size );

        /*  Any file that is encrypted by Cryx must have the header "Cryx"
            If the header is not "Cryx" then it was never encrypted by Cryx.
         */
        if ( headerStoredInMetadata != "Cryx" ){
            cout << "Sorry! This file is not encrypted by Cryx." << endl;
            getchar();
            getchar();
            return -1;
        }

        /* Show the selected file and prompt for a password. */
        cout << "Your select file is: " << argv[1] << endl;
        cout << "Enter a password: ";
        string password; cin >> password;

        /*  The hash of the password that was used to encrypt the file is stored in the metadata.
            If we enter the same password that was used to encrypt the file,
            then the hash of the given password must match the hash stored in metadata.

            If the hash of the password doesn't match, then the password is wrong.
         */
        if ( getStringHash(password) != passwordHashStordInMetadata ){
            cout << "Sorry! Wrong Password." << endl;
            cout << "Try again with the correct password." << endl;
            getchar();
            getchar();
            return -1;
        }

        /*  Since the encrypted file contains both metadata and the encrypted bytes,
            but we only need the encrypted bytes and not the metadata.
            We create a newByteArray, which will only contain the encrypted bytes.

            Therefore, we needed to subtract the size of the metadata from the file size.
        */

        size_t newSize      = fileSize - metadata_size;
        Byte*  newByteArray = new Byte[ newSize ];

        /* Skip the metadata and store only the encrypted bytes in the newByteArray.*/
        for ( int i = metadata_size; i < (int) newSize; i++ )
            newByteArray[i-metadata_size] = byteArray[i];

        /* The decrypted file should have the extension "Cryx." removed, so remove it. */
        string fileName = argv[1];
        fileName.erase( fileName.size() - string("Cryx").length() );

        /* Create a newBinaryFile to write all the decrypted bytes.*/
        FILE* newBinaryFile = fopen( fileName.c_str() , "wb");
        if ( newBinaryFile == NULL ){
            cout << "Failed to crate new binary file" << endl;
            getchar();
            getchar();
            return -1;
        }

        /* Decrypting bytes using the XOR cipher. */
        size_t length = password.length();
        for ( int i = 0; i < (int) fileSize; i++ )
            newByteArray[i] = newByteArray[i] ^ password[ i % length ];

        /*  Now write all the bytes into the newBinaryFile.
            The newBinaryFile will contain the decrypted bytes.
            Then close the newBinaryFile.
        */
        fwrite( newByteArray, newSize, 1, newBinaryFile );
        fclose(newBinaryFile);
        
        /* Show the success message and the destination where the decrypted file is stored.*/
        cout << "File successfuly decrypted and stored at: " << endl;
        cout << fileName << endl;

        /* Delete the .cryx file*/
        remove(argv[1]);
        getchar();
        getchar();
        return 0;
    }
    else {
        cout << "You didn't select any option." << endl;
        cout << "Press enter to exit." << endl;
        getchar();
        return -1;
    }

    return 0;
}
