import random

from Animal import Animal
import Constants
from CollisionResult import CollisionResult


class Tortoise(Animal):

    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.TORTOISE_STRENGTH, Constants.TORTOISE_INITIATIVE, position,
                         "tortoise.png", world, creation_turn)

    def action(self):
        # decide if Tortoise will move in this turn or no
        x = random.random()
        if x >= Constants.TORTOISE_MOVE_PROBABILITY:
            self._world.log(str(self.to_string()) + " didnt move")
            return None
        else:
            return super().action()

    def hostile_collision(self, opponent):
        # if opponent's strength is greater than Tortoise strength - determine if tortoise bounced off the attack
        if opponent.get_strength() > self._strength:
            if opponent.get_strength() > Constants.TORTOISE_DEFENSE:
                return CollisionResult.DIED
            else:
                return CollisionResult.OPPONENT_ESCAPED
        else:
            return CollisionResult.KILLED_OPPONENT

