import Constants
from Organism import Organism
from abc import ABC, abstractmethod, abstractproperty
import math

from Animal import Animal
from Sheep import Sheep
from Fox import Fox
from Wolf import Wolf
from Tortoise import Tortoise
from Antelope import Antelope
from CyberSheep import CyberSheep
from Human import Human
from MoveStruct import MoveStruct

from Plant import Plant
from Grass import Grass
from Dandelion import Dandelion
from Guarana import Guarana
from Wolfberry import Wolfberry
from Hogweed import Hogweed

import random
import tkinter as tk
from tkinter import messagebox
from CollisionResult import CollisionResult


class World(ABC):
    def __init__(self):
        self._root = tk.Tk()
        self._root.title("Piotr Sieński 184297")
        self._root_frame = None
        self._window = None
        self._ground_icon = tk.PhotoImage(file="ground.png")
        self._size = (0, 0)
        self._initial_organisms_count = 0
        self._organisms = list()
        self._activated_human_ability = False
        self.human_move = None
        self._turn_count = 0
        self._txt_logs = None
        gen_method = self._get_info()

        if gen_method.get() == "Generate":
            self._map = [[None for j in range(int(self._size[1]))] for i in range(int(self._size[0]))]
            self._populate_world(self._initial_organisms_count)
        else:
            self._load_world()

        self._display()

    # getters

    def is_hex(self):
        return self._is_hex

    def get_hogweeds_positions(self):  # return all hogeed's present on map positions in list
        hogweeds_list = list()
        for organism in self._organisms:
            if type(organism) is Hogweed:
                hogweeds_list.append(organism)
        return hogweeds_list

    def did_activate_human_ability(self):
        return self._activated_human_ability

    def get_map_field(self, position):
        return self._map[position[0]][position[1]]

    def get_turn(self):
        return self._turn_count

    def get_main_window(self):
        return self._window

    def get_size(self):
        return self._size

    def get_human_move_direction(self):
        return self.human_move

    def _validate_size(self, x, y, amount, radio_var, window):  # validate input provided in _get_info()
        flag = False
        if radio_var != "":
            if radio_var == "Generate":
                if x.isdigit() and y.isdigit() and amount.isdigit():
                    self._size = (int(x), int(y))
                    self._initial_organisms_count = int(amount)
                else:
                    flag = True
                    messagebox.showerror(title="Invalid input", message="Input is not a number")

            if flag is False:
                window.destroy()
                window.quit()

        else:
            messagebox.showerror(title="Invalid input", message="Select world generation method")

    def _get_info(self):
        window = tk.Toplevel(self._root)
        window.attributes('-topmost', True)

        radio_var = tk.StringVar()
        radio_load = tk.Radiobutton(master=window, text="Load", variable=radio_var,
                                    value="Load", tristatevalue=0)
        radio_load.pack()
        radio_generate = tk.Radiobutton(master=window, text="Generate", variable=radio_var,
                                        value="Generate", tristatevalue=0)
        radio_generate.pack()

        lbl_width = tk.Label(master=window, text="World width")
        lbl_width.pack()
        ent_width = tk.Entry(master=window)
        ent_width.pack()
        lbl_height = tk.Label(master=window, text="World height")
        lbl_height.pack()
        ent_height = tk.Entry(master=window)
        ent_height.pack()
        lbl_amount = tk.Label(master=window, text="Organisms amount")
        lbl_amount.pack()
        ent_amount = tk.Entry(master=window)
        ent_amount.pack()

        btn_accept = tk.Button(master=window, text="Continue",
                               command=lambda: self._validate_size(ent_width.get(), ent_height.get(),
                                                                   ent_amount.get(), radio_var.get(), window))
        btn_accept.pack()
        window.mainloop()
        return radio_var

    def _add_organism(self, organism, position):
        print("adding " + organism.to_string())
        self._organisms.append(organism)
        self._map[position[0]][position[1]] = organism

    def _runtime_adding(self, position, cls, window):
        instance = globals()[cls]
        organism = instance(position, self, self._turn_count)
        self._add_organism(organism, position)
        organism.display(self._window)
        window.destroy()

    def _display_adding_window(self, position):
        window = tk.Toplevel(self._root)
        window.attributes('-topmost', True)

        radio_var = tk.StringVar()

        for cls in Organism.__subclasses__():
            for sub in cls.__subclasses__():
                if sub != Human:
                    radio = tk.Radiobutton(master=window,  text=sub.__name__, variable=radio_var,
                                value=sub.__name__, tristatevalue=0)
                    radio.pack()

        btn_continue = tk.Button(master=window, text="Add",
                                 command=lambda: self._runtime_adding(position, radio_var.get(), window))
        btn_continue.pack()

    @abstractmethod
    def _add_ground(self, position):
        pass

    def _delete_organism(self, organism):
        pos = organism.get_position()
        self._map[pos[0]][pos[1]].icon_button.destroy()

        self._add_ground(pos)
        self._organisms.remove(organism)
        del organism

    @abstractmethod
    def _find_free_adjacent(self, position, radius, search_wider):
        pass

    def _save_world(self):
        file = open("save.txt", "wt")
        file.write(str(len(self._organisms)) + "\n")
        file.write(str(self._size) + "\n")
        for organism in self._organisms:
            file.write(str(organism._position) + " " + str(type(organism).__name__) + "\n")

        file.close()

    def _load_world(self):
        file = open("save.txt", "rt")
        count = int(file.readline())
        size_line = file.readline()
        # offset is set in order to correctly read 2 digit size
        offset = 0
        if size_line[2] != ',':
            offset += 1
        x = size_line[1:2+offset]

        if size_line[5+offset] != ')':
            y = size_line[4+offset:5+offset+1]
        else:
            y = size_line[4 + offset]

        self._size = (int(x), int(y))

        self._map = [[None for j in range(int(self._size[1]))] for i in range(int(self._size[0]))]

        offset = 0
        for i in range(count):
            o = file.readline()
            # format of data is always (x_pos, y_pos) class name       \n
            #                           ^       ^     ^                 ^
            #                         o[1]     o[4]   begins with o[7]  o[-1]
            if o[2] == ',':
                x_pos = int(o[1])
            else:
                x_pos = int(o[1:2])
                offset += 1

            if o[5] == ')':
                y_pos = int(o[4+offset])
            else:
                y_pos = int(o[4+offset:5+offset])
                offset += 1

            position = (x_pos, y_pos)
            cls = o[7+offset:-1]
            instance = globals()[cls]
            self._add_organism(instance(position, self, self._turn_count), position)
            offset = 0
        file.close()

    def _populate_world(self, n):
        # add
        human = Human((0, 0), self, self._turn_count)
        self._add_organism(human, (0, 0))
        probabilities_sum = 0
        # iterate through all existing organism subclasses and count sum of probablilities of generating each one
        for cls in Organism.__subclasses__():
            for sub in cls.__subclasses__():
                probabilities_sum += sub.generation_probability
        for i in range(n):
            self._generate_random_organism(probabilities_sum)

    def _generate_random_organism(self, probabilities_sum):
        found = False
        count = 0
        # get random number in range 0 - sum of all generation probabilities
        # and decide which organism should be added based on that random number
        rand = random.randint(0, probabilities_sum)
        for cls in Organism.__subclasses__():
            for sub in cls.__subclasses__():
                count += sub.generation_probability

                if count >= rand:
                    self._add_at_random_pos(sub)
                    found = True
                    break

            if found is True:
                break

    def _add_at_random_pos(self, organism_class):
        x = random.randint(0, (self._size[0] - 1))
        y = random.randint(0, (self._size[1] - 1))
        if self._map[x][y] is None:
            self._add_organism(organism_class((x, y), self, self._turn_count), (x, y))

        else:
            possible_pos = self._find_free_adjacent((x, y), 1, True)
            if possible_pos is not None:
                self._add_organism(organism_class(possible_pos, self, self._turn_count), possible_pos)
            else:
                print("no free space")

    def _handle_move(self, organism, new_pos):
        # handle move and collisions occuring after this move
        self.log(organism.to_string() + " moves to " + str(new_pos))

        if self._map[new_pos[0]][new_pos[1]] is None:
            self._move_organism(new_pos, organism)
        else:
            collision_result = self._map[new_pos[0]][new_pos[1]].collision(organism)

            if collision_result == CollisionResult.DIED:
                self.handle_collision_death(new_pos, organism)

            elif collision_result == CollisionResult.KILLED_OPPONENT:
                self._handle_collision_kill(new_pos, organism)

            elif collision_result == CollisionResult.MULTIPLIED and organism.can_breed():
                self._handle_collision_multilpification(new_pos, organism)

            elif collision_result == CollisionResult.ESCAPED:
                self._handle_collision_escape(new_pos, organism)

            elif collision_result == CollisionResult.OPPONENT_ESCAPED:
                self._handle_collision_attacker_escape(new_pos, organism)

            elif collision_result == CollisionResult.BOTH_DIED:
                self._handle_collision_death_both(new_pos, organism)

            else:
                pass

    def _handle_collision_attacker_escape(self, new_pos, organism):
        self.log(self._map[new_pos[0]][new_pos[1]].to_string() + " scares off opponent "
                 + organism.to_string())

    def _handle_collision_death_both(self, new_pos, organism):
        self.log(organism.to_string() + " ate " + self._map[new_pos[0]][new_pos[1]].to_string() + " and died")
        self._delete_organism(organism)
        self._delete_organism(self._map[new_pos[0]][new_pos[1]])

    def _handle_collision_escape(self, new_pos, organism):
        possible_pos = self._find_free_adjacent(new_pos, 1, False)
        if possible_pos is not None:
            self.log(organism.to_string() + "moves to " + str(new_pos) + " and "
                     + self._map[new_pos[0]][new_pos[1]].to_string() + " escapes to " + str(possible_pos))
            self._move_organism(possible_pos, self._map[new_pos[0]][new_pos[1]])
            self._move_organism(new_pos, organism)
        else:
            self.handle_collision_death(new_pos, organism)

    def _handle_collision_multilpification(self, new_pos, organism):
        self.log(self._map[new_pos[0]][new_pos[1]].to_string() + " breeds with "
                 + organism.to_string())
        self._map[new_pos[0]][new_pos[1]].set_on_cooldown()
        self._multiply_organisms(organism)

    def _handle_collision_kill(self, new_pos, organism):
        self.log(self._map[new_pos[0]][new_pos[1]].to_string() + " defends itself and kills "
                 + organism.to_string())
        self._delete_organism(organism)

    def handle_collision_death(self, new_pos, organism):
        self.log(organism.to_string() + " kills " + self._map[new_pos[0]][new_pos[1]].to_string())
        self._delete_organism(self._map[new_pos[0]][new_pos[1]])
        self._move_organism(new_pos, organism)

    def _move_organism(self, new_pos, organism):
        self._map[new_pos[0]][new_pos[1]] = organism
        organism.icon_button.destroy()
        # to_del = self._window.grid_slaves(column=new_pos[0], row=(new_pos[1]+1))[0]
        # to_del.destroy()
        self._add_ground(organism._position)
        organism._position = new_pos
        organism.display(self._window)

    def _set_human_move(self, direction):
        self.human_move = direction
        self._update()

    def log(self, txt):
        self._txt_logs.config(state="normal")
        self._txt_logs.insert("0.0", str(txt) + "\n")
        self._txt_logs.config(state="disabled")

    def _multiply_organisms(self, organism):
        pos = self._find_free_adjacent(organism.get_position(), 1, False)
        if pos is not None:
            instance = type(organism)(pos, self, self._turn_count)
            self._add_organism(instance, pos)
            instance.display(self._window)
        else:
            self.log("there is no free place for new organism to be created")

    def _update(self):
        self._organisms.sort(key=lambda o: (o.get_initiative(), -o.get_creation_turn()), reverse=True)
        self._turn_count += 1
        self._update_turn_display()

        self._update_loop()

        self._activated_human_ability = False
        self.log("---\n" + "Turn number " + str(self._turn_count))

    def _update_loop(self):
        result = None
        for organism in self._organisms:
            result = organism.action()
            if isinstance(organism, Animal) and type(result) is tuple:
                self._handle_move(organism, result)

            elif isinstance(organism, Plant):
                if type(result) is tuple:
                    if organism.is_poisonous():
                        poison_radius = result[1]
                        self._spread_poison(organism, poison_radius)

                    result = result[0]

                for i in range(result):
                    self.log(type(organism).__name__ + " spread")
                    self._multiply_organisms(organism)

    def _update_turn_display(self):
        info_str = str(self._turn_count) + " turn"
        lbl_info = tk.Label(master=self._window, text=info_str)
        lbl_info.grid(row=0, column=0)

    @abstractmethod
    def _can_search_for_existing_organism(self, pos, x, y, x_min, x_max, y_min, y_max):
        # predicate function needed for spreading poison at every adjacent field
        pass

    def _spread_poison(self, organism, poison_radius):
        pos = organism.get_position()
        for x in range(pos[0] - poison_radius, pos[0] + poison_radius):
            for y in range(pos[1] - poison_radius, pos[1] + poison_radius):
                if self._can_search_for_existing_organism(pos,x, y, pos[0] - poison_radius, pos[0] + poison_radius,
                                                          pos[1] - poison_radius, pos[1] + poison_radius):
                    if type(self._map[x][y]) is not CyberSheep:
                        self.log(organism.to_string() + " poisoned " + self._map[x][y].to_string())
                        self._delete_organism(self._map[x][y])

    def _activate_human_ability(self):
        self._activated_human_ability = True

    def _display(self):

        self._crate_main_window()

        self._display_map()

        self._add_movement_binds()

        self._add_logs_frame()

        self._root_frame.pack(side="left", fill="both", expand=1)

        self._window.configure(width=self._root.winfo_reqwidth())

        self._root.mainloop()

    def _crate_main_window(self):
        self._root.geometry("800x800")
        self._root_frame = tk.Frame(self._root)
        self._root_frame.pack(fill="both", expand=True)
        canvas = tk.Canvas(self._root_frame)
        canvas.pack(side="left", fill="both", expand=True)
        scrollbar_x = tk.Scrollbar(self._root_frame, orient="horizontal", command=canvas.xview)
        scrollbar_x.pack(side="top", fill="none")
        scrollbar_y = tk.Scrollbar(self._root_frame, orient="vertical", command=canvas.yview)
        scrollbar_y.pack(side="right", fill="y")
        canvas.configure(yscrollcommand=scrollbar_y.set)
        canvas.configure(xscrollcommand=scrollbar_x.set)
        canvas.configure(scrollregion=canvas.bbox("all"))
        canvas.bind('<Configure>', lambda e: canvas.configure(scrollregion=canvas.bbox("all")))
        self._window = tk.Frame(canvas)
        canvas.create_window((0, 0), window=self._window, anchor="w")
        self._root.attributes('-topmost', True)

    @abstractmethod
    def _display_map(self):
        pass

    def _add_movement_binds(self):
        self._root.bind('<Left>', lambda event: self._set_human_move(MoveStruct(0)))
        self._root.bind('<Right>', lambda event: self._set_human_move(MoveStruct(1)))
        self._root.bind('<Up>', lambda event: self._set_human_move(MoveStruct(2)))
        self._root.bind('<Down>', lambda event: self._set_human_move(MoveStruct(3)))
        self._root.bind('x', lambda event: self._activate_human_ability())

    def _add_logs_frame(self):
        logs_frame = tk.Frame(self._window)
        logs_scroll = tk.Scrollbar(logs_frame)
        logs_scroll.pack(side="right", fill="y")
        self._txt_logs = tk.Text(master=logs_frame, wrap="word", width=50,
                                 height=self._size[1] * int(Constants.ICON_SIZE / 16))
        # 16 is approximate size of one text row
        self._txt_logs.pack(side="left", fill="both")
        self._place_log_frame(logs_frame)
        self._txt_logs.insert("0.0", "GAME STARTS \n")
        self._txt_logs.config(state="disabled")
        logs_scroll.config(command=self._txt_logs.yview)

    def _place_log_frame(self, logs_frame):
        logs_frame.grid(rowspan=self._size[1], row=1, column=self._size[0], padx=20)





