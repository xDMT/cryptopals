#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>

#include <mbedtls/aes.h>
    


typedef unsigned char * bytePtr;
typedef unsigned char  byte;


void padBlock(char * filename);
int ecbDetect(char * file);






int main(int argc, char * argv[])
{
    srand(time(NULL));

    FILE * fp = fopen(argv[1], "r");
    size_t len;
    int ret,i;
    char option[3], key[16];
    option[3] = '\0';
    strcpy(option, argv[2]);





    // Argument checking
    // ///////////////////////////////////////////////////////
    //
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
    else
    {
        // Turning off decrypt mode
        exit(0);
    }
    
    if ((argc < 3) || (argc > 4))
    {
        exit(1);
    }

    // 
    // ///////////////////////////////////////////////////////
    //




    // File reading sequence
    if(!fp){
            exit(1);
    }
    // File length sequence
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rewind(fp);


    
    // Start file read
    bytePtr inBlock = calloc(len, sizeof(byte));
    fread(inBlock, sizeof(char), len, fp);
    fclose(fp);
    // End file read
    //



    ////////////////
    // Now append bytes



            
    bytePtr outBlock = calloc(len+1, sizeof(byte));
    // Define context structure for AES context 
    // Initialize AES context specifying rounds,
    // rounds for 128 bit keys is 10
    
    mbedtls_aes_context ctx;
    ctx.nr = 10;
    mbedtls_aes_init(&ctx);









    //////////////////////////////////////////////////////////////////////////////////////////////////////yy
    // Function to set th AES key, uses the context object
    // 

    if(!strcmp(option, "-d")) 
    {
        // Instead of modifying this code heavily..
        // im just going to temporarily disable the decrypt mode
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
            // And instead of using a default key, were going to generate a
            // random AES key
            printf("\n[+] Generating random 128 bit AES key\n");


            for (i = 0; i < 16; ++i)
            {
                key[i] = (rand() % 255);
            }

            ret = mbedtls_aes_setkey_enc(&ctx, key, 128);
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






    // Set up in/out buffers and IV for 
    // CBC (random)
    ///////////////////////////////////////////////////////////////////////////////// 
    
    byte blockBufferIn[16];
    byte blockBufferOut[16];        
    byte IV[16];

    // Apply IV for CBC mode
    // Generate random IV blocl
    for (i = 0; i < 16; ++i)
    {
        IV[i] = (rand() % 255);
    }
    int r;
    int ecb;


    //
    //
    ///////////////////////////////////////////////////////////////////////////////// 








    // Randomly choose between ECB mode and CBC mode
    // Give some good iterations to make sure its random
    for (r = 0; r < 50; ++r)
    {
        ecb = (rand() % 2);
    }


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
                if (ecb == 1)
                {

                    mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, blockBufferIn, blockBufferOut);
                }
                else
                {
                    for (r = 0; r < 16; ++r)
                    {
                        blockBufferIn[r] ^= IV[r];
                    }  
                    // XOR with the IV
                    mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, blockBufferIn, blockBufferOut);
                    memcpy(IV, blockBufferOut, 16);
                }
            }
            
            
            
       
            // Copy the cipher text produced into IV
            memcpy(outBlock+i, blockBufferOut, 16);
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


    system("sleep 1");
    printf("\n\n\n[+] Detecting cipher mode...\n");
    system("sleep 4");
    
    if ( ecbDetect(argv[1]))
    {
        printf("[+] ECB mode detected \n\n");
        
    }
    else
    { 
        printf("[+] CBC mode detected \n\n");
    }
    if (ecb == 1)
    {
        system("sleep 2");
        printf("[+] Actual mode used: ECB\n");
    }
    else
    {

        system("sleep 2");
        printf("[+] Actual mode used: CBC\n");
    }


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
    
    
    // Append bytes before and after plaintext (count 
    // chosen randomly)
    //
    //////////////////////////////////////////////////////////////////////////
    
    int appendCount;
    do
    {
         appendCount = (rand() % 11);   
    } while (appendCount < 5);


    // Add appended bytes to new buffer length
    len += (appendCount*2);




    if (len % blockSize == 0)
    {
        return;
    }  
    // Determine padding amount by subtracting length
    // from the largest nearest blockSize multiple
    while (blockSize < len)
    {
        blockSize += 16;
    }
    padding = blockSize - len;

    // Create buffer with proper length for padded
    // message
    unsigned char paddedMsg[len+padding];

    for (i = 0; i < appendCount; ++i)
    {
        paddedMsg[i] = 'A';
    }

    // Read bytes into buffer and begin padding
    fread(paddedMsg+appendCount, 1, len, fp);

    for (i = len-appendCount-1; i < len; ++i)
    {
        paddedMsg[i] = 'A';
    }

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


int ecbDetect(char * file)
{

    FILE * fp = fopen(file, "r");
    char * line = NULL;
    size_t len;
    int i, f, z = 0, ret; 


    while (getline(&line, &len, fp) != -1)
    {

        z++;
        // Remember space for the null terminator!!
        byte buffer1[17], buffer2[17];
        buffer1[16] = '\0';
        buffer2[16] = '\0';


        // Compare blocks 
        for (i = 0; i < len-32; i += 16)
        {
            memcpy(buffer1, line+i, 16);

            // Second block starts 1 block past comparison
            // block, position 'i'
            for (f = 16; f+i < len; f += 16)
            {

                memcpy(buffer2, line+i+f, 16);
                
                // On 0 return value, blocks are equal
                // indicating ecbECB mode
                if (!strcmp(buffer1,buffer2))
                {

                    printf("\n\n\nLine %d ECB Mode Detected\n", z, line);
                    printf("Repeated byte block #%d: %s\n", (i/16)+1, buffer1);
                    printf("Repeated byte block #%d: %s\n", ((f+i)/16)+1, buffer2);
                    return 1;
                }
            }
        }
    }
    return 0;
}

