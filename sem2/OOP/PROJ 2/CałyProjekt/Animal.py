from Organism import Organism
import random
from CollisionResult import CollisionResult
from MoveStruct import MoveStruct


class Animal(Organism):
    def __init__(self, strength, initiative, position, icon, world, creation_turn):
        super().__init__(strength, initiative, position, icon, world, creation_turn)
        self._move_range = 1
        self.multiplied_this_turn = False

    def set_on_cooldown(self):
        # function that is called in order to ensure that animal that breed with other animal this turn
        # doesnt try to breed again
        self.multiplied_this_turn = True

    def action(self):
        # if organism can move, return generated move
        if self.is_on_cooldown() or self.multiplied_this_turn is True:
            self.multiplied_this_turn = False
        else:
            return self._generate_move()

    def _generate_move(self):
        # generate random move
        if self._world.is_hex():
            m = 5
        else:
            m = 3
        x = random.randint(0, m)
        x = MoveStruct(x)
        return self._decide_move_direction(x)

    def collision(self, opponent):
        if type(opponent) == type(self):
            if self.can_breed() is True:
                return CollisionResult.MULTIPLIED
            else:
                return None
        else:
            return self.hostile_collision(opponent)

    def hostile_collision(self, opponent):
        if opponent.get_strength() > self._strength:
            return CollisionResult.DIED
        else:
            return CollisionResult.KILLED_OPPONENT

    def _decide_move_direction(self, x):
        if x == MoveStruct.RIGHT and self._position[0] + self._move_range < self._world.get_size()[0]:
            return self._position[0] + self._move_range, self._position[1]
        elif x == MoveStruct.LEFT and self._position[0] - self._move_range >= 0:
            return self._position[0] - self._move_range, self._position[1]
        elif x == MoveStruct.DOWN_RIGHT and self._position[1] + self._move_range < self._world.get_size()[1]:
            return self._position[0], self._position[1] + self._move_range
        elif x == MoveStruct.UP_LEFT and self._position[1]-self._move_range >= 0:
            return self._position[0], self._position[1] - self._move_range
        elif x == MoveStruct.DOWN_LEFT and self._position[1]-self._move_range >= 0 and\
                self._position[0]-self._move_range >= 0:
            return self._position[0] - 1, self._position[1] - self._move_range
        elif x == MoveStruct.UP_RIGHT and self._position[1]+self._move_range < self._world.get_size()[1] and\
                self._position[0]+self._move_range < self._world.get_size()[0]:
            return self._position[0] + 1, self._position[1] + self._move_range
        else:
            return None
