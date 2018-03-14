#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 50000
#define INIT_B64 1
#define DECODE_B64 0
#define PRINT_MAP

void initB64DecodeMap(unsigned char b64[], int guessmap[]);
int  b64decrypt(unsigned char b64[], unsigned char * instr , u_int64_t b64decryptStr[]);
unsigned char getB64Val(unsigned char c, unsigned char b64[]);
unsigned char * loadFile(unsigned char * instr, unsigned char *filename);

int hammingDistance(unsigned char *val1, unsigned char *val2);
int guessKeysize(u_int64_t *str, int guessmap[]);
void findKey(u_int64_t *str, int keySize, int len);

int main(int argc, char * argv[])
{
	unsigned char b64[64], filename[MAX_STR_LEN];
	unsigned char * instr = malloc(sizeof(char)*10000);
	u_int64_t * b64decryptStr = malloc(sizeof(u_int64_t)*10000);	
	int len, hd, x, z, nll = 0, guessmap[40];


	if (argv[1] == NULL)
	{
		printf("\nInvalid argument\n");
		exit(0);
	}

	strcpy(filename, argv[1]);
	initB64DecodeMap(b64, guessmap);
	instr = loadFile(instr, filename);
	len = b64decrypt(b64, instr, b64decryptStr);	

	hd = guessKeysize(b64decryptStr,guessmap);
	
	findKey(b64decryptStr, hd, len);

#ifdef PRINT_HEX
	for (x = 0; x < len;)
	{
		for (z = 0; z < 45; ++z, ++x)
		{
			printf("%02x ", b64decryptStr[x]);
		}
		printf("\n");
	}
#endif

#ifdef PRINT_MAP
	float normalized;	
	for (x = 0; x < 40; ++x)
	{
		if (x != 0)
		{
			normalized = guessmap[x] / x;
		}
		printf("Guess size:%d - Edit distance: %f\n", x, normalized);
	}
#endif
	free(instr);
	free(b64decryptStr);

	return 0;
}













 int b64decrypt(unsigned char b64[], unsigned char *instr, u_int64_t b64decryptStr[])
{

	int i, x, z, f;
	unsigned char tempbyte[9];
	tempbyte[8] = '\0';
	void * np  = NULL;
	u_int64_t b64Read, out, hexOut;
	size_t len = strlen(instr);
		



	// Logic here essentially takes 8 base 64 characters at a time, totaling to 48 bits
	// Those characters are loaded into the tempbyte[] array and each characters
	// equivalent hex value is extracted using getB64Val
	//
	// Then that value is shifted the appropriate amount of bits to the left
	// in order to set up the 48 bit hex string, and added to b64Read 
	//
	// Once this is done for all characters in the tempbyte array, b64Read should contain
	// 48 bits, and it is anded 8 bits at a time to extract the 5 hex characters
	// and that value that is extracted is loaded into b64decrypStr
	//
	// When this function finishes, b64decryptStr should contain an array of hex characters
	// of the decrypted base 64 message
	for (i = 0, f = 0; i < len; )
	{
		b64Read = 0;
		for(z = 0; z < 8; ++i)
		{
			if (instr[i] != '\n')
			{
				tempbyte[z++] = instr[i];
			}
		}
		for (z = 0, x = 6; z < 8; ++z)
		{
			out = getB64Val(tempbyte[z], b64);
			b64Read += out;
			if (z < 7)
			{
				b64Read = b64Read << x;
			}
		}
		
			hexOut = (b64Read & 0b0000000000000000111111110000000000000000000000000000000000000000);
			hexOut = hexOut >> 40;
			b64decryptStr[f++] = hexOut;
		
#ifdef DEBUG
			printf("Storing number %d - Hex value: %x\n", hexOut, hexOut);
#endif
			hexOut = (b64Read & 0b0000000000000000000000001111111100000000000000000000000000000000);
			hexOut = hexOut >> 32;
			b64decryptStr[f++] = hexOut;
#ifdef DEBUG
			printf("Storing number %d - Hex value: %x\n", hexOut, hexOut);
#endif

			hexOut = (b64Read & 0b0000000000000000000000000000000011111111000000000000000000000000);
			hexOut = hexOut >> 24;
			b64decryptStr[f++] = hexOut;
#ifdef DEBUG
			printf("Storing number %d - Hex value: %x\n", hexOut, hexOut);
#endif

			hexOut = (b64Read & 0b0000000000000000000000000000000000000000111111110000000000000000);
			hexOut = hexOut >> 16;
			b64decryptStr[f++] = hexOut;
#ifdef DEBUG
			printf("Storing number %d - Hex value: %x\n", hexOut, hexOut);
#endif

			hexOut = (b64Read & 0b0000000000000000000000000000000000000000000000001111111100000000);
			hexOut = hexOut >> 8;
			b64decryptStr[f++] = hexOut;
#ifdef DEBUG
			printf("Storing number %d - Hex value: %x\n", hexOut, hexOut);
#endif

			hexOut = (b64Read & 0b0000000000000000000000000000000000000000000000000000000011111111);
			b64decryptStr[f++] = hexOut;
#ifdef DEBUG
			printf("Storing number %d - Hex value: %x\n", hexOut, hexOut);
#endif

	}
	return f;
}

void initB64DecodeMap(unsigned char b64[], int guessmap[])
{
	
	int i;
	unsigned char key = 'A';

		for (i = 0; i < 40; ++i)
		{
			guessmap[i] = 0;
		}	

		// Initialize b64 key map	
		for (i =0; i < 26; ++i)
		{
			b64[i] = key++;
		}
		key = 'a';
		for (; i < 52; ++i)
		{
			b64[i] = key++;
		}
		key = '0';
		for (; i < 62; ++i)
		{
			b64[i] = key++;
		}
		b64[62] = '+';
		b64[63] = '/';
		

	return;
}

