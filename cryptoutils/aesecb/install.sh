#! /bin/bash

file=/usr/bin/aesecb

echo "Installing mbedTLS library"
sleep 2

apt-get install libmbedtls10 libmbedcrypto0 libmbedtls-dev

echo "Compiling source file"
sleep 2

gcc -g aesEncDec.c -lmbedtls -lmbedcrypto -o aesecb

if [ ! -e "$file" ]; then
	mv aesecb /usr/bin/.
	echo "Program moved to /usr/bin"
else
	echo "Program file already exists"
	sleep 1
	echo "Leaving file alone"
	exit 0
fi


echo "AES-128 ECB Tool installed.. to use, type aesecb into terminal"




