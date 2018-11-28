#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>


#include <mbedtls/aes.h>
#define BUF_LEN 10000 

// Refactor me... please



size_t b64decrypt(unsigned char * instr, unsigned char * b64decryptStr);
unsigned char * generateRandomBytes(int * len);
void generateRandomKey(unsigned char key[]);
int getInput(unsigned char initialBuf[], int * i);
void padBlock(unsigned char initialBuf[], int len);
int analyzeCommands(unsigned char initialBuf[], unsigned char key[]);

int main(int argc, char * argv[])
{

    size_t len;
    int i,b,b64decodeLen,ret,prependLen, reuselen, loopcount = 0;
    char c;


    unsigned char * strAppend = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
    unsigned char key[16], reuse[BUF_LEN];
    unsigned char * strPrepend = generateRandomBytes(&prependLen);
    unsigned char * initialBufb64 = (unsigned char *) malloc(BUF_LEN);
    
    // Random key generation
    generateRandomKey(key);

    // Decode unknown string, copy to seperate buffer 'strAppendDecoded'
    b64decodeLen = b64decrypt(strAppend, initialBufb64);
    unsigned char * strAppendDecoded = (unsigned char * ) calloc(b64decodeLen+1, sizeof(unsigned char));
    memcpy(strAppendDecoded, initialBufb64, b64decodeLen);
    strAppendDecoded[b64decodeLen] = '\0';
    free(initialBufb64);


    // Malloc a ton of bytes for initialize buffer, get length, malloc
    // new buffer, memcpy by len then delete other buffer
    while (1) {
        unsigned char initialBuf[BUF_LEN];
        i = 0;

        // Prompt for input
        printf(">>");
        len = getInput(initialBuf, &i);

        // Determine special command entry on input ( clear screen, print key, exit )
        if (analyzeCommands(initialBuf, key)) {
            continue;
        }

        // On blank entry, reuse last input, except increment to record for blocks
        if (!i) {
            if (loopcount) {
                memcpy(initialBuf, reuse, reuselen);
                len = reuselen;
            }
        }
        else {
            memset(reuse, '\0', BUF_LEN);
            memcpy(reuse, initialBuf, i);
            reuselen = i;
        }
        ++loopcount;


        // New buffer for size of input text + appended string
        i = strlen(strAppendDecoded); 
        unsigned char inBlock[i+len+1];

        memcpy(inBlock, initialBuf, len);
        memcpy(inBlock+len, strAppendDecoded, i);
        
        if (len > 16) {
            padBlock(initialBuf, len);
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



size_t b64decrypt(unsigned char * instr, unsigned char * b64decryptStr)
{

	unsigned char * tempbyte = calloc(9, sizeof(unsigned char));
    unsigned char * b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned char * pch;
	
    int i, x, z, f,s,b;
	u_int64_t b64Read, out, hexOut, andr;
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

            // Refactored base64 decode
        for (s = 40, andr = 0xFF0000000000; s >= 0; s -= 8)
            {
                hexOut = (b64Read & andr);
                hexOut = hexOut >> s;
                b64decryptStr[f++] = hexOut;
                andr = andr >> 8;
            }
	}
	return f;
}

int analyzeCommands(unsigned char initialBuf[], unsigned char key[]) {

    int b;
    // Clear and exit
    if ((initialBuf[0] == 'c') && (initialBuf[1] == 'l'))
    {
        system("clear");
        return 1;
    }
    else if ((initialBuf[0] == 'q') && (initialBuf[1] == 'q'))
    {
        exit(0);
    }
    else if ((initialBuf[0] == 'r') && (initialBuf[1] == 'k'))
    {
        for (b = 0; b < 16; ++b)
        {
            printf("0x%x ", key[b]);
        }
        printf("\n");
        return 1;
    }
    return 0;
}



void generateRandomKey(unsigned char key[]) {
    int i;
    srand(time(NULL));
    for (i = 0; i < 16; ++i) {
        key[i] = (rand() % 255);
    }
}


unsigned char * generateRandomBytes(int * len) {
    
    srand(time(NULL));
    int byteCount = rand() % 256;
    int i;

    unsigned char * randomByteString = (unsigned char *) malloc(sizeof(unsigned char)* byteCount );
    for (int i=0; i < byteCount; ++i) {
        
        randomByteString[i] = (unsigned char) rand() % 256;
    }
    *len = i;
    return randomByteString;

}




void padBlock(unsigned char initialBuf[], int len) {
    int blockSize = 16, i;

    len = strlen(initialBuf);
    if (len % blockSize == 0) {
        return;
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
    return;
}


int getInput(unsigned char initialBuf[], int * i) {
    char c;
    while ((c = getchar()) != '\n') 
    {
        initialBuf[(*i)++] = c;
    }
    return *i;
}
