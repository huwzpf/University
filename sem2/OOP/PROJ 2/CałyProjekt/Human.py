import random

from Animal import Animal
import Constants
from MoveStruct import MoveStruct


class Human(Animal):
    generation_probability = 0

    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.HUMAN_STRENGTH, Constants.HUMAN_INITIATIVE, position,
                         "human.png", world, creation_turn)
        self._last_activation = -1
        self._ability_active = False

    def can_activate(self):
        if self._last_activation == -1:
            return True
        else:
            return self._ability_active is False and self._last_activation + Constants.HUMAN_ABILITY_ACTIVE_TIME + Constants.HUMAN_ABILITY_COOLDOWN < self._world.get_turn()

    def activate_ability(self):
        if self.can_activate():
            self._world.log("Human ability active")
            self._ability_active = True
            self._move_range = Constants.HUMAN_ABILITY_MOVE_RANGE
            self._last_activation = self._world.get_turn()

    def action(self):
        current_turn = self._world.get_turn()

        # world has a variable that determines if button activating human ability was clicked
        if self._world.did_activate_human_ability() is True:
            self.activate_ability()

        # deactivate human ability after HUMAN_ABILITY_ACTIVE_TIME
        if self._ability_active is True and current_turn - self._last_activation >= Constants.HUMAN_ABILITY_ACTIVE_TIME:
            self._ability_active = False
            self._move_range = Constants.ANIMAL_DEFAULT_MOVE_RANGE
            self._world.log("Human ability inactive")

        elif self._ability_active is True and current_turn - self._last_activation >= \
                Constants.HUMAN_ABILITY_FULLY_ACTIVE:
            # in this period of time human doesnt have 100% chance to keep ability active
            # 50% to activate
            x = random.randint(0, 2)
            if x == 1:
                self._move_range = Constants.ANIMAL_DEFAULT_MOVE_RANGE
            else:
                self._move_range = Constants.HUMAN_ABILITY_MOVE_RANGE

        # world has a variable that determines what button was clicked ( so in what direction human shall move )
        direction = self._world.get_human_move_direction()

        return super()._decide_move_direction(direction)
