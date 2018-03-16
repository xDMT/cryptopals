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
	unsigned char buf1[256], decbuf[3], message[256], tmessage[256], *pos;
	unsigned char key, keyCorrect, out, *freq = "etaoin srhldcumfpgwybvkxjqzETAOINSRHLDCUMFPGWYBVKXJQZ0123456789.?!"; 
	long int hexDecode[256];
	int i,x,z,score, scoreHigh = 0x7FFFFFFF;
	size_t lenb1;
	
	i = x = z = score =0;
	decbuf[2] = '\0';

#ifndef DEBUG
	strcpy(buf1,argv[1]);
#else
	strcpy(buf1, "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
#endif
	lenb1 = strlen(buf1);


	printf("Entered string to decode: %s\n\n", buf1);

	// First decode into hex and store in integer array
	for (i = 0, z = 0; i < lenb1; ++z)
	{
		// Load word into buffer
		for (x = 0; x < 2; ++x, ++i)
		{
			decbuf[x] = buf1[i];
		}
		// Get each character into its hexadecimal equivalent
		hexDecode[z]  = strtol(decbuf, NULL, 16);
	}
	
	
	for ( key = 0; key < 255; ++key)
	{	

		for (score = 0, z = 0; z < (lenb1/2); ++z)
		{
			out = hexDecode[z] ^ key;
			pos = strchr(freq, out);
			if(pos) 
			{
				score += pos-freq;
			}
			else
			{
				score += 255;
			}
			tmessage[z] = out;
		}
		if (score < scoreHigh)
		{
			keyCorrect = key;
			scoreHigh = score;
			strcpy(message, tmessage);
		}

	}
	printf("%s - XOR'd with %x\n", message, keyCorrect);



	return 0;
}


