from CollisionResult import CollisionResult
from CyberSheep import CyberSheep
from Plant import Plant
import Constants


class Hogweed(Plant):
    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.HOGWEED_STRENGTH, position, "hogweed.png", world, creation_turn)
        self._is_poisonous = True

    def action(self):
        breeding_res = super().action()
        return breeding_res, Constants.HOGWEED_POISON_RADIUS  # returns a tuple, where first element is breeding result
        # and second is radius of poison ( in what radius from Hogweed all organisms shall be killed )

    def collision(self, opponent):
        # kill every opponent except of CyberSheep
        if type(opponent) is CyberSheep:
            return CollisionResult.DIED
        else:
            return CollisionResult.KILLED_OPPONENT
