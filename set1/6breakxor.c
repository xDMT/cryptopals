#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>


<<<<<<< HEAD
typedef unsigned char * bytePtr;
typedef unsigned char  byte;



double guessKeysize(bytePtr outBlock);
size_t b64decrypt(bytePtr b64, bytePtr instr, bytePtr b64decryptStr, size_t len);
int hammingDistance(bytePtr val1, bytePtr val2);

int main(int argc, char * argv[])
{
	size_t len, len2;
	int z,i;
	FILE * fp = fopen(argv[1], "r");

	byte x,y,key,out;
	bytePtr b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345679+/";
	bytePtr scoreScale = "etaoinsrhldcumfpgwybvkxjqz ETAOINSRHLDCUMFPGWYBVKXJQZ.?!123456789";
	
	// Array to contain the five keysizes with the lowest hamming distance
	bytePtr pch, guessMap[5];

	u_int64_t scoreHigh = 0x7FFFFFFF, score = 0x0;




	// File reading sequence
	if(!fp){
		exit(1);
	}


	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	rewind(fp);
	
	
	
	bytePtr inBlock = calloc(len, sizeof(byte));
	if(fread(inBlock, sizeof(char), len, fp) != len){
		exit(1);
	}
	fclose(fp);
	// End file read	



	// Allocate space for decoded base64, for every 3 hex characters, 4 base64
	// characters result, len2 stores exact length of outBlock
	bytePtr outBlock = calloc((len*3)/4, sizeof(byte));
	len2 = b64decrypt(b64, inBlock, outBlock, len);


	guessKeysize(outBlock);





				












	return 0;
}

double guessKeysize(bytePtr outBlock)
{
	int i, x;
	double lowest = DBL_MAX; 
	double hd, guess, lowGuess;
	

	for (guess = 2; guess < 40; ++guess)
	{
		hd = 0;	
		byte val1[(int)guess];
		byte val2[(int)guess];
		byte val3[(int)guess];
		byte val4[(int)guess];

		memset(val1, 0, (int)guess);
		memset(val2, 0, (int)guess);
		memset(val3, 0, (int)guess);
		memset(val4, 0, (int)guess);

		for (i = 0; i < guess; ++i)
		{
			val1[i] = outBlock[i];
			val2[i] = outBlock[i+(int)guess];
		}
		i++;	
		for (x = 0; x < guess; ++i, ++x)
		{
			val3[x] = outBlock[i];
			val4[x] = outBlock[i+(int)guess];
		}
		hd += ((hammingDistance(val1,val2)));
		hd += ((hammingDistance(val3,val4)));
		hd /= 2;
		hd /= guess;
		printf("Guess %f - Score %.2f\n", guess, hd); 
	}

	return 0;




}





int hammingDistance(bytePtr val1, bytePtr val2)
{
	int len,i,x,hammingD = 0;
	len = strlen(val1);

	byte result[len], m2;

	for(i = 0; i < len; ++i)
	{
		result[i] = val1[i] ^ val2[i];
	}

	for (i = 0; i < len; ++i)
	{
		for (x = 0; x < 8; ++x)
		{
			m2 = result[i] & 0x1;
			( m2 == 0x1) ? hammingD++ : (m2 = 0x0);
			result[i] = result[i] >> 1;
		}
	}
	return hammingD;
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
