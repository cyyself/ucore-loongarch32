#!/usr/bin/env python3

regs = []

with open("../kern/include/asm/regdef.h") as file:
    for line in file.readlines():
        s = line.strip().replace("\t"," ")
        sp = s.split(" ")
        if len(sp) >= 3 and sp[2].startswith("$r"):
            sp = sp[:3]
            regs.append(sp[1])

def make_pushregs(base=148):
    i = 31
    while i >= 0:
        print("st.w\t{}, sp, {}".format(regs[i],base))
        i -= 1
        base -= 4

def make_popregs(base=148):
    buffer = []
    i = 31
    while i >= 0:
        buffer.append("ld.w\t{}, sp, {}".format(regs[i],base))
        i -= 1
        base -= 4
    print("\n".join(buffer[::-1]))


#make_pushregs()
make_popregs()
