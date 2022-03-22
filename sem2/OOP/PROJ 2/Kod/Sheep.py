from Animal import Animal
import Constants
from CollisionResult import CollisionResult


class Sheep(Animal):

    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.SHEEP_STRENGTH, Constants.SHEEP_INITIATIVE, position, "sheep.png",
                         world, creation_turn)


