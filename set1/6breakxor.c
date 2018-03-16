#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>



#define TOTAL_GUESSES 40

typedef unsigned char * bytePtr;
typedef unsigned char  byte;



double guessKeysize(bytePtr outBlock, int trySize[]);
void decryptMessageAndFindKey(bytePtr outBlock, int trySize[], bytePtr scoreScale, size_t blockLength);
size_t b64decrypt(bytePtr b64, bytePtr instr, bytePtr b64decryptStr, size_t len);
int hammingDistance(bytePtr val1, bytePtr val2);

int main(int argc, char * argv[])
{
	size_t len, len2;
	int z,i, guessMap[TOTAL_GUESSES];
	FILE * fp = fopen(argv[1], "r");

	byte x,y,key,out;
	bytePtr b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345679+/";
	bytePtr scoreScale = "etaoinsrhldcumfpgwybvkxjqz ETAOINSRHLDCUMFPGWYBVKXJQZ.?!123456789";
	
	// Array to contain the five keysizes with the lowest hamming distance
	bytePtr pch;

	u_int64_t scoreHigh = 0x7FFFFFFF, score = 0x0;




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



	// Allocate space for decoded base64, for every 3 hex characters, 4 base64
	// characters result, len2 stores exact length of outBlock
	bytePtr outBlock = calloc((len*3)/4, sizeof(byte));
	
	// Get hex equiv of base64 block
	b64decrypt(b64, inBlock, outBlock, len);



	// Initialize a guess map for most likely keysizes
	memset(guessMap, 0, 40);

	// Fill guessmap with best possible keysizes
	guessKeysize(outBlock, guessMap);



	// Finds the key and displays the decrypted message
	decryptMessageAndFindKey(outBlock, guessMap, scoreScale, len);

	





				












	return 0;
}

void decryptMessageAndFindKey(bytePtr outBlock, int trySize[], bytePtr scoreScale, size_t blockLength)
{
	int score, z, x, i, r, b, keySize, scoreHigh, scoreHighMessage = INT_MAX;
	byte key, out, message[blockLength], tmessage[blockLength];
	bytePtr pch;
	double scoreAvg;

	memset(message, 0 , blockLength);
	// Each keysize guess
	for (z = 0; trySize[z] != 0; ++z)
	{
		keySize = trySize[z];
		
		
		// Array to hold the correct key
		byte keyMap[keySize];
		

		scoreAvg = 0;
		// Solve each block of keysize
		for (x = 0; x < keySize; ++x)
		{


			scoreHigh = INT_MAX;
			// Try 256 keys
			for (key = 0x0; key < 0xFF; ++key)
			{
				score = 0;
				// Use each key against individual blocks of keysize
				// using multiple of keysize and offset
				for (i = 0; x+(keySize*i) < blockLength; ++i)
				{
					out = key ^ outBlock[x+(keySize*i)];
					pch = strchr(scoreScale, out);
					if(pch)
					{
						score += pch-scoreScale;
					}
					else
					{
						score += 255;
					}
				}
				// Get lowest score of all 256 keys by comparing to
				// and modifying INT_MAX
				//
				// Store lowest scoring key in the keyMap
				if (score < scoreHigh)
				{
					keyMap[x] = key;
					scoreHigh = score;
				}
				scoreAvg += score;
			}
		}

		scoreAvg/= (double)keySize;
		printf("Average score for key: %s is %.3f\n", keyMap, scoreAvg);
			
		// Try key on main block, only show message if score is lowest
		memset(tmessage, 0 , blockLength);
		score = 0;
		for (r = 0; r < blockLength;)
		{
			for (b = 0; b < keySize; ++b, ++r)
			{
				tmessage[r] = keyMap[b] ^ outBlock[r];
				pch = strchr(scoreScale, tmessage[r]);
				if(pch)
				{
					score += pch-scoreScale;
				}
				else
				{
					score += 255;
				}
			}
		}
		if (score < scoreHighMessage)
		{
			strcpy(message, tmessage);
			scoreHighMessage = score;
		}



	}

	
	printf("\n\n\n####################################\n\n\n");

	printf("%s", message);

	return;



					
}




double guessKeysize(bytePtr outBlock, int trySize[])
{
	int i, x, z = 0;
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

		// Compares blocks of guess size 
		for (i = 0; i < guess; ++i)
		{
			val1[i] = outBlock[i];
			val2[i] = outBlock[i+(int)guess];
		}
		i++;	
		// 4 Seperate blocks
		for (x = 0; x < guess; ++i, ++x)
		{
			val3[x] = outBlock[i];
			val4[x] = outBlock[i+(int)guess];
		}
		// Gets hamming distance of each block set
		// Averages the result by dividing by sets (2)
		hd += ((hammingDistance(val1,val2)));
		hd += ((hammingDistance(val3,val4)));
		hd /= 2;

		// Normalizes the result by dividing by guess
		hd /= guess;

		// All sub-2 hamming distances are to be tried as possible
		// key sizes, this is because after reviewing hamming distances
		// of 38 guess, there are only 4-6 keysizes that produce sub-2
		// hamming distances
		if (hd < 2)
		{
			trySize[z++] = guess;
		}

		printf("Guess %.0f - Score %.2f\n", guess, hd); 
	}
	

	printf("\n\n\n####################################\n\n\n");
	return 0;




}





int hammingDistance(bytePtr val1, bytePtr val2)
{
	int len,i,x,hammingD = 0;
	len = strlen(val1);

	byte result[len], m2;


	// Xors each block against eachother
	for(i = 0; i < len; ++i)
	{
		result[i] = val1[i] ^ val2[i];
	}


	// Counts one bits by ANDing the 0x1 bit
	// and shifting byte to account for every bit in
	// the byte
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
