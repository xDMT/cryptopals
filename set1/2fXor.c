#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEBUG



int main(int argc, char * argv[])
{
	unsigned char buf1[256], buf2[256];
	unsigned char * b1 = calloc(2,sizeof(unsigned char));
	unsigned char * b2 = calloc(2,sizeof(unsigned char));
	long int out, xor;
	int i,x;
	size_t lenb1, lenb2, len;




	// This just makes it faster to run for the challenge
#ifndef DEBUG
	strcpy(buf1,argv[1]);
	printf("Enter string to XOR against: ");
	scanf("%s", buf2);

#else
	strcpy(buf1, "1c0111001f010100061a024b53535009181c");
	strcpy(buf2, "686974207468652062756c6c277320657965");
#endif

	// Preliminary stuff
	lenb1 = strlen(buf1);
	lenb2 = strlen(buf2);
	(lenb1 > lenb2) ? (len = lenb1) : (len = lenb2);



	// Actual XORing happens here.
	for (i = 0; i < len; ++i)
	{
		b1[0] = buf1[i];
		b2[0] = buf2[i];
		
		// Get each character into its hexadecimal equivalent
		out = strtol(b1, NULL, 16);
		xor = strtol(b2, NULL, 16);	

		// Then XOR the two strings and print the output
		out = xor ^ out;
		printf("%x", out);
	}



	free(b1);
	free(b2);

	return 0;
}


