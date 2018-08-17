#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>

#define MALLOC_BUF 4096
#define MAX_LEN 100
#define FIELDS 3
#define true 1
#define false 0
#define BLOCKSIZE 16
#define KEYSIZE 16

#define DEBUG 1



#include <mbedtls/aes.h>



// Structured cookie
typedef struct {
	char *email;
	int uid;
	char *role; 
} Cookie;


// Core functions	
void profile_for(char *email, char* encoded);
void parse( Cookie *kv, char *encodedCookie );
void destroyMeta(char *email);
void generateKey(char *key);
void encryptEncoded(char *encoded, char *encrypted, mbedtls_aes_context *ctx);
void padBlock(char *input);

// Utility functions
void arCalloc(char *ar);
void getInput(char *input);
void printEncArr(char *ar);





int main(int argc, char * argv[]) {


	// Seed random
    srand(time(NULL));
	unsigned char key[16];
	char input[MAX_LEN], output[MAX_LEN];
	char encoded[MAX_LEN], encrypted[MAX_LEN];
	




	// Set up MbedTLS struct
    mbedtls_aes_context ctx;
    ctx.nr = 10;
    mbedtls_aes_init(&ctx);
	
	generateKey(key);
	
	// Returns 0 on success
	int ret = mbedtls_aes_setkey_dec(&ctx, key, 128);



	while ((true) && (!ret)) {
		
		// Clear and get input
		arCalloc(input);
		getInput(input);

		// Encode email into profile
		profile_for(input, encoded);

		// Pad and encrypt
		padBlock(encoded);
		encryptEncoded(encoded, encrypted, &ctx);

		// Print ciphertext 
		printEncArr(encrypted);
	}


}





void profile_for(char *email, char* encoded) {

    destroyMeta(email);


    // Get random UID
    srand(time(NULL));
    int uid = rand() % 255;
    
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



void generateKey(char *key) {
	
	int i;
	for (i = 0; i < KEYSIZE; ++i) {
		key[i] = (rand() % 255);
	}
	return;
}



void getInput(char *input) {
	char c;
	int i = 0;
	
	printf("Enter email: ");
	while ((c = getchar()) && (c != '\n')) {
		
		if (c != '\b') {
			input[++i] = c;
		}
		else if (i > 0) {
			i -= 1;
		}
	}
	return;
}









void encryptEncoded(char *encoded, char *encrypted, mbedtls_aes_context *ctx) {

	int i, len = strlen(encoded);
	char blockIn[MAX_LEN], blockOut[MAX_LEN];
	
	// Make sure input is blocksize multiple
	!(len % BLOCKSIZE) ? exit(1) : len;

	// Encrypt block by block
	for (i = 0; i < len; i += 16) {
		memcpy(blockIn, encoded+i, BLOCKSIZE);
        mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, blockIn, blockOut);
		memcpy(blockOut, encrypted+i, BLOCKSIZE);
	}
	return;

}




void padBlock(char *input) {


    int padding, i, len, blockMod;
	len = strlen(input);
	blockMod = len % BLOCKSIZE;
	

	// Subtract modulus from block size to get padding
    if (blockMod) {
		padding = BLOCKSIZE - blockMod;
    }
	// No padding required, return
	else {
		return;
	}



    // Append bytes to end of original string
    for (i = len; i < (len+padding); ++i)
    {
		input[i] = (unsigned char) padding;
    }

    return ;
}


// Print encrypted string in hex
void printEncArr(char *ar) {
	int i,  len = strlen(ar);
	for (i=0; i < len; ++i) {
		printf("%x", ar[i]);
	}
	printf("\n");

	return;
}

