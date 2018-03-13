#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 50000
#define INIT_B64 1
#define DECODE_B64 0


void initB64DecodeMap(unsigned char b64[]);
void b64decrypt(unsigned char b64[], unsigned char instr[], u_int64_t b64decryptStr[]);
unsigned char getB64Val(unsigned char c, unsigned char b64[]);
void loadFile(unsigned char instr[], unsigned char *filename);






int main(int argc, char * argv[])
{
	unsigned char b64[64], instr[MAX_STR_LEN] , filename[MAX_STR_LEN];
	int i, x, z, nll = 0;
	strcpy(filename, argv[1]);
	u_int64_t b64decryptStr[MAX_STR_LEN];

	// Loads the decoding index for base64 to hex
	initB64DecodeMap(b64);

	// Loads the contents of the file to be decrypted into instr
	loadFile(instr, filename);

	// Decrypts the file
	b64decrypt(b64, instr, b64decryptStr);	


	size_t len  = (sizeof(b64decryptStr)/sizeof(u_int64_t));


	for (i = 0; i < len; ++i)
	{
		printf("%x ", b64decryptStr[i]);
//#ifdef DEBUG
		if (i % 20 == 0)
		{
			printf("\n");
		}
	
//#endif
		if (b64decryptStr[i] != 0)
		{
			nll = 0;
		}
		if (b64decryptStr[i] == 0)
		{
			nll++;
			if (nll > 20)
			{
				break	;
			}
		}
	}
	



	return 0;
}



void b64decrypt(unsigned char b64[], unsigned char instr[], u_int64_t b64decryptStr[])
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
		for(z = 0; z < 8; ++z, ++i)
		{
			tempbyte[z] = instr[i];
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

}

void initB64DecodeMap(unsigned char b64[])
{
	
	int i;
	unsigned char key = 'A';

		

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
	return;
}











void loadFile(unsigned char instr[], unsigned char *filename)
{
	FILE *fp;
	unsigned char c;
	int i = 0;

	fp = fopen(filename, "r");
	do 
	{
		c = fgetc(fp);
		if (c != '\n')
		{
			instr[i++] = c;
		}
	} while ((c != EOF) && (i < MAX_STR_LEN));
	return;


}
