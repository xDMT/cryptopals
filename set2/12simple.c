#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>


#include <mbedtls/aes.h>
#define BUF_LEN 10000 





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
    unsigned char * initialBufb64 = malloc(BUF_LEN);
    i = b64decrypt(b64, strAppend, initialBufb64);


    // Load base64 decoded string into strAppendDecode
    unsigned char * strAppendDecoded = calloc(i+1, sizeof(unsigned char));
    memcpy(strAppendDecoded, initialBufb64, i);
    free(initialBufb64);
    strAppendDecoded[i] = '\0';

    // Malloc a ton of bytes for initialize buffer, get length, malloc
    // new buffer, memcpy by len then delete other buffer
    unsigned char reuse[BUF_LEN];
    int reuselen, loopcount = 0;
    while (1)
    {
        // Prompt for input
        unsigned char initialBuf[BUF_LEN];
        printf(">>");
        
        i = 0;
        while ((c = getchar()) != '\n') 
        {
            initialBuf[i++] = c;
        }
        len = i;
    

        // Clear and exit
        if ((initialBuf[0] == 'c') && (initialBuf[1] == 'l'))
        {
            system("clear");
            continue;
        }
        else if ((initialBuf[0] == 'q') && (initialBuf[1] == 'q'))
        {
            exit(0);
        }
        // On blank entry, reuse last input, except increment to record for blocks
        if (i == 0)
        {
            if ( loopcount != 0)
            {
                memcpy(initialBuf, reuse, reuselen);
                len = reuselen;
            }
        }
        else
        {
            memset(reuse, '\0', BUF_LEN);
            memcpy(reuse, initialBuf, i);
            reuselen = i;
        }
        ++loopcount;


        // New buffer for size 
        i = strlen(strAppendDecoded); 
        unsigned char inBlock[i+len+1];

        memcpy(inBlock, initialBuf, len);
        memcpy(inBlock+len, strAppendDecoded, i);
        
        /////////////////////////////
        //pad here

        if (len > 16)
        {

            int blockSize = 16, i;

            int len = strlen(initialBuf);
            if (len % blockSize == 0)
            {
                break;
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

            memcpy(paddedMsg, initialBuf, len);
            // Append bytes to end of original file
            for (i = len; i < (len+padding); ++i)
            {
                paddedMsg[i] = (unsigned char) padding;
            }
            memcpy(initialBuf, paddedMsg, len+padding);
            free(paddedMsg);
        }









        len += i+16;
        unsigned char  outBlock[len];






        // Define context structure for AES context 
        // Initialize AES context specifying rounds,
        // rounds for 128 bit keys is 10
        
        mbedtls_aes_context ctx;
        ctx.nr = 10;
        mbedtls_aes_init(&ctx);



        // Function to set th AES key, uses the context object
        // and takes the key and the number of bits in the key as the
        // second and third arguments
        ret = mbedtls_aes_setkey_dec(&ctx, key, 128);
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


        int z, r = 0;
        for (i = 0; r < len; ++i)
        {
            printf("Block %d:", i);
            for (z = 0; z < 16; ++z,++r)
            {
            printf("%x", outBlock[r]);    
            }
            printf("\n");
        }


    } 






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


















