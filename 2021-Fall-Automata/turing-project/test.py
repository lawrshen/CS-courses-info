import subprocess

def gcd(a,b):
    while b!=0:
        c = a%b
        a = b
        b = c
    return a

# n = 100
# for i in range(1,n):
#     prefix = i*'1'
#     for j in range(i+1,n):

a,b = input().split()
i,j = int(a),int(b)

prefix = i*'1'
postfix = j*'1'
str = prefix + '0' +postfix
process = subprocess.Popen(['./turing','../programs/gcd.tm',str],stdout=subprocess.PIPE)
output = process.stdout.readline()
if output.strip()!=b'1'*gcd(i,j):
    print(i,j,str)
    # break
