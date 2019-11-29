import commands
import os
import sys

def init():
    compile = commands.getstatusoutput("gcc -o hw4 hw4.c")
    assert(compile[0] == 0)

def clean():
    clean = commands.getstatusoutput("rm hw4")
    assert(clean[0] == 0)

def arg_tests():
    init()
    zero_arg = commands.getstatusoutput("./hw4")
    assert(zero_arg[0] != 0)
    two_arg = commands.getstatusoutput("./hw4 a b")
    assert(two_arg[0] != 0)
    clean()

def non_exist_file():
    init()
    output = commands.getstatusoutput("./hw4 lll")
    assert(output[0] != 0)
    clean()

def exist_file():
    init()
    output = commands.getstatusoutput("./hw4 hw4.c")
    print(output)
    assert(output[0] == 0)
    clean()


arg_tests()
non_exist_file()
exist_file()