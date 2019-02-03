import pexpect, time, pdb

# Proof of Concept
#
# The goal of challenge 14 is very similar to challenge 12. Though in challenge 12,
# our advantage was our input and its effect on the ciphertext output could be observed
# in the first block
#
# Challenge 14 no longer provides us that advantage, as we now have a random count of bytes
# prepend to our input
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
# We also know that there 9 bytes between the end of the prepended string and the start of the next
# cipher block. 
# 
# If we provide 9 bytes of input to the ciphertext, we are now in the same position as we were in
# challenge 12, that is our put ( after 9 bytes ) will have a direct impact on the the ciphertext
# containing the message we wish to decrypt.
#
# Thus we can begin the attack with suppling 9 + 15 bytes, forcing the following block to give the
# first character of our target to be the last character in a block we have specially crafted
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
prog = pexpect.spawn("./a.out")
prog.setecho(True)
x = 0
str_in = "a"
while True:
    prog.sendline(str_in)
    str_in += "a"
    while True:
        if prog.before is not None and prog.before.decode() != ">>" and x != 0:
            x = 0
            break
        out = prog.readline().decode()
        print(out,end="")
        if out.find(">>") != -1:
            if x == 1:
                break
            else:
                x += 1
