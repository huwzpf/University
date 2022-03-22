from Animal import Animal
import Constants


class Fox(Animal):
    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.FOX_STRENGTH, Constants.FOX_INITIATIVE, position, "fox.png", world, creation_turn)

    def action(self):
        potential_move = self._generate_move()
        # potential move might be none if generated move is outside of map borders
        if potential_move is not None:
            potential_opponent = self._world.get_map_field(potential_move)
        else:
            potential_opponent = None

        # determine if potential opponent is stronger than Fox, if it is dont move
        if potential_opponent is not None and potential_opponent.get_strength() >= Constants.FOX_STRENGTH:
            self._world.log(str(self.to_string()) + " didnt move")
            return None
        else:
            return potential_move
