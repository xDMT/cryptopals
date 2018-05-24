#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>


#include <mbedtls/aes.h>
    





size_t b64decrypt(unsigned char * b64, unsigned char * instr, unsigned char * b64decryptStr);
unsigned char * padBlock(unsigned char * input);




int main(int argc, char * argv[])
{

    size_t len;
    int ret,i = 0;
    char c;
    unsigned char * b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    // Random key generation
    srand(time(NULL));
    unsigned char key[16];
    for (i = 0; i < 16; ++i)
    {
        key[i] = (rand() % 255);
    }

    // Unknown string
    unsigned char * strAppend = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
    unsigned char * initialBufb64 = malloc(10000);
    i = b64decrypt(b64, strAppend, initialBufb64);


    // Load base64 decoded string into strAppendDecode
    unsigned char * strAppendDecoded = calloc(i, sizeof(unsigned char));
    memcpy(strAppendDecoded, initialBufb64, i);
    free(initialBufb64);


    // Malloc a ton of bytes for initialize buffer, get length, malloc
    // new buffer, memcpy by len then delete other buffer

    while (1)
    {
        // Prompt for input
        unsigned char * initialBuf = malloc(10000);
        printf(">>");
        
        i = 0;
        while ((c = getchar()) != '\n') 
        {
            initialBuf[i++] = c;
        }
        i += strlen(strAppendDecoded); 
        len = i;

        // New buffer for size 
        unsigned char * inBlock = calloc(i+2 , sizeof(unsigned char));

        memcpy(inBlock, strAppendDecoded,strlen(strAppendDecoded));
        memcpy(inBlock+strlen(strAppendDecoded), initialBuf,i-strlen(strAppendDecoded));

        inBlock = padBlock(inBlock);


        free(initialBuf); 
        unsigned char * outBlock = calloc(i, sizeof(unsigned char));






        // Define context structure for AES context 
        // Initialize AES context specifying rounds,
        // rounds for 128 bit keys is 10
        
        mbedtls_aes_context ctx;
        ctx.nr = 10;
        mbedtls_aes_init(&ctx);



        // Function to set th AES key, uses the context object
        // and takes the key and the number of bits in the key as the
        // second and third arguments
        ret = mbedtls_aes_setkey_dec(&ctx, "YELLOW SUBMARINE", 128);
        if (ret)
        {
            // Should return 0 on success
            printf("Error occurred");
            exit(1);
        }



        // Because the aes_cryp_ecb() function only works on single blocks
        // of 16 bytes, we must iterate 16 bytes at a time through the 
        // cipher and append the decrypted text to the outBlock pointer
        // for each iteration
        
        unsigned char blockBufferIn[16];
        unsigned char blockBufferOut[16];        
        for (i = 0; i < len; i += 16)
        {
                memcpy(blockBufferIn, inBlock+i, 16); 
                mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_DECRYPT, blockBufferIn, blockBufferOut);
                memcpy(outBlock+i, blockBufferOut, 16);
        }




        printf("%x", outBlock);	


        free(inBlock);
        free(outBlock);
    } 




    // Remember the file needs to be base64 decoded first.
    // The code to do that is here, but just in case im going to
    // first use the openssl command line tool to accomplish that



	return 0;
}



size_t b64decrypt(unsigned char * b64, unsigned char * instr, unsigned char * b64decryptStr)
{

	int i, x, z, f;
	unsigned char * tempbyte = calloc(9, sizeof(unsigned char));
	unsigned char * pch;
	u_int64_t b64Read, out, hexOut;
        size_t len = strlen(instr);	



	for (i = 0, f = 0; i < len; )
	{
		b64Read = 0;
		for(z = 0; z < 8; ++i)
		{
			if (instr[i] != '\n')
			{
				tempbyte[z++] = instr[i];
			}
		}
		for (z = 0, x = 6; z < 8; ++z)
		{
			pch = strchr(b64, tempbyte[z]);
			out = pch-b64;	
			b64Read += out;
			if (z < 7)
			{
				b64Read = b64Read << x;
			}
		}
		
			hexOut = (b64Read & 0b0000000000000000111111110000000000000000000000000000000000000000);
			hexOut = hexOut >> 40;
			b64decryptStr[f++] = hexOut;
		

			hexOut = (b64Read & 0b0000000000000000000000001111111100000000000000000000000000000000);
			hexOut = hexOut >> 32;
			b64decryptStr[f++] = hexOut;


			hexOut = (b64Read & 0b0000000000000000000000000000000011111111000000000000000000000000);
			hexOut = hexOut >> 24;
			b64decryptStr[f++] = hexOut;

			hexOut = (b64Read & 0b0000000000000000000000000000000000000000111111110000000000000000);
			hexOut = hexOut >> 16;
			b64decryptStr[f++] = hexOut;

			hexOut = (b64Read & 0b0000000000000000000000000000000000000000000000001111111100000000);
			hexOut = hexOut >> 8;
			b64decryptStr[f++] = hexOut;


			hexOut = (b64Read & 0b0000000000000000000000000000000000000000000000000000000011111111);
			b64decryptStr[f++] = hexOut;

	}
	return f;
}















unsigned char * padBlock(unsigned char * input)
{


    int blockSize = 16, i;

    int len = strlen(input);
    if (len % blockSize == 0)
    {
        return NULL;
    }  
    // Determine padding amount by subtracting length
    // from the largest nearest blockSize multiple
    while (blockSize < len)
    {
        blockSize += 16;
    }
    int padding = blockSize - len;

    // Create buffer with proper length for padded
    // message
    unsigned char * paddedMsg = malloc(len+padding);


    // Read bytes into buffer and begin padding


    // Append bytes to end of original file
    for (i = len; i < (len+padding); ++i)
    {
        paddedMsg[i] = (unsigned char) padding;
    }


    return paddedMsg;
}









