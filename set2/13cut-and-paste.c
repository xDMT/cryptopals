#include <stdio.h>
#include <string.h>

#define MAX_LEN 100
#define FIELDS 3
#define true 1
#define false 0

void arCalloc(char *ar);
void parse( Cookie *kv, char *encodedCookie );


typedef struct {
	char *email;
	int uid;
	char *role; 
} Cookie;



// Encoded parser
void parse( Cookie *kv, char *encodedCookie ) {

	int r,i,x,e,u, hitEq, hitAnd;
	i = x = r = e = u hitAnd = hitEq = 0;


	// Zero out garbage
	char email[MAX_LEN];
	char uid[MAX_LEN];
	char role[MAX_LEN];
	arCalloc(email);
	arCalloc(role);


	for (x = 0 ; x < FIELDS;) {
		for (i = 0; i < strlen(encodeCookie) ; ++i) {
			
			if (encodedCookie[i] == '&') {
				hitAnd = true;
				x++;
			}
			if (encodedCookie[i] == '=') {
				hitAnd = false;
				hitEq = true;
			}

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


// Calloc for stack array
void arCalloc(char *ar) {
	int i;
	for (i = 0; i < MAX_LEN; ++i_ ){
		ar[i] = '\0';
	}

}




int main() {






}



