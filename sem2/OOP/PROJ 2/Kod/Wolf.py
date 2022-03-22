from Animal import Animal
import Constants
from CollisionResult import CollisionResult


class Wolf(Animal):

    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.WOLF_STRENGTH, Constants.WOLF_INITIATIVE, position, "wolf.png", world, creation_turn)
