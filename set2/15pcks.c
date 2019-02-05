#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

int main() {







    return 0;
}


int pkcs_validate(unsigned char* padded_str) {
   
    int len = strlen(padded_str);
    int i, pad_pos;
    unsigned char pad_val = padded_str[len-1];

    if (pad_val == 0x1) {
        pad_val[len-1] = '\0';
        return true;
    }
    pad_pos = len - pad_val - 1;
    for (i=pad_pos; i < len; ++i) {
        if (padded_str[i] != pad_val) {
            return false;
        }
    }




}
