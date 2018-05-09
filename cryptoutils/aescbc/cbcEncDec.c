#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>

#include <mbedtls/aes.h>
    
#define IV_LENGTH 16


/*
 *  Current problem: Having an issue with detecting the 
 *  IV that is appended to the beginning of the file
 *
 *  Certain blocks decrypt okay but others do not...
 *
 *  Weird.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */












typedef unsigned char * bytePtr;
typedef unsigned char  byte;


void padBlock(char * filename);






int main(int argc, char * argv[])
{
    srand(time(NULL));
    FILE * fp = fopen(argv[1], "r");
    size_t len;
    int ret,i;
    char option[3];
    option[3] = '\0';
    strcpy(option, argv[2]);
    if (argc == 4)
    {
        if (strlen(argv[3]) != 16)
        {
            printf("\n\nKey must be 16 bytes");
            exit(1);
        }
    }

    if (!strcmp(option, "-e"))
    {
        padBlock(argv[1]);
    }
    
    if ((argc < 3) || (argc > 4))
    {
        printf("\n\n\tAES-128 in CBC Mode Encryption/Decryption Tool\n");
        printf("\t\t\tWritten by: xdmtk\n");
        printf("\t-----------------------------------------------\n\n");
        printf("    Usage: aesecb <file> <-d/-e> <key>\n\n");
        printf("    *   -d - Decryption\n");
        printf("    *   -e - Encryption\n\n");
        printf("    *   IV is automatically generated\n\n");
        printf("\n\n    *     Key field is optional, default key will be used \(\"YELLOW SUBMARINE\")\n");
        printf("    *     Key MUST be exactly 16 characters\n\n");
        exit(1);
    }


        



    // File reading sequence
    if(!fp){
            exit(1);
    }
    // File length sequence
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rewind(fp);
    len += IV_LENGTH; 
    
    // Start file read
    bytePtr inBlock = calloc(len, sizeof(byte));
    if(fread(inBlock, sizeof(char), len-IV_LENGTH, fp) != len-IV_LENGTH){
            exit(1);
    }
    fclose(fp);
    // End file read
    //







            
    bytePtr outBlock = calloc(len+1+IV_LENGTH, sizeof(byte));
    // Define context structure for AES context 
    // Initialize AES context specifying rounds,
    // rounds for 128 bit keys is 10
    
    mbedtls_aes_context ctx;
    ctx.nr = 10;
    mbedtls_aes_init(&ctx);









    //////////////////////////////////////////////////////////////////////////////////////////////////////yy
    // Function to set th AES key, uses the context object
    // and takes the key and the number of bits in the key as the
    // second and third arguments
    if(!strcmp(option, "-d")) 
    {
        if (argc == 3)
        {
            printf("\n[+] Using default decryption key \"YELLOW SUBMARINE\"\n");
            ret = mbedtls_aes_setkey_dec(&ctx, "YELLOW SUBMARINE", 128);
        }
        else
        {
            if (strlen(argv[3]) != 16)
            {
                printf("\n[-] AES Requires 128 bit key\n");
                printf("[-] Exiting..\n\n");
            }
            else
            {
                ret = mbedtls_aes_setkey_dec(&ctx, argv[3], 128);
            }
        }

    }
    else
    {
        if (argc == 3)
        {
            printf("\n[+] Using default encryption key \"YELLOW SUBMARINE\"\n");
            ret = mbedtls_aes_setkey_enc(&ctx, "YELLOW SUBMARINE", 128);
        }
        else
        {
            if (strlen(argv[3]) != 16)
            {
                printf("\n[-] AES Requires 128 bit key\n");
                printf("[-] Exiting..\n\n");
            }
            else
            {
                ret = mbedtls_aes_setkey_enc(&ctx, argv[3], 128);
            }
        }
    }
       
    if (ret)
    {
        // Should return 0 on success
        printf("Error occurred");
        exit(1);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////yy
    //







    // Because the aes_cryp_ecb() function only works on single blocks
    // of 16 bytes, we must iterate 16 bytes at a time through the 
    // cipher and append the decrypted text to the outBlock pointer
    // for each iteration
    
    byte blockBufferIn[16];
    byte blockBufferOut[16];        
    
    printf("\n\tAES-128 In CBC Mode Encryption/Decryption Tool\n");
    printf("\t\tWritten by: xdmtk\n\n");
    system("sleep 3");

    
    // Randomize IV
    byte IV[16];
    



    if(!strcmp(option, "-e")) 
    {
        // Generate random IV blocl
        for (i = 0; i < 16; ++i)
        {
            IV[i] = (rand() % 255);
        }

        printf("[+] Generating random IV\n");
        system("sleep 2");


        // Write to beginning of file
        // Not the most secure but itll do for basic applications
        memcpy(outBlock, IV, 16);
    }
    else
    {
        // For decryption, get the IV from the file
        memcpy(IV, inBlock, 16);
    }
    int r;







    //////////////////////////////////////////////////////////////////////////////////////////////////////yy
    // Encrypt or decrypt process
    for (i = 0; i < len; i += 16)
    {
            // Copy 16 bytes from the input to the bufer
            memcpy(blockBufferIn, inBlock+i, 16);




            // For decryption
            if(!strcmp(option, "-d")) 
            {


                mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_DECRYPT, blockBufferIn, blockBufferOut);
                for (r = 0; r < 16; ++r)
                {
                    blockBufferOut[r] ^= IV[r];
                }  
                memcpy(IV, blockBufferIn, 16);
            }




            // For encryption
            else if(!strcmp(option, "-e")) 
            {
                for (r = 0; r < 16; ++r)
                {
                    blockBufferIn[r] ^= IV[r];
                }  
                // XOR with the IV
                mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, blockBufferIn, blockBufferOut);
                memcpy(IV, blockBufferOut, 16);
            }
            
            
            
            
            // Copy the cipher text produced into IV
            memcpy(outBlock+i+IV_LENGTH, blockBufferOut, 16);
    }
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////yy








    
    //////////////////////////////////////////////////////////////////////////////////////////////////////yy
    outBlock[len] = '\0';
    // Output message 
    if (!strcmp(option, "-d"))
    {
        printf("[+] Succussfully decrypted %d bytes\n", len);
    }
    else
    {
        printf("[+] Succussfully encrypted %d bytes\n", len);
    }

    int padbool = 0;
    unsigned char a,b;
    // Check for padding sequence
    if (outBlock[len-1] == outBlock [len-2])
    {
        a = outBlock[len-1];
        for (i = len, b = '\x00'; b < a+1; ++b, --i)
        {
            outBlock[len] = '\0';
        }
        len = len-(int)a;
        padbool = 1;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////yy







    //////////////////////////////////////////////////////////////////////////////////////////////////////yy
    FILE *fpWrite = fopen(argv[1], "w");
    fwrite(outBlock, 1, len, fpWrite);
    fclose(fpWrite);

    if (padbool)
    {
        printf("[+] Padded message detected, sucessfully stripped %d padded bytes\n", a);
    }
    printf("[+] Succussfully wrote %d bytes to %s\n", len, argv[1]);


    

    //////////////////////////////////////////////////////////////////////////////////////////////////////yy






	return 0;
}





