from os import system
import sys


bench = 'c17'
nvecs = 5

seed = sys.argv[1]

# cmd = "./tarmac {0} {1} TARMAC {2}".format(bench, nvecs, seed)
# print(cmd)
# system(cmd)

# print("--- --- --- --- --- --- ---")

# cmd = "./tarmac {0} {1} RANDOM {2}".format(bench, nvecs, seed)
# print(cmd)
# system(cmd)

print("--- --- --- --- --- --- ---")

cmd = "./tarmac {0} {1} SAT {2}".format(bench, nvecs, seed)
print(cmd)
system(cmd)