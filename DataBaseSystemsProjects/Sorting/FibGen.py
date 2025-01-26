class FibGen:
    def __init__(self):
        self.a = 0
        self.b = 1

    def __iter__(self):
        return self

    def __next__(self):
        ret = self.b
        c = self.a + self.b
        self.a = self.b
        self.b = c
        return ret
