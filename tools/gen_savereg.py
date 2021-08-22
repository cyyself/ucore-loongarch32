#!/usr/bin/env python3

regs = dict()

with open("../kern/include/asm/regdef.h") as file:
    for line in file.readlines():
        s = line.strip().replace("\t"," ")
        sp = s.split(" ")
        if len(sp) >= 3 and sp[2].startswith("$r"):
            sp = sp[:3]
            regs[int(sp[2][2:])] = sp[1]

def make_pushregs(base=148):
    print("push_reg:")
    i = 31
    while i >= 0:
        print("st.w\t{}, sp, {}".format(regs[i],base))
        i -= 1
        base -= 4

def make_popregs(base=148):
    print("pop_reg:")
    buffer = []
    i = 31
    while i >= 0:
        buffer.append("ld.w\t{}, sp, {}".format(regs[i],base))
        i -= 1
        base -= 4
    print("\n".join(buffer[::-1]))

assert(len(regs)==32,"len(regs) should be 32!")
print(regs)

make_pushregs()
make_popregs()
