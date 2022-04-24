import random
from collections import OrderedDict
import math


class GenC:
    def __init__(self):
        pass


class GenD:
    def __init__(self):
        self.table = [0.2, 0.4, 0.15, 0.25]
        self.sums = []
        for i in range(1, 5):
            self.sums.append(sum(self.table[:i]))

    def get_number(self):
        rnd = random.uniform(0, 1)
        for i in range(4):
            if rnd <= self.sums[i]:
                return i + 1


class GenC:
    def __init__(self):
        pass

    def inv_f(self, n):
        return (n * 100)+50

    def get_number(self):
        return self.inv_f(random.uniform(0, 1))


def main():
    gd = GenD()
    values = [0 for _ in range(4)]
    for i in range(100000):
        num = gd.get_number()
        values[num-1] += 1
    print(values)

    gc = GenC()
    values = [0 for _ in range(100)]
    for i in range(100000):
        num = gc.get_number()
        values[math.floor(num - 50)] += 1
    print(values)


if __name__ == "__main__":
    main()