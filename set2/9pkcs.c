#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <assert.h>


    






int main(int argc, char * argv[])
{
    // Print usage and exit if invalid number of arguments
    if (argc != 3)
    {
        printf("\nUsage: <program name> <file> <blocksize>\n\n");
        exit(1);
    }


    FILE * fp = fopen(argv[1], "r");
    int blockSize, padding, i;
    size_t len;

    // Get specified block size for message from command line argument 
    blockSize  = atoi(argv[2]);


    // Get message length of file
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rewind(fp);
    

    // Determine padding amount by subtracting length
    // from the largest nearest blockSize multiple
    while (blockSize < len)
    {
        blockSize += 16;
    }
    padding = blockSize - len;

    // Create buffer with proper length for padded
    // message
    unsigned char paddedMsg[len+padding];


    // Read bytes into buffer and begin padding
    fread(paddedMsg, 1, len, fp);


    // Append bytes to end of original file
    for (i = len; i < (len+padding); ++i)
    {
        paddedMsg[i] = (unsigned char)padding;
    }
    fclose(fp);


    // Write new buffer to same file
    FILE * fpWrite = fopen(argv[1], "w");
    fwrite(paddedMsg, 1, len+padding, fpWrite);
    fclose (fpWrite);



    // Show status
    printf("\n\n");
    printf("[+] %d bytes read from %s\n", len, argv[1]);
    printf("[+] %d bytes appended as padding\n", padding);
    printf("[+] %d bytes written to %s\n\n", len+padding, argv[1]);

    







    return 0;
}
