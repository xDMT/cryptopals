#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1000

int main(int argc, char * argv[])
{
	unsigned char instr[MAX_INPUT];
	unsigned char * tempbyte = calloc(7,sizeof(unsigned char));
	unsigned char * b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int i, x;
	long int hexRead, out;
	
	
	strcpy(instr,argv[1]);






	// Logic here is that it will read 6 unsiged chars at a time, 
	// totaling 24 bits 
	for(i = 0; i < strlen(instr); )
	{
	
		for (x = 0; (x < 6) && (i < strlen(instr)); ++i, ++x)
		{
			tempbyte[x] = instr[i];		
		}
		// Then it converts the hex characters(as an encoded string) to its actual
		// hex integer value
		hexRead = strtol(tempbyte,NULL, 16);


		// Then in order to get the corresponding base 64 character
		// it ANDS 6 bits of the 24 bit string at a time
		// and shifts them the appropriate amount to get the integer
		// value of those 6 bits
		out = hexRead & 0b111111000000000000000000;
	
		// It then stores that integer value in out and uses it as an
		// index to lookup the corresponding base64 value
		out = out >> 18;
		putchar(b64[out]);


	
	       	out = hexRead & 0b000000111111000000000000;
		out = out >> 12;
		putchar(b64[out]);

		
	       	out = hexRead & 0b000000000000111111000000;
		out = out >> 6;
		putchar(b64[out]);


	       	out = hexRead & 0b000000000000000000111111;
		putchar(b64[out]);
	}

	
	free(tempbyte);


	return 0;
}