void padBlock(char * filename)
{


    FILE * fp = fopen(filename, "r");
    int blockSize, padding, i;
    size_t len;

    // Get specified block size for message from command line argument 
    blockSize = 16;

    // Get message length of file
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rewind(fp);
    
    if (len % blockSize == 0)
    {
        return;
    }  
    // Determine padding amount by subtracting length
    // from the largest nearest blockSize multiple
    while (blockSize < len)
    {
        blockSize += blockSize;
    }
    padding = blockSize - len;

    // Create buffer with proper length for padded
    // message
    unsigned char paddedMsg[len+padding];


    // Read bytes into buffer and begin padding
    fread(paddedMsg, 1, len, fp);


    // Append bytes to end of original file
    for (i = len; i < (len+padding); ++i)
    {
        paddedMsg[i] = (unsigned char) padding;
    }
    fclose(fp);


    // Write new buffer to same file
    FILE * fpWrite = fopen(filename, "w");
    fwrite(paddedMsg, 1, len+padding, fpWrite);
    fclose (fpWrite);



    // Show status
    printf("\n\n");
    printf("Pre-padding message...\n\n");
    system("sleep 2");
    printf("[+] %d bytes read from %s\n", len, filename);
    printf("[+] %d bytes appended as padding\n", padding);
    printf("[+] %d bytes written to %s\n\n", len+padding, filename);

    







    return ;
}
