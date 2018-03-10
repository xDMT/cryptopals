#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEBUG

// This code runs 255 possible XOR characters against the given string
// It scores the likelyhood of the correct message by determining how many
// characters are readable ASCII characters and prints those
// strings along with the character they were XOR'd against


int main(int argc, char * argv[])
{
	unsigned char buf1[256], outputs[8][256], decbuf[3], key = 0x1, message[256];
	long int hexDecode[256], out;
	int i,x,z,r,score, hasPrintable;
	size_t lenb1;
	char * np = NULL;
	
	i = x = z = r = score =0;
	decbuf[2] = '\0';


#ifndef DEBUG
	strcpy(buf1,argv[1]);
#else
	strcpy(buf1, "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
#endif

	lenb1 = strlen(buf1);

	printf("Entered string to decode: %s\n\n", buf1);
		

	// Print out 8 possible xor'd strings 
	for (r = 0, score = 0; r < 255; ++r, ++key)
	{	
		// First decode into hex and store in integer array
		for (i = 0, z = 0; i < lenb1; ++z)
		{
			// Load word into buffer
			for (x = 0; x < 2; ++x, ++i)
			{
				decbuf[x] = buf1[i];
			}
			// Get each character into its hexadecimal equivalent
			hexDecode[z]  = strtol(decbuf, &np, 16);
		}

		// Now hexDecode should be loaded with the hex representation
		// of the entered string, and we just have to xor through the
		// array on an incremented xor key through 8 possible values
		// maybe more if I have mistaken character by a byte instead of 4 bits
		score = 0;
		hasPrintable = 1;
		for (z = 0; z < (lenb1/2); ++z)
		{
			out = hexDecode[z];
			out ^= key;	
			( (out > 0x41) && (out < 0x7A) ) ? score++ : score--;
			message[z] = out;
		}

		if (score >= 20)
		{
		printf("XOR'd with %x: %s ", key,message);
		printf(" - String score: %d\n", score);
		}

	}




	return 0;
}


