# Proof of Concept
#
# The goal of challenge 14 is very similar to challenge 12. Though in challenge 12,
# our advantage was our input and its effect on the ciphertext output could be observed
# in the first block
#
# Challenge 14 no longer provides us that advantage, as we now have a random count of bytes
# prepended to our input
#
# Though as stated in the challenge, no further tools are needed for executing our chosen-plaintext
# attack. All we have to do is simply determine the byte count prepended to our input. As soon as we
# know this information, we can essentially 'pad out' the prepended string with our own input,
# and then begin brute forcing the ciphertext as we have before, except now we will be starting our
# measurements at the first block that our input becomes the only plaintext in the ciphertext
#
#
# For instance, lets assume 73 random bytes have been prepended to the input. 
#   
#   Thus we have:
#       73 div 16 = 4.5625
#       73 mod 16 = 9 
#
# From this, we know that the first 4 blocks of ciphertext we can ignore completely, as they will
# remain constant throughout the attack.
# 
# We also know that there are 9 bytes between the end of the prepended string and the start of the next
# cipher block. 
# 
# If we provide 9 bytes of input to the ciphertext, we are now in the same position as we were in
# challenge 12, that is, our input ( after 9 bytes ) will have a direct impact on the the ciphertext
# containing the message we wish to decrypt.
#
# Thus we can begin the attack with suppling 9 + 15 bytes, forcing the following block to provide the
# first character of our target mesage to be the last character in our block we have specially crafted
#
#   AAAAAAAAAAAAAAAX   <- With X being our target byte. 
#   
# Just like challenge 12, we now have the corresponding cipher text of the block AAAAAAAAAAAAAAAX
# which we can bruteforce by giving all 256 possible input choices to that block, and determining
# when the ciphertext matches the crafted input block of 15 bytes + the target byte
#
# 
# Because this is slightly harder to demonstrate byte by byte, I've written a proof of concept python
# script to automate this process for us, and we can watch in real time the process of the chosen
# plaintext attack
#

import pexpect, time, pdb
import pprint, os

pp = pprint.PrettyPrinter(indent=4)
prog = pexpect.spawn("./a.out")
prog.setecho(True)
x = 0
str_in = "A"
block_list = {}
block_list_last = {}
static_blocks = []
z = 0
entry_block_val_last = None
for y in range(0,100):
    z += 1
    prog.sendline(str_in)
    str_in += "a"
    for y in range(0,1000):
        if prog.before is not None and prog.before.decode() != ">>" and x != 0:
            x = 0
            break
        out = prog.readline().decode()
        if out.find("Block") != -1:
            block_list[out.split(':')[0]] = out.split(':')[1].replace('\n','').replace('\r','')

            # Compare blocks for changes
            if out.split(':')[0] not in block_list_last:
                block_list_last[out.split(':')[0]] = out.split(':')[1].replace('\n','').replace('\r','')
            else:
                if block_list_last[out.split(':')[0]] == block_list[out.split(':')[0]]:
                    if out.split(':')[0] not in static_blocks:
                        static_blocks.append(out.split(':')[0])
        
        if len(block_list) != 0:
            print('\033c')
            pp.pprint(block_list)
        if len(static_blocks) != 0:
            print("\n\nStatic Blocks:\n---------------")
            pp.pprint(static_blocks) 
        if out.find(">>") != -1:
            if x == 1:
                break
            else:
                x += 1

    # Detect the entry block
    if len(static_blocks) != 0:
        last_block = static_blocks[len(static_blocks)-1]
        next_block_num = int(last_block[len(last_block)-2])+1
        next_block = "Block " + str(next_block_num) + " "
        entry_block_val = block_list[next_block] 
        print("Entry block is " + next_block + ": " + entry_block_val + "\n\n")
        if entry_block_val_last is None:
            entry_block_val_last = entry_block_val
        else:
            if entry_block_val_last == entry_block_val:
                z -= 1
                print("Byte count for injection is: " + str(z))
                break
            else:
                entry_block_val_last = entry_block_val
        

next_block_num += 1
next_block = "Block " + str(next_block_num)

# Now begin attack
str_attack_basis = ""
for d in range(0,z):
    str_attack_basis += "A"
str_attack = ""
attack_counter = 15
while True:
    for d in range(0,attack_counter):
        str_attack += "A"
    str_attack += str_attack_basis
    prog.sendline(str_attack)

    block_pos = 0
    while True:
        out = prog.readline().decode()
        if out.find(next_block) != -1:
            break
        block_pos += 1

    basis_cipher = out.split(':')[1].replace('\n','').replace('\r','')
    compare_cipher = ""
    for c in range(0x0, 0xFF):
        prog.sendline(str_attack + chr(c))
        for r in range(0, block_pos):
            compare_cipher = prog.readline().decode().replace('\n','').replace('\r','')
            try: 
                compare_cipher = compare_cipher.split(':')[1]
            except:
                pass
        if compare_cipher == basis_cipher:
            print(c)
            break
    attack_counter -= 1
    if attack_counter == 0:
        attack_counter = 15


        

    






