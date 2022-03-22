from World import *


class GridWorld(World):
    def __init__(self):
        self._is_hex = False
        super().__init__()

    def _add_ground(self, position):
        self._map[position[0]][position[1]] = None
        button = tk.Button(master=self._window, image=self._ground_icon,
                           command=lambda: self._display_adding_window(position))
        button.grid(row=position[1] + 1, column=position[0])

    def _find_free_adjacent(self, position, radius, search_wider):
        for i in range(1 + 2 * radius):
            for j in range(1 + 2 * radius):

                x = i - radius
                y = j - radius

                if self._size[0] > position[0] + x >= 0 and self._size[1] > position[1] + y >= 0:
                    if self._map[position[0] + x][position[1] + y] is None:
                        return position[0] + x, position[1] + y

        if search_wider is True:
            if radius + 1 >= self._size[0] / 2 or radius + 1 >= self._size[1] / 2:
                return None
            else:
                self._find_free_adjacent(position, radius + 1, True)
        else:
            return None

    def _can_search_for_existing_organism(self, pos, x, y, x_min, x_max, y_min, y_max):
        return (x, y) != (pos[0], pos[1]) and self._map[x][y] is not None

    def _display_map(self):
        self._update_turn_display()
        btn_new_turn = tk.Button(master=self._window, text="NEXT", command=self._update)
        btn_new_turn.grid(row=0, column=1, sticky="W")
        btn_save = tk.Button(master=self._window, text="SAVE", command=self._save_world)
        btn_save.grid(row=0, column=2, sticky="W")
        y_range = self._size[1]
        x_range = self._size[0]
        for y in range(y_range):
            for x in range(x_range):
                if self._map[x][y] is None:
                    self._add_ground((x, y))
                else:
                    self._map[x][y].display(self._window)
