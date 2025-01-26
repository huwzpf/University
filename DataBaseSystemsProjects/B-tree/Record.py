from math import pi


class Record:
    def __init__(self, radius: float = 0, height: float = 0, index: int = 0):
        self.radius = radius
        self.height = height
        self.index = index

    def __repr__(self):
        return f'Right circular cylinder - index: {self.index} - ' \
               f' base radius: {self.radius:.2f}, height: {self.height:.2f}, volume: {self.get_volume():.2f}'

    def to_string(self):
        return f'{"index": {self.index} "radius": {self.radius}, "height": {self.height}}'

    def get_volume(self):
        return pi * (self.radius ** 2) * self.height

    def __eq__(self, other):
        return self.get_volume() == other.get_volume()

    def __gt__(self, other):
        return self.get_volume() > other.get_volume()

    def __lt__(self, other):
        return self.get_volume() < other.get_volume()

    def __ge__(self, other):
        return self > other or self == other

    def __le__(self, other):
        return self < other or self == other