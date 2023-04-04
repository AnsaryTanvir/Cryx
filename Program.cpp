#include <iostream>
#include <iomanip>
using namespace std;

void printByteInHex(int number){
    cout << hex << setw(2) << setfill('0') << number << " ";
}

int main(){

    /* Open the binary file, if fails then exit the program */
    FILE* binaryFile = fopen("image.png", "rb");
    if ( binaryFile == NULL ){
        cerr << "Failed to open the binary file." << endl;
        return -1;
    }

    /* Determine the number of bytes in the file i.e file size */
    fseek(binaryFile, 0, SEEK_END);
    size_t fileSize = ftell(binaryFile);
    fseek(binaryFile, 0, SEEK_SET);

    cout << "Number of  Bytes: " << fileSize << " Pieces. " << endl;
    cout << "The Size of File: " << fileSize << " Bytes . " << endl;
    cout << endl;

    /* Create an array of byte to store all the bytes of the binary file*/
    unsigned char byteArray[fileSize];
    
    /* Now read all the bytes from the file and store it in the byteArray */
    fread(&byteArray, fileSize, 1, binaryFile);

    /* After Reading from the file we can close it*/
    fclose(binaryFile);



    /* Now print first 16 bytes from the byteArray */
    cout << "The first 16 bytes of the binary file are: " << endl;
    for ( int i = 0; i < 16; i++ )
        printByteInHex(byteArray[i]);

}