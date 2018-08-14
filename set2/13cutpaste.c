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

void profile_for(char *email);
void arCalloc(char *ar);
void parse( Cookie *kv, char *encodedCookie );



int main(int argc, char * argv[])
{

    if (DEBUG) {
        
        
        Cookie *foo = malloc(MALLOC_BUF);
        char *encoded = "email=foo@bar.com&uid=10&role=user";
        
        int uid;

        parse(foo, encoded);
        uid = foo->uid;
        
        printf("%x\n", foo->email);

        free(foo);
    }
    
















}





void profile_for(char *email)
{





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

    // Get vals for 3 fields
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
