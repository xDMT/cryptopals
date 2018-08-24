#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include <assert.h>



#define MALLOC_BUF 4096
#define MAX_LEN 100
#define FIELDS 3
#define true 1
#define false 0
#define BLOCKSIZE 16
#define KEYSIZE 16


#define ENCODE 0 
#define DECODE 1


#define DEBUG 0



#include <mbedtls/aes.h>



// Structured cookie
typedef struct {
	char *email;
	int uid;
	char *role; 
} Cookie;


// Parsing functions	
void profile_for(char *email, char* encoded);
void parse( Cookie *kv, char *encodedCookie );
void destroyMeta(char *email);

// Encryption functions
unsigned char * generateKey();
void encryptEncoded(char *encoded, char *encrypted, const unsigned char *key);
int padBlock(char *input);
void decryptAndParse(unsigned char *encrypted, unsigned char *encoded, const unsigned char *key, int len);
void stripPadding(char *encoded);

// Utility functions
void arCalloc(char *ar);
int getInput(char *input, int mode);
void printEncArr(unsigned char *ar);
void convertHex(unsigned char *hex);
mbedtls_aes_context *aesInit();





int main(int argc, char * argv[]) {


	// Seed random
    srand(time(NULL));
	const unsigned char *key;
	int len;
	unsigned char input[MAX_LEN], output[MAX_LEN], encoded[MAX_LEN], encrypted[MAX_LEN];
	




	
	if (DEBUG) {
		key = "YELLOW SUBMARINE";
	}
	else {
		key =	generateKey();
	}
	
	// Returns 0 on success



	while (true) {
		
		// Clear and get input
		arCalloc(input);
		getInput(input, ENCODE);

		// Encode email into profile
		profile_for(input, encoded);

		// Pad and encrypt
		len = padBlock(encoded);
		encryptEncoded(encoded, encrypted, key);

		// Print ciphertext 
		printEncArr((unsigned char *) encrypted);

		if (DEBUG) {
			decryptAndParse(encrypted,output, key, len);
		}
		else {
			arCalloc(input);
			len = getInput(input, DECODE);
			decryptAndParse(input,output, key, len);
		}


	}
	

	/* Test out implementing signal handler to free
	 * malloc'd memory on SIGINT
	 */




}





void profile_for(char *email, char* encoded) {

    destroyMeta(email);

	int uid;
    // Get random UID
	if (DEBUG) {
		uid = 1;
	}
	else {
		srand(time(NULL));
		uid = rand() % 255;
	}
    
    // Define role and UID buffer
    char *role = "user";
    char uidStr[4];
    
    // Ghetto itoa
    sprintf(uidStr, "%d", uid);


    
    // Temp buffer to hold encoded profile
    char tmp[MAX_LEN];
    arCalloc(tmp);
	arCalloc(encoded);


    // Begin encoding
    strcat(tmp, "email=");
    strcat(tmp, email);
    strcat(tmp, "&uid=");
    strcat(tmp, uidStr);
    strcat(tmp, "&role=");
    strcat(tmp, role);

    strcpy(encoded,tmp);
    return;


}




void destroyMeta(char *email) {
    int i,x;
    char tmp[MAX_LEN];
    arCalloc(tmp);
    
    // Iterate through email contents, do not copy meta
    for (i = x = 0; i < strlen(email) ; ++i) {
        if (email[i] == '&' || email[i] == '=') {
            continue;
        }
        tmp[x++] = email[i];
    }

    // Clear original email
    arCalloc(email);

    // Re-load with meta-escaped
    strcpy(email, tmp);
    return;


}



// Encoded parser
void parse( Cookie *kv, char *encodedCookie ) {

	int r,i,x,e,u, hitEq, hitAnd;
	i = x = r = e = u = hitAnd = hitEq = 0;


	// Zero out garbage
	char email[MAX_LEN];
	char uid[MAX_LEN];
	char role[MAX_LEN];
	arCalloc(email);
	arCalloc(role);
	arCalloc(uid);

    // Get vals for 3 fields of foo
	for (x = 0 ; x < FIELDS && i < strlen(encodedCookie) ;) {
		for (; i < strlen(encodedCookie) ; ++i) {
			

            // When & is found, stop getting value for field
            // and jump to next field
			if (encodedCookie[i] == '&') {
				hitAnd = true;
				x++;
			}
			if (encodedCookie[i] == '=') {
				hitAnd = false;
				hitEq = true;
                ++i;
			}
            
            // When = is found, start getting value for field
			if (hitEq && !hitAnd) {
				switch (x) {
					case 0:
						email[e++] = encodedCookie[i];
						break;
					case 1:
						uid[u++] = encodedCookie[i];
						break;
					case 2:
						role[r++] = encodedCookie[i];
						break;
				}
            }
					
		
		}
    }
    int uidInt = atoi(uid);

    kv->email = email;
    kv->uid = uidInt;
    kv->role = role;

    return;

}


// Calloc for stack array
void arCalloc(char *ar) {
	int i;
	for (i = 0; i < MAX_LEN; ++i ){
		ar[i] = '\0';
	}

}



