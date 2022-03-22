import Constants
from Organism import Organism
from CollisionResult import CollisionResult
import random


class Plant(Organism):
    def __init__(self, strength, position, icon, world, creation_turn):
        super().__init__(strength, 0, position, icon, world, creation_turn)
        self._multiplification_attempts = Constants.PLANT_MULTIPLIFICATION_ATTEMPTS
        self._is_poisonous = False

    def is_poisonous(self):
        # function determining if Plant kills everything around it - used for Hogweed implementatnion
        return self._is_poisonous

    def action(self):
        multiplified = 0
        if self.can_breed():
            for i in range(self._multiplification_attempts):
                if random.random() <= Constants.PLANT_MULTIPLIFICATION_PROBABILITY:
                    multiplified += 1

        return multiplified

    def collision(self, opponent):
        if opponent.get_strength() > self._strength:
            return CollisionResult.DIED
        else:
            return CollisionResult.KILLED_OPPONENT
