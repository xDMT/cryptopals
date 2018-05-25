from Crypto.Cipher import AES



def main():

    key = b'Sixteen byte key'
    cipher = AES.new(key, AES.MODE_ECB)
    msg = cipher.encrypt(b'Attack at dawndd')
    print(key)

main()