unsigned char * generateKey() {


	unsigned char *key = (unsigned char *) malloc(sizeof(unsigned char) * 17);
	int i;
	for (i = 0; i < KEYSIZE; ++i) {
		key[i] = (rand() % 255);
	}
	key[16] = '\0';
	return key;
}



int getInput(char *input, int mode) {
	char c;
	int i = 0;

	if (mode == ENCODE) {
		printf("Enter email: ");
	}
	else
	{
		printf("Decode email: ");
	}
	while ((c = getchar()) && (c != '\n')) {
		
		if (c != '\b') {
			input[i++] = c;
		}
		else if (i > 0) {
			i -= 1;
		}
	}

	return i ;
}









void encryptEncoded(char *encoded, char *encrypted, const unsigned char *key) {


	// Initialize context and set key
	mbedtls_aes_context * enc_context = aesInit();
	int ret = mbedtls_aes_setkey_enc(enc_context, key, 128);
	assert(ret == 0);


	int i, len = strlen(encoded);
	char blockIn[BLOCKSIZE], blockOut[BLOCKSIZE];

	// Clear encrypted
	arCalloc(encrypted);


	// Make sure input is blocksize multiple
	((len % BLOCKSIZE) != 0) ? exit(1) : len;
	
	// Encrypt block by block
	for (i = 0; i < len; i += 16) {
		memcpy(blockIn, encoded+i, BLOCKSIZE);
        mbedtls_aes_crypt_ecb(enc_context, MBEDTLS_AES_ENCRYPT, blockIn, blockOut);
		memcpy(encrypted+i, blockOut, BLOCKSIZE);
	}

	free(enc_context);
	return;

}




int padBlock(char *input) {


    int padding, i, len, blockMod;
	len = strlen(input);
	blockMod = len % BLOCKSIZE;
	

	// Subtract modulus from block size to get padding
    if (blockMod) {
		padding = BLOCKSIZE - blockMod;
    }
	// No padding required, return
	else {
		return len;
	}



    // Append bytes to end of original string
    for (i = len; i < (len+padding); ++i)
    {
		input[i] = (unsigned char) padding;
    }

    return len+padding ;
}


// Print encrypted string in hex
void printEncArr(unsigned char *ar) {
	int i,x,  len = strlen(ar);
    printf("Cipher-text: ");
    for (x=0; x < len;) {
        for (i=0; i < BLOCKSIZE; ++i) {
            printf("%.2x", ar[x++]);
        }
        printf(" ");
    }
    printf("\n\n");

	return;
}




void decryptAndParse(unsigned char *encrypted, unsigned char *encoded, const unsigned char *key, int len) {


	// Initialize context and set key
	mbedtls_aes_context * dec_context = aesInit();
	int ret = mbedtls_aes_setkey_dec(dec_context, key, 128);
	assert(ret == 0);


	int i;
	unsigned char blockIn[BLOCKSIZE], blockOut[BLOCKSIZE];
	arCalloc(encoded);

    
    len = strlen(encrypted)/2;
	(DEBUG == false) ? convertHex(encrypted) : i;
    
    if (DEBUG) {
        printf("Confirm encrypted: ");
        for (i=0; i < len+1; ++i) {
            printf("0x%.2x ", encrypted[i]);
        }	
    }
	
	// Decrypt input
	for (i = 0; i < len; i += 16) {
		memcpy(blockIn, encrypted+i, BLOCKSIZE);
        mbedtls_aes_crypt_ecb(dec_context, MBEDTLS_AES_DECRYPT, blockIn, blockOut);
		memcpy(encoded+i, blockOut, BLOCKSIZE);
	}

	stripPadding(encoded);

	printf("\n\nDecoded: ");
	for (i=0; i < len+1; ++i) {
		printf("%c", encoded[i]);
	}
	printf("\n");
	free(dec_context);

	return;

}



void stripPadding(char *encoded) {

	int len = strlen(encoded);
	int x, i = len-1;
	unsigned char a,b;


	// If last byte is 0x1, or last two byte are same, assum
	// padding and strip 
	if ((encoded[i] == encoded[i-1]) || (encoded[i] == 0x1) ) {
		a = encoded[i];

		// Traverse backwards in string, erasing padding
		// for as many bytes as padding value
		for (i=0, x=len-1; i < a; i++, x--) {
			encoded[x] = '\0';
		}
	}
	return;
}



void convertHex(unsigned char *hex) {

	int i,x,len = strlen(hex);
	unsigned char inHex[3], tmp[MAX_LEN], outChar;
    inHex[2] = '\0';

	// Clear temp array
	arCalloc(tmp);


	// Grab hex string and convert to actual hex value
	for (i=x=0; i < len; i+=2, ++x) {
		memcpy(inHex, hex+i, 2);
		outChar = strtol(inHex, NULL, 16);

		// Store in temp
		tmp[x] = outChar;
	}
	arCalloc(hex);
	strcpy(hex,tmp);
	
	return;

}


mbedtls_aes_context *aesInit() {
	
	// Set up MbedTLS struct
    mbedtls_aes_context *ctx = (mbedtls_aes_context *) malloc(sizeof(mbedtls_aes_context));
    ctx->nr = 10;
    mbedtls_aes_init(ctx);

	// Return pointer to context
	return ctx;

}




