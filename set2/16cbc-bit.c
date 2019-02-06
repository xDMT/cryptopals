#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mbedtls/aes.h>

#define true 1
#define false 0
#define bool int

unsigned char * pkcs_validate(unsigned char* padded_str, bool* valid);
bool output_validity(unsigned char* padded_str, bool* valid);
mbedtls_aes_context * aesInit();
unsigned char * collect_input();
unsigned char * prepend_append(unsigned char *input);


int main() {
    
    unsigned char *in = collect_input();
    printf("%s\n", in);
    free(in);
   
    return 0;
}


unsigned char * pad_string(unsigned char *input) {

    int i;
    size_t in_len = strlen(input);
    size_t pad_len = in_len  % 16;
    unsigned char * pad_buffer = (unsigned char *) calloc(strlen(input) + pad_len,
            sizeof(unsigned char));
    
    strcpy(pad_buffer,input);
    for (i=in_len; i < in_len+pad_len; i++) {
        pad_buffer[i] = (unsigned char) pad_len;
    }
    
    free(input);
    return pad_buffer;
}


unsigned char * prepend_append(unsigned char *input) {

    unsigned char *prepend = "comment1=cooking%20MCs;userdata=";
    unsigned char *append = ";comment2=%20like%20a%20pound%20of%20bacon";
    size_t total_len = strlen(prepend) + strlen(append) + strlen(input) + 1;

    unsigned char *modified_string = (unsigned char *) calloc(total_len, sizeof(unsigned char));

    strcpy(modified_string, prepend);
    strcpy(modified_string+strlen(prepend), input);
    strcpy(modified_string+strlen(prepend)+strlen(input), append);
    
    return modified_string;

}




unsigned char * collect_input() {
    
    int i=0;
    unsigned char c;
    unsigned char *initial_buffer = (unsigned char *) calloc(1000,sizeof(unsigned char));

    while ((c =getchar()) != '\n') {
        if ((c != ';') && (c != '=')) {
            initial_buffer[i++] = c;
        }
    }
    
    unsigned char *buffer = (unsigned char *) calloc(i,sizeof(unsigned char));
    memcpy(buffer, initial_buffer, i);

    free(initial_buffer);
    unsigned char *final_str = prepend_append(buffer);
    unsigned char *final_str_padded = pad_string(final_str);
    free(buffer);

    return final_str_padded;
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

mbedtls_aes_context * aesInit() {

    mbedtls_aes_context* ctx = (mbedtls_aes_context*) malloc(sizeof(mbedtls_aes_context));
    ctx->nr = 10;
    mbedtls_aes_init(ctx);
    
    return ctx;
}
    




