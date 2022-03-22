from GridWorld import GridWorld
from HexWorld import HexWorld
import tkinter as tk


def start(t, window):
    window.quit()
    window.destroy()
    if t == "Hex":
        HexWorld()
    else:
        GridWorld()


def display_start_window():
    win = tk.Tk()
    label = tk.Label(master=win, text="Choose world type")
    btn_hex = tk.Button(master=win, text="Start Hex World", command=lambda: start("Hex", win))
    btn_grid = tk.Button(master=win, text="Start Grid World", command=lambda: start("Grid", win))
    label.pack()
    btn_hex.pack()
    btn_grid.pack()
    win.mainloop()


if __name__ == '__main__':
    display_start_window()
