import random

from Animal import Animal
import Constants
from CollisionResult import CollisionResult


class Antelope(Animal):

    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.ANTELOPE_STRENGTH, Constants.ANTELOPE_INITIATIVE, position,
                         "antelope.png", world, creation_turn)
        self._move_range = 2

    def hostile_collision(self, opponent):
        if opponent.get_strength() > self._strength:
            if random.random() >= Constants.ANTELOPE_ESCAPE_PROBABILITY:
                return CollisionResult.ESCAPED
            else:
                return CollisionResult.DIED
        else:
            return CollisionResult.KILLED_OPPONENT
