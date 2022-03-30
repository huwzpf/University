from bitarray import bitarray
import math
from bitarray.util import ba2int

class Gen1:
    def __init__(self):
        self.a = 69069
        self.x_last = 15
        self.c = 1
        self.M = 2**31

    def get_number(self):
        ret = (self.a * self.x_last + self.c) % self.M
        self.x_last = ret
        return ret
class Gen2:
    def __init__(self):
        self.n = 31
        self.p = 7
        self.q = 3
        self.start_val = bitarray('1011010')

    def get_number(self):
        ret = bitarray(31)
        for i in range(7):
            ret[i] = self.start_val[i]

        for i in range(7, self.n):
            ret[i] = ret[i - self.p] ^ ret[i - self.q]

        self.start_val = ret[-7:]
        return ba2int(ret)

def main():
    g1 = Gen1()

    n = 2**31

    values = [0 for _ in range(10)]
    for i in range(100000):
        num = g1.get_number()
        values[math.floor((10 * num)/n)] += 1

    print(f""values)

    g2 = Gen2()

    values = [0 for _ in range(10)]
    for i in range(100000):
        num = g2.get_number()
        values[math.floor((10 * num)/n)] += 1

    print(values)


if __name__ == "__main__":
    main()