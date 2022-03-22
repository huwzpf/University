from World import *


class HexWorld(World):
    def __init__(self):
        self._map_canvas = None
        self._hex_radius = 50
        self._hex_width = math.sqrt(3) * self._hex_radius
        self._is_hex = True
        super().__init__()

    def get_hex_radius(self):
        return self._hex_radius

    def get_hex_width(self):
        return self._hex_width

    def _add_hex(self, center, x, y):
        half_width = self._hex_width / 2
        # calculate positions of all points needed to create hexagon with given center
        point_1 = (center[0], center[1] + self._hex_radius)
        point_2 = (center[0] + half_width, center[1] + (0.5 * self._hex_radius))
        point_3 = (center[0] + half_width, center[1] - (0.5 * self._hex_radius))
        point_4 = (center[0], center[1] - self._hex_radius)
        point_5 = (center[0] - half_width, center[1] - (0.5 * self._hex_radius))
        point_6 = (center[0] - half_width, center[1] + (0.5 * self._hex_radius))
        pol = self._map_canvas.create_polygon([point_1, point_2, point_3, point_4, point_5, point_6],
                                              fill="white", outline="black")
        # self._map_canvas.tag_bind(pol, "<Button-1>", lambda e: print(x, y))

    def get_hex_center(self, position):
        half_width = self._hex_width / 2
        return half_width * (position[1] + 1) + half_width * position[0] * 2, self._hex_radius +\
            self._hex_radius * position[1] * 1.5

    def _print_hex(self, position):
        self._add_hex(self.get_hex_center(position), position[0], position[1])

    def _add_ground(self, position):
        x = position[0]
        y = position[1]
        self._map[x][y] = None
        button = tk.Button(master=self._window, image=self._ground_icon,
                           command=lambda: self._display_adding_window(position))
        center = self.get_hex_center((x, y))

        a = self._hex_width / 2
        rad = self._hex_radius

        point = (center[0] - a, center[1] - (0.5 * rad))
        button.place(x=point[0] + rad * (math.sqrt(3) - 1) / 2, y=point[1] + 30)

    def _find_free_adjacent(self, position, radius, search_wider):
        for i in range(1 + 2 * radius):
            for j in range(1 + 2 * radius):
                # dont search left top and bottom right corners -> in array x o o |search only in 'o' regions |x o o
                #                                                           o X o |   since at hex grid       | o {} o
                #                                                           o o x |    it looks like -------->|   o o x
                # the same principle applies to larger radius search

                if (i == 0 and j == 0) or (i == 2 * radius and j == 2 * radius):
                    continue
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
        return (x, y) != (pos[0], pos[1]) and\
                        not (x == x_min and y == y_min) and\
                        not (x == x_max and y == y_max) and\
                        0 <= x < self._size[0] and 0 <= y < self._size[1] and\
                        self._map[x][y] is not None

    def _display_map(self):
        self._map_canvas = tk.Canvas(master=self._window, width=self._hex_width * 2 * self._size[0],
                                     height=self._hex_radius * 2 * self._size[1])
        self._update_turn_display()
        self._window.rowconfigure(0, minsize=30)
        btn_new_turn = tk.Button(master=self._window, text="NEXT", command=self._update)
        btn_new_turn.grid(row=0, column=1)
        btn_save = tk.Button(master=self._window, text="SAVE", command=self._save_world)
        btn_save.grid(row=0, column=2)
        y_range = self._size[1]
        x_range = self._size[0]
        for y in range(y_range):
            for x in range(x_range):
                self._print_hex((x, y))
                if self._map[x][y] is not None:
                    self._map[x][y].display(self._window)
                else:
                    self._add_ground((x, y))

        self._map_canvas.grid(row=1, column=0, rowspan=self._size[1], columnspan=self._size[0])

    def _place_log_frame(self, logs_frame):
        logs_frame.grid(rowspan=self._size[1], row=self._size[1]+1, column=1, padx=20)
