# cryptopals
This repo consists of my solutions to the Cryptopals challenges (https://cryptopals.com)

Also in the directory 'cryptoutils' you will find a collection of cryptographic programs
and utilities, unrelated to the challenges.

My latest contribution to that is a simple AES-128 encryption/decryption tool for use
with ECB mode. The program is only one file, so you can easily clone the repo, and compile 
using 'gcc aesEncDec.c -lmbedtls -lmbedcrypto' 

Dependent on mbedTLS library, which can be found in the Debian repositories.

