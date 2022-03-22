from Plant import Plant
import Constants


class Dandelion(Plant):
    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.DANDELION_STRENGTH, position, "dandelion.png", world, creation_turn)
        self._multiplification_attempts = Constants.DANDELION_MULTIPLIFICATION_ATTEMPTS