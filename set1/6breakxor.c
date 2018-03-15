#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef unsigned char * bytePtr;
typedef unsigned char  byte;



size_t b64decrypt(bytePtr b64, bytePtr instr, bytePtr b64decryptStr, size_t len);
bytePtr loadFile(bytePtr instr, bytePtr filename);

int main(int argc, char * argv[])
{
	FILE * fp = fopen(argv[1], "r");
	if(!fp){
		exit(1);
	}
	fseek(fp, 0, SEEK_END);
	size_t len = ftell(fp);
	rewind(fp);
	bytePtr inBlock = calloc(len, sizeof(byte));
	if(fread(inBlock, sizeof(char), len, fp) != len){
		exit(1);
	}
	fclose(fp);


	bytePtr outBlock = calloc((len*3)/4, sizeof(byte));
	bytePtr b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345679+/";

	size_t len2 = b64decrypt(b64, inBlock, outBlock, len);

	byte x,y,key,out,keymap[5];
	int z;
	u_int64_t scoreHigh = 0x7FFFFFFF, score = 0x0;
	bytePtr scoreScale = "etaoinsrhldcumfpgwybvkxjqz ETAOINSRHLDCUMFPGWYBVKXJQZ.?!123456789";
	bytePtr pch;

	for (y = 0; y < 5; ++y)
	{
		scoreHigh = 0x7FFFFFFF;
		for (x = 0; x < 0xFF; ++x)
		{
			score = 0;
			for(z = 0; z < (len2/5); ++z)
			{
				key = x;
				out = outBlock[(z*5)+y] ^ key;
				pch = strchr(scoreScale, out);
				if (!pch)
				{
					score += 255;
				}
				else
				{
					score += pch-scoreScale;
				}
			}
			if (score < scoreHigh)
			{
				scoreHigh = score;
				keymap[y] = x;
			}
		}
	}



	for (z = 0; z < len2; )
	{
		for (y = 0; y < 5; ++y, ++z)
		{
			outBlock[z] ^= keymap[y];
			printf("%c",outBlock[z]);
		}
	}	


	printf("%s, %d", keymap, scoreHigh);

				

				









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

