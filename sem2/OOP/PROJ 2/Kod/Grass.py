from Plant import Plant
import Constants


class Grass(Plant):
    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.GRASS_STRENGTH, position, "grass.png", world, creation_turn)
