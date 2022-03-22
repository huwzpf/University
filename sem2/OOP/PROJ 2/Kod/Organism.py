import tkinter as tk
from abc import ABC, abstractmethod
import Constants
import math


class Organism(ABC):
    generation_probability = 1

    def __init__(self, strength, initiative, position, icon, world, creation_turn):
        self._strength = int(strength)
        self._initiative = int(initiative)
        self._position = tuple(position)
        self._icon = tk.PhotoImage(file=icon)
        if self._icon.width() != 50 or self._icon.height() != 50:
            # require 50 x 50 button size
            raise Exception("Invalid photo size")
        self._world = world
        self.icon_button = None
        self._creation_turn = creation_turn

    def get_position(self):
        return self._position

    def get_initiative(self):
        return self._initiative

    def get_creation_turn(self):
        return self._creation_turn

    def change_strength(self, amount):
        self._strength += amount

    def get_strength(self):
        return self._strength

    def is_on_cooldown(self):
        # organism is on cooldown in first turn after creation
        if self._world.get_turn() - self._creation_turn >= Constants.ACTION_COOLDOWN:
            return False
        else:
            return True

    def can_breed(self):
        # organism cant breed if first 5 turns after creation
        if self._world.get_turn() - self._creation_turn >= Constants.MULTIPLIFICATION_COOLDOWN:
            return True
        else:
            return False

    @abstractmethod
    def action(self):
        pass

    @abstractmethod
    def collision(self, opponent):
        pass

    def to_string(self):
        ret = str(type(self).__name__) + " at " + str(self._position)
        return ret

    def display(self, window):
        if self._world.is_hex():
            self._display_at_position(window)
        else:
            self._display_at_grid(window)


    def _display_at_grid(self, window):
        # display method adds button at organism position
        x = self._position[0]
        y = self._position[1] + Constants.MAP_Y_OFFSET
        self.icon_button = tk.Button(master=window, image=self._icon)
        self.icon_button.grid(row=y, column=x)

    def _display_at_position(self, window):
        x = self._position[0]
        y = self._position[1]
        center = self._world.get_hex_center((x, y))
        self.icon_button = tk.Button(master=window, image=self._icon)

        a = self._world.get_hex_width()/2
        rad = self._world.get_hex_radius()

        # calcualte where to place the button - left top corner of hex and in  a - rad / 2 distance from it
        point = (center[0] - a, center[1] - (0.5 * rad))
        self.icon_button.place(x=point[0] + a - rad / 2, y=point[1]+30)





