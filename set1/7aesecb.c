#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <openssl/aes.h>



typedef unsigned char * bytePtr;
typedef unsigned char  byte;



size_t b64decrypt(bytePtr b64, bytePtr instr, bytePtr b64decryptStr, size_t len);

int main(int argc, char * argv[])
{
	FILE * fp = fopen(argv[1], "r");
	size_t len;
	// File reading sequence
	if(!fp){
		exit(1);
	}
	// File length sequence
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	rewind(fp);
	
	
	// Start file read
	bytePtr inBlock = calloc(len, sizeof(byte));
	if(fread(inBlock, sizeof(char), len, fp) != len){
		exit(1);
	}
	fclose(fp);
	// End file read
	//



		
	bytePtr outBlock = calloc(len, sizeof(byte));
	AES_decrypt(inBlock, outBlock, "YELLOW SUBMARINE");

	printf("%s", outBlock);	



	










	return 0;
}



size_t b64decrypt(bytePtr b64, bytePtr instr, bytePtr b64decryptStr, size_t len)
{

	int i, x, z, f;
	bytePtr tempbyte = calloc(9, sizeof(byte));
	bytePtr pch;
	u_int64_t b64Read, out, hexOut;
	



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
			pch = strchr(b64, tempbyte[z]);
			out = pch-b64;	
			b64Read += out;
			if (z < 7)
			{
				b64Read = b64Read << x;
			}
		}
		
			hexOut = (b64Read & 0b0000000000000000111111110000000000000000000000000000000000000000);
			hexOut = hexOut >> 40;
			b64decryptStr[f++] = hexOut;
		

			hexOut = (b64Read & 0b0000000000000000000000001111111100000000000000000000000000000000);
			hexOut = hexOut >> 32;
			b64decryptStr[f++] = hexOut;


			hexOut = (b64Read & 0b0000000000000000000000000000000011111111000000000000000000000000);
			hexOut = hexOut >> 24;
			b64decryptStr[f++] = hexOut;

			hexOut = (b64Read & 0b0000000000000000000000000000000000000000111111110000000000000000);
			hexOut = hexOut >> 16;
			b64decryptStr[f++] = hexOut;

			hexOut = (b64Read & 0b0000000000000000000000000000000000000000000000001111111100000000);
			hexOut = hexOut >> 8;
			b64decryptStr[f++] = hexOut;


			hexOut = (b64Read & 0b0000000000000000000000000000000000000000000000000000000011111111);
			b64decryptStr[f++] = hexOut;

	}
	return f;
}
