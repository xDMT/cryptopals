# cryptopals
This repo consists of my solutions to the Cryptopals challenges (https://cryptopals.com)

### Set 1 - Basics
---

##### [x] Challenge 1 -> Convert hex to base64
Write a function to convert hexadecimal input to base64 output

##### [x] Challenge 2 -> Fix XOR
Write a function that takes two equal-length buffers and produces their XOR combination

##### [x] Challenge 3 -> Single-byte XOR cipher
Given a string that has been XOR'd against a single character, write a function to find the encryption key and and decrypt the message.

##### [x] Challenge 4 -> Detect single-character XOR
Given a file with 60 strings, only one of them have been ecrypted by single-character XOR. Write a function to parse the strings and find the encrypted string  

##### [x] Challenge 5 -> Implement repeating-key XOR
Given a plain text input, write a function to encrypt the string under repeating-key XOR. 

##### [x] Challenge 6 -> Break repeating-key XOR
Given a file that has been encrypted under repeating key XOR, and then base64 encoded, wrtie a program to find the key and decrypt the file. Also known as the Vigenere Cipher.

##### [x] Challenge 7 -> AES in ECB mode
Given a file encrypted under AES-128 in ECB mode, under a supplied key, and then base64 encoded, parse the file and decrypt the message using the key. 

##### [x] Challenge 8 -> Detect AES in ECB mode
Given a file of hex encoded ciphertexts, one of them have been encrypted under AES-128 in ECB mode. Write a function to detect that string. 



### Set 2 - Block Crypto 
---

##### [x] Challenge 9 -> Implement PKCS#7 Paddingp
Write a function to implement PKCS#7 padding, that is, pad out irregular sized input to the specified block length, writing the value of the padding bytes as the total count of padding bytes. 

##### [x] Challenge 10 -> Implement CBC mode
Given a file encrypted under AES-128 in CBC mode, write a function to decrypt the message using a supplied key. Instead of using a library function for CBC mode, implement it yourself by modifying your ECB implementation to accept initialization vectors.

##### [x] Challenge 11 -> An ECB/CBC detection oracle
Write a program to generate a random AES key, and encrypt input under this key in either ECB or CBC mode. Under the hood, have the function append 5-10 bytes ( count chosen randomly ) before the plaintext and 5-10 bytes after the plaintext. Now have the function randomly decide to encrypt under ECB or CBC mode. Write a function that, for every input given to your program, it should be able to determine whether the resulting ciphertext was encrypted under ECB mode or CBC mode.

##### [x] Challenge 12 -> Byte-at-a-time ECB decryption (Simple)
Write a program that encrpyts input under a consistent but unknown key ( generate the key at runtime ). Then write a function that appends a stringstring to the input, initially base64 encoded, but write a function to decode the string before appending it. Your program should output the ciphertext of your input + an unknown base64 decoded string. By using nothing more than your ciphertext oracle, decrypt the ciphertext by using a known-plaintext attack. 

##### [x] Challenge 13 -> ECB cut-and-paste
Write a k=v parsing routine, to parse the string into profile values. Allow your profile to accept profile value inputs, and output the corresponding cipher text as it is parsed. Using only your oracle function and the ciphertext output, craft a ciphertext that will decode to a modified profile with elevated privileges.

##### [ ] Challenge 14 -> Byte-at-a-time ECB decryption (Hard)
Similar to challenge 12, again use a known plaintext attack to decrypt an unknown message appended to some given plaintext. In this challenge though, modify your function to also prepend a random count of bytes to the given input.

##### [ ] Challenge 15 -> PKCS#7 padding validation
Write a function that takes plaintext, and determine if it has valid PKCS#7 padding, and strips the padding off. 

##### [ ] Challenge 16 -> CBC bitflipping attacks
Write a function to generate a random AES key and encrypt given input in CBC mode under that key. The function should also prepend and append a known message to the input. The function should quote out the ";" and "=" characters.
The function should then pad out the input to the 16-byte AES block length and encrypt it under the random AES key.The second function should decrypt the string and look for the characters ";admin=true;" (or, equivalently, decrypt, split the string on ";", convert each resulting string into 2-tuples, and look for the "admin" tuple). Return true or false based on whether the string exists. If you've written the first function properly, it should not be possible to provide user input to it that will generate the string the second function is looking for. We'll have to break the crypto to do that. Instead, modify the ciphertext (without knowledge of the AES key) to accomplish this. You're relying on the fact that in CBC mode, a 1-bit error in a ciphertext block: 
Completely scrambles the block the error occurs in
Produces the identical 1-bit error(/edit) in the next ciphertext block.



