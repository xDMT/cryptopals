#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


#define DEBUG




int main(int argc, char * argv[])
{
	size_t len;
	int i,x;
	i = x = 0;
#ifndef DEBUG

	unsigned char input[512], key[3], c;
	printf("Enter an ASCII string to encrypt: ");
	while((c = getchar() ) && (c != '\n'))
	{
		input[i++] = c;
	}

	i = 0;
	printf("Enter a 3 character key to encrypt with: ");
	while((c = getchar() ) && (c != '\n'))
	{
		key[i++] = c;
	}
	
#else

	unsigned char input[] = "Burning \'em, if you ain\'t quick and nimble\nI go crazy when I hear a cymbal";
	unsigned char key[] = "ICE";

#endif
	len = strlen(input);


	for (i = 0; i < len;)
	{
		for (x = 0; x < 3; ++x,++i)
		{
			input[i] ^= key[x];
		}
	}

	printf("\nEncrypted string (in hex) : ");
	for (i = 0; i < len; ++i)
	{
		printf("%x", input[i]);
	}

	return 0;
}

