from CollisionResult import CollisionResult
from Plant import Plant
import Constants


class Wolfberry(Plant):
    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.WOLFBERRY_STRENGTH, position, "wolfberry.png", world, creation_turn)

    def collision(self, opponent):
        # kil every opponent that ate it
        return CollisionResult.BOTH_DIED
