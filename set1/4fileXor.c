#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


#define DEBUG

// This code is similar to the previous code except it includes a readfile 
// function in order to get the strings from a text file
//
// The function will be called once and load each string into a multidimensional
// array passed into the function, then the strings decoded will be anaylzed
// like the last file and depending on the average scores an arbitrary score 
// limit will be set for which decoded strings to produce
//

int getStringsFromFile(char *filecontents[], FILE * fp);


int main(int argc, char * argv[])
{
	unsigned char buf1[256], outputs[8][256], decbuf[3], key = 0x1, message[256];
	long int hexDecode[256], out;
	int i,x,z,r,f,score,maxScore;
	size_t lenb1;
	char *np = NULL;
	char *filecontents[1000];
	FILE * fp;


	fp = fopen(argv[1], "r");
	getStringsFromFile(filecontents, fp);	
	fclose(fp);
	

	i = x = z = r = f = score = maxScore =0;
	decbuf[2] = '\0';




	// Read through all recorded strings in file
	while(filecontents[++f] != NULL)
	{
		//Load each individually into buf1
		strcpy(buf1, filecontents[f]);
		lenb1 = strlen(buf1);

			

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
			for (z = 0; z < (lenb1/2); ++z)
			{
				out = hexDecode[z];
				out ^= key;
			
				// String scoring for valid ASCII characters
				// Higher the score, higher likely hood of decoded message
				if ((isalpha(out)) || (isspace(out)))
				{
					score++;
				}
				message[z] = out;
			}
			
			// This line stops print outs of strings that scored lower
			// than previous strings outputted, which should ultimately put 
			// the highest scoring string at the bottom of the list
			(score > maxScore) ? (maxScore  = score) : score;
			if (score >= maxScore)
			{
			printf("Entered string to decode: %s\n", buf1);
			printf("XOR'd with %x: %s ", key,message);
			printf(" - String score: %d\n", score);
			}

		}
	}



	return 0;
}

int getStringsFromFile(char *filecontents[], FILE * fp)
{
	int i, ret;
	size_t n = 256;
	ret = i = 0;
	while (ret != -1)
	{
		ret = getline(&filecontents[i++], &n, fp);
	}	

	return 0;
}
