import pexpect, time, pdb

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
