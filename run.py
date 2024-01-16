from os import system

bench = 'test'
nvecs = 4

cmd = "./tarmac {0} {1} TARMAC".format(bench, nvecs)
print(cmd)
system(cmd)

# print("--- --- --- --- --- --- ---")

cmd = "./tarmac {0} {1} RANDOM".format(bench, nvecs)
print(cmd)
system(cmd)
