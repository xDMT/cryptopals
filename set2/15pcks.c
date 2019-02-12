#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0
#define bool int

unsigned char * pkcs_validate(unsigned char* padded_str, bool* valid);
bool output_validity(unsigned char* padded_str, bool* valid);


int main() {
    int i; 
    bool valid = false;
    unsigned char *test1 = "This is valid padding\x000B\x000B\x000B\x000B\x000B" \
                            "\x000B\x000B\x000B\x000B\x000B\x000B";
    unsigned char *test2 = "This is invalid padding\x0007\x000B\x000B\x000B\x000B" \
                            "\x000B\x0003\x000B\x000B\x000B\x000B";
    unsigned char *test3 = "This is valid padding\x0007\x0007\x0007\x0007\x0007" \
                            "\x0007\x0007";
    unsigned char *test4 = "This is invalid padding\x000B\x000B\x000B\x000B\x000B" \
                            "\x000B\x000B\x000B\x000B\x000B";
    unsigned char *test5 = "This is valid padding\x000A\x000A\x000A\x000A\x000A" \
                            "\x000A\x000A\x000A\x000A\x000A";
    
    unsigned char * test_cases[] = { 
        test1, test2, test3, test4, test5
    };
    for (i=0; i < 5; i++) {
        output_validity(test_cases[i], &valid);
    }
    
    
    return 0;
}


unsigned char * pkcs_validate(unsigned char* padded_str, bool* valid) {
   
    int len = strlen(padded_str);
    int i, pad_pos;
    unsigned char pad_val = padded_str[len-1];
    unsigned char * new_str = (unsigned char *) calloc(len,sizeof(unsigned char));


    memcpy(new_str, padded_str, len);
    if (pad_val == 0x1) {
        new_str[len-1] = '\0';
        *valid = true;
        return new_str;
    }
    else if (pad_val > 0xF) {
        *valid - true;
        return padded_str;
    }

    pad_pos = len - pad_val;
    for (i=pad_pos; i < len; ++i) {

        // Validate padding values 
        if (padded_str[i] != pad_val) {
            *valid = false;
            return NULL;
        }

        // For passing padding, strip the string by nulls
        new_str[i] = '\0';
    }
    *valid = true;
    return new_str;

}


bool output_validity(unsigned char* padded_str, bool* valid) {
    
    padded_str = pkcs_validate(padded_str, valid);
    if (*valid) {
        printf("Padding is valid for string: %s\n", padded_str);
        return true;
    }
    else {
        printf("Padding not valid\n");
        return false;
    }
    free(padded_str);

}

