#include <limits.h>
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
	unsigned char buf1[256], decbuf[3], key, tmessage[256], message[256], out;
	unsigned char keyCorrect,  *pch, *freq = "etaoin srhldcumfpgwybvkxjqzETAOINSRHLDCUMFPGWYBVKXJQZ0123456789.?!"; 
	long int hexDecode[256];
	
	int i,x,z,r,f,score, len, scoreMax = INT_MAX;
	size_t lenb1;

	i = x = z = r = f = score = 0;
	decbuf[2] = '\0';

	FILE * fp = fopen(argv[1], "r");
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	rewind(fp);
	char *filecontents[len];
	len = getStringsFromFile(filecontents, fp);	
	fclose(fp);
	





	memset(message, '\0', 256);
	// Read through all recorded strings in file
	for (f = 0; f < len;)
	{
		//Load each string individually into buf1
		strcpy(buf1, filecontents[f++]);
		lenb1 = strlen(buf1);
		memset(hexDecode, '\0', 256);
		memset(tmessage, '\0', 256);

			

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
			
			// Xor all key values and score
			for (key = 0; key < 0xFF; ++key)
			{
				score = 0;
				for (r = 0; hexDecode[r] != '\0'; ++r)
				{
					out = hexDecode[r] ^ key;
					pch = strchr(freq, out);
					tmessage[r] = out;
					if (pch)
					{
						score += pch-freq;
					}
					else
					{
						score += 255;
					}
				}
				if (score < scoreMax)
				{
					scoreMax = score;
					strcpy(message,tmessage);
				}
			}
	}
	printf("%s", message);




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

	return i;
}
