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

#define DEBUG 1



#include <mbedtls/aes.h>




typedef struct {
	char *email;
	int uid;
	char *role; 
} Cookie;

void profile_for(char *email, char* encoded);
void arCalloc(char *ar);
void parse( Cookie *kv, char *encodedCookie );
void destroyMeta(char *email);
void getInput(char *input);
void generateKey(char *key);
void encryptEncoded(char *encoded, char *encrypted);




int main(int argc, char * argv[])
{
	// Seed random
    srand(time(NULL));
	unsigned char key[16];
	char input[MAX_LEN];
	char encodedInput[MAX_LEN];
	char encrypted[MAX_LEN];

	
	// Set up MbedTLS struct
    mbedtls_aes_context ctx;
    ctx.nr = 10;
    mbedtls_aes_init(&ctx);
	
	generateKey(key);
	
	// Returns 0 on success
	int ret = mbedtls_aes_setkey_dec(&ctx, key, 128);



	while ((true) && (!ret)) {
		arCalloc(input);
		getInput(input);
		profile_for(input, encodedInput);	
				












}





void profile_for(char *email, char* encoded)
{

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




void destroyMeta(char *email)
{
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
	for (i = 0; i < 16; ++i) {
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








}

void encryptEncoded(char *encoded, char *encrypted) {







}


