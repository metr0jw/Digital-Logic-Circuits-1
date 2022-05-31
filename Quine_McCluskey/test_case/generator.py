import random

bit_length = 16
num_of_dontcare = 192
num_of_minterm = 8000
idx = 0


f = open('./input_minterm.txt', 'w')
f.write(f'{bit_length}\n')
data = ''

dontcare = [random.randrange(0, pow(2, bit_length)) for _ in range(num_of_dontcare)]
minterm = [random.randrange(0, pow(2, bit_length)) for _ in range(num_of_minterm)]

for d in dontcare:
    bin2dec = f'{bin(d)[2:]}'
    while len(bin2dec) < bit_length:
        bin2dec = '0' + bin2dec
    f.write(f'd {bin2dec}\n')
    idx += 1

for m in minterm:
    bin2dec = f'{bin(m)[2:]}'
    while len(bin2dec) < bit_length:
        bin2dec = '0' + bin2dec
    f.write(f'm {bin2dec}\n')
f.close()
