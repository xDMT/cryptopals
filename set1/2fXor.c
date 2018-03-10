#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEBUG

// Just for the record, had a HUGE problem with strtol() due to 
// the array being read lacking a null terminator.
//
// Everytime I thought i would be converting the 1 character that could possibly
// be in an  array of 1 element, it would overflow everytime, producing -1
//
// I couldnt fucking figure out why until I realized it. What still trips me out though
// is I am pretty fucking sure my other file does not contain a terminator for the 
// string to be read by strtol() but works fine....
//
// Either way its working now..



int main(int argc, char * argv[])
{
	unsigned char buf1[256], buf2[256], b1[2], b2[2];
	long int out, xor;
	int i,x;
	char * np;
	size_t lenb1, lenb2, len;


	// Doing some debugging here
	errno = 0;


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
	b1[1] = '\0';
	b2[1] = '\0';
	(lenb1 > lenb2) ? (len = lenb1) : (len = lenb2);



	// Actual XORing happens here.
	for (i = 0; i < len; ++i)
	{
		b1[0] = buf1[i];
		b2[0] = buf2[i];
		
		// Get each character into its hexadecimal equivalent
		out = strtol(b1, &np, 16);
		xor = strtol(b2, &np, 16);	

		// Then XOR the two strings and print the output
		out = xor ^ out;
		printf("%x", out);
	}





	return 0;
}