unsigned char getB64Val(unsigned char c, unsigned char b64[])
{
	// This function increments i untill it finds the matching b64 character
	// and returns the value of i
	unsigned char i = 0;
	for (; i < 63 ; ++i)
	{
		
		if (b64[i] == c)
		{
			return i;
		}
	}
	return 0;
}

unsigned char * loadFile(unsigned char * instr, unsigned char *filename)
{
	FILE *fp;
	long len;
	size_t result;

	fp = fopen(filename, "r");
	fp == NULL ? exit(1) : fp;
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	rewind(fp);

	result = fread(instr, sizeof(char), len, fp);
	if (result != len)
	{
		printf("\nRead Error\n");
		exit(0);
	}
	fclose(fp);




	return instr;


}

int hammingDistance(unsigned char *val1, unsigned char *val2)
{
	int len, i,x, hammingD = 0;
	len = strlen(val1);
	(strlen(val2) > len) ? (len = strlen(val2)) : len;
	unsigned char *result = malloc(sizeof(unsigned char)*len);
	unsigned char m2;


	// Xors each character of two strings and stores the result in result
	for (i = 0; i < len; ++i)
	{
		result[i] = val1[i] ^ val2[i];
	}

	// Iterates through result determining the differing bits 
	// by anding each character of result with 1, 
	// and adding a count when the result of the AND is 1
	// indicating a differing bit
	for (i = 0 ; i < (len); ++i)
	{
		for (x = 0; x < 8; ++x)
		{
			m2 = result[i] & 0x1;
			( m2 == 0x1 ) ? hammingD++ : (m2 = 0x0);
			result[i] = result[i] >> 1;
		}
	}
	free(result);
	return hammingD;
}	

int guessKeysize(u_int64_t *str, int guessmap[])
{
	int guess, lowGuess, hd, i;
	int lowest = 0x7FFFFFFF;
	unsigned char *val1, *val2;

	val1 = malloc(sizeof(unsigned char)*1);
	val2 = malloc(sizeof(unsigned char)*1);


	// Tries a series of key size guesses
	for (guess = 2; guess < 40; ++guess)
	{
		val1 = realloc(val1,sizeof(unsigned char)*guess);
		val2 = realloc(val2,sizeof(unsigned char)*guess);

		// Copys two strings that are guess characters away from
		// eachother and sends them to the hammingDistance function
		
		for (i = 0; i < guess; ++i)
		{
			val1[i] = str[i];
		       	val2[i] = str[i+guess];
		}		
		hd = hammingDistance(val1,val2);
		guessmap[guess] = hd;


		// The lowest hamming distance will be recorded
		// and the keysize that produced the lowest hammingDistance
		// is also recorded
		if (hd < lowest)
		{
			lowest = hd;
			lowGuess = guess;
		}
	}
	free(val1);
	free(val2);

	// The keysize that produced the lowest hamming distance is returned for a following function
	// to single Xor decrypt the hex
	return lowGuess;
}


void findKey(u_int64_t *str, int keySize, int len)
{
	unsigned char blockCipher[len], key = 0x1, outchar, messageTemp[len], message[len];
	int i,x,z, f, score = 0, lowestScore = 0x80000000, threshold, block;
	i = x = z = f = 0;
	unsigned char keyString[5];
#ifdef PRINT_SCORE
	printf("Enter key threshold: ");
	scanf(" %d", &threshold); 
	printf("Enter block to analyze: ");
	scanf(" %d", &block);
#endif
	// Sort into blocks of keysize
	for (i = 0; i < len; ++z)
	{
		for (x = 0; x < (len/keySize); ++x, ++i)
		{
			blockCipher[i] = str[(x*keySize)+z];
		}
	}

	

	// Make single xor decrypt function here
	for (f = 0; f < 5; ++f)
	{
		for (key = 0x1; key < 0xFF; key++)
		{	
			for(i = ((len/5)*f); i < ((len/5)*(f+1)); ++i)
			{
				outchar = key ^ blockCipher[i];
				( (outchar > 0x41) && (outchar < 0x7A) ) ? score++ : score--;
				messageTemp[i] = outchar;
			}
			if (score > lowestScore)
			{
				lowestScore = score;
				keyString[f] = key;
				for (i = ((len/5)*f); i < ((len/5)*(f+1)); ++i)
				{
					message[i] = messageTemp[i];
				}
			}
#ifdef PRINT_SCORE 
		if ((f+1) == block)
		{	
			if(score > threshold)
			{
				printf("Block %d: Key tried: 0x%02x Score: %d\n", (f+1), key, score);
			}
		}
#endif
			score = 0;
		}
	}
#ifdef PRINT_MSG
	for (i = 0; i < len; ++i)
	{
		for (z = 0; z < 45; ++z)
		{
			printf("%c", message[i]);
		}
		printf("\n");
	}
#endif


#ifdef PRINT_KEY
	for (i = 0; i < 5; ++i)
	{
		printf("%c ", keyString[i]);
	}
#endif




#ifdef PRINT_HEX_DECRYPT
	for (x = 0; x < len;)
	{
			for (z = 0; z < 45; ++z, ++x)
			{
				if (x % (len/5) == 0)
				{
					printf("\n\n\n\n\nBlock %d\n", ((f++) +1));
				}
				printf("%02x ", blockCipher[x]);
			}
			printf("\n");
	}
#endif
		
	return;


}





