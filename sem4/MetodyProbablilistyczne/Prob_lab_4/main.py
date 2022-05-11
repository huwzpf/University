import random
import copy


class GenD2:
    def __init__(self, t):
        self.table = t
        self.max_p = max(t)

    def get_number(self):
        rnd1 = random.randint(0, len(t)-1)
        rnd2 = random.uniform(0, self.max_p + 0.01)
        while rnd2 > self.table[rnd1]:
            rnd1 = random.randint(0, len(t)-1)
            rnd2 = random.uniform(0, self.max_p)
        return rnd1 + 1


class GenD1:
    def __init__(self, t):
        self.table = t
        self.sums = []
        for i in range(1, 5):
            self.sums.append(sum(self.table[:i]))

    def get_number(self):
        rnd = random.uniform(0, 1)
        for i in range(4):
            if rnd <= self.sums[i]:
                return i + 1


class GenPoint:
    def __init__(self, table, gen):
        x_probabilities = [sum(a) for a in table]
        self.x_gen = gen(x_probabilities)

        conditional_probabilities = copy.deepcopy(table)
        for i in range(len(x_probabilities)):
            for j in range(len(conditional_probabilities[i])):
                conditional_probabilities[i][j] /= x_probabilities[i]
        self.y_gens = [gen(x) for x in conditional_probabilities]

    def get_point(self):
        a = self.x_gen.get_number()
        return a, self.y_gens[a - 1].get_number()


if __name__ == '__main__':
    t = [[0.15, 0, 0, 0.2],
         [0.05, 0, 0.1, 0],
         [0, 0.1, 0., 0.05],
         [0.2, 0, 0, 0.15]]
    g = GenPoint(t, GenD2)
    results = [[0 for _ in range(4)] for _ in range(4)]
    for _ in range(100000):
        x, y = g.get_point()
        results[x - 1][y - 1] += 1
    for res in results:
        print(res)
