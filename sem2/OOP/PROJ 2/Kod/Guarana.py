from CollisionResult import CollisionResult
from Plant import Plant
import Constants


class Guarana(Plant):
    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.GUARANA_STRENGTH, position, "guarana.png", world, creation_turn)

    def collision(self, opponent):
        result = super().collision(opponent)
        if result == CollisionResult.DIED:  # if guarana was eaten, boost strength of animal who ate it
            opponent.change_strength(Constants.GUARANA_STRENGTH_BOOST)
        return result

