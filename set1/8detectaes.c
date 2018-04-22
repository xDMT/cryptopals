#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>


#include <mbedtls/aes.h>
    


typedef unsigned char * bytePtr;
typedef unsigned char  byte;




int main(int argc, char * argv[])
{
    FILE * fp = fopen(argv[1], "r");
    char * line = NULL;
    size_t len;
    int i, f, z = 0, ret; 


    while (getline(&line, &len, fp) != -1)
    {

        z++;
        byte buffer1[17], buffer2[17];
        buffer1[16] = '\0';
        buffer2[16] = '\0';

        for (i = 0; i < len-32; i += 16)
        {
            memcpy(buffer1, line+i, 16);

            for (f = 16; f+i < len; f += 16)
            {

                memcpy(buffer2, line+i+f, 16);
                
           //     printf("Comparing %s to %s for line %d\n", buffer1, buffer2, z); 
                ret = strcmp(buffer1,buffer2);
                if (!ret)
                {
                    printf("\nLine %d ECB Mode Detected: %s\n\n", z, line);
                    printf("Repeated byte block #%d: %s\n", (i/16)+1, buffer1);
                    printf("Repeated byte block #%d: %s\n", (i/16)+2, buffer2);
                }
            }
        }
    }


    return 0;
}
