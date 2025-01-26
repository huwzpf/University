from tkinter import *
from tkinter import filedialog, messagebox
import PIL
from PIL import Image, ImageTk
from ecb import ECB
from cbc import CBC
from ctr import CTR
from cfb import CFB
from ofb import OFB
import os
from image_loader import evaluate_benchmark
import matplotlib.pyplot as plt
import numpy as np

P1_X = 100  # First Image X
P2_X = 350  # Second Image X
P_Y = 50  # Image Y
RES = 150  # Resize
MENU_Y = 375  # Menu Labels Y
ACTION_X = 50  # Action Menu X
MODE_X = 450  # Mode Menu X
OFFSET = 50  # Margin


class GUI(Tk):
    def __init__(self):
        super().__init__()
        self.title("Temat 14 - Kryptografia Symetryczna")
        self.width = 600
        self.height = 700
        self.geometry(f"{self.width}x{self.height}")
        self.resizable(False, False)
        icon = PhotoImage(file='data/icon.png')
        self.iconphoto(False, icon)
        self.configure(bg='#ede8b6')
        self.path = None
        self.benchmark_enc = None
        self.benchmark_dec = None

        # Default Images and  buttons
        self.open_img("data/blank.png", P1_X, P_Y)
        self.open_img("data/blank.png", P2_X, P_Y)
        Button(text="Choose file", command=self.open_img_dialog, width=12).place(x=255, y=250)
        Button(text="Run", command=self.run, width=12).place(x=255, y=300)
        Label(text="Simulate error (%)").place(x=250, y=MENU_Y)
        self.error = Scale(from_=0, to=100, orient=HORIZONTAL, width=10)
        self.error.place(x=250, y=425)
        self.visualize = IntVar()
        Label(text="Visualize").place(x=250, y=MENU_Y + 125)
        c = Checkbutton(text="Visualize", variable=self.visualize, onvalue=1, offvalue=0)
        c.place(x=250, y=MENU_Y + 150)
        c.select()
        Label(text="Benchmark").place(x=250, y=MENU_Y + 200)
        Button(text="Run Benchmark", command=self.run_benchmark, width=12).place(x=250, y=MENU_Y + 225)

        # Action and Mode Menu
        self.action = StringVar()
        self.mode = StringVar()
        self.r = []
        Label(text="Choose Action: ").place(x=50, y=MENU_Y)
        self.r.append(Radiobutton(text="Encrypt", variable=self.action, value="encrypt", tristatevalue=0))
        self.r.append(Radiobutton(text="Decrypt", variable=self.action, value="decrypt", tristatevalue=0))
        Label(text="Choose mode: ").place(x=450, y=MENU_Y)
        self.r.append(Radiobutton(text="ECB", variable=self.mode, value="ecb", tristatevalue=0))
        self.r.append(Radiobutton(text="CBC", variable=self.mode, value="cbc", tristatevalue=0))
        self.r.append(Radiobutton(text="CFB", variable=self.mode, value="cfb", tristatevalue=0))
        self.r.append(Radiobutton(text="OFB", variable=self.mode, value="ofb", tristatevalue=0))
        self.r.append(Radiobutton(text="CTR", variable=self.mode, value="ctr", tristatevalue=0))
        self.init_radio()

    # Helper function, open dialog and return filepath
    @staticmethod
    def open_file():
        filename = filedialog.askopenfilename(title='Open')
        return filename

    # Get path of Image from dialog window and put it in default location
    def open_img_dialog(self):
        self.path = self.open_file()
        if self.visualize.get() == 1:
            self.open_img(self.path, P1_X, P_Y)
        else:
            self.open_img("data/blank.png", P1_X, P_Y)
        self.open_img("data/blank.png", P2_X, P_Y)

    # Put Image in custom location with provided path
    def open_img(self, path, x, y):
        img = PIL.Image.open(path)
        img = img.resize((RES, RES), PIL.Image.ANTIALIAS)
        img = ImageTk.PhotoImage(img)
        panel = Label(self, image=img)
        panel.image = img
        panel.place(x=x, y=y)

    # Place and deselect all radio buttons
    def init_radio(self):
        y_mode = MENU_Y + OFFSET
        y_action = MENU_Y + OFFSET
        for i in range(2):
            self.r[i].place(x=ACTION_X, y=y_action)
            y_action += OFFSET
            self.r[i].deselect()
        for i in range(2, 7):
            self.r[i].place(x=MODE_X, y=y_mode)
            y_mode += OFFSET
            self.r[i].deselect()

    def run(self):
        mode = None
        # Set up mode
        if self.mode.get() == "ecb":
            mode = ECB()
        elif self.mode.get() == "cbc":
            mode = CBC()
        elif self.mode.get() == "ctr":
            mode = CTR()
        elif self.mode.get() == "cfb":
            mode = CFB()
        elif self.mode.get() == "ofb":
            mode = OFB()
        if mode is None:
            messagebox.showerror("Error", "You have to choose a mode.")
            exit()
        # Make sure file is chosen
        if self.path is None:
            messagebox.showerror("Error", "You have to choose a file.")
            exit()
        # Set up action and run it
        if self.action.get() == "encrypt":
            mode.run_encryption(self.path, self.error.get())
            new_path = os.path.splitext(self.path)[0] + "-enc" + os.path.splitext(self.path)[1]
            if self.visualize.get() == 1:
                self.open_img(new_path, P2_X, P_Y)
            else:
                self.open_img("data/blank.png", P2_X, P_Y)
        elif self.action.get() == "decrypt":
            mode.run_decryption(self.path, self.error.get())
            new_path = os.path.splitext(self.path)[0] + "-dec" + os.path.splitext(self.path)[1]
            if self.visualize.get() == 1:
                self.open_img(new_path, P2_X, P_Y)
            else:
                self.open_img("data/blank.png", P2_X, P_Y)
        else:
            messagebox.showerror("Error", "You have to choose an action.")
            exit()

    def run_benchmark(self):
        self.path = "data/tux.bmp"
        self.benchmark_dec = [[], [], [], [], [], []]
        self.benchmark_enc = [[], [], [], [], [], []]
        labels = ["ecb","cbc","ctr","cfb","ofb"]
        modes = [ECB(), CBC(), CTR(), CFB(), OFB()]
        for i in range(100):
            idx = 0
            for m in modes:
                m.run_encryption(self.path, 0)
                m.run_decryption("data/tux-enc.bmp", i)
                self.benchmark_dec[idx].append(evaluate_benchmark("data/tux-enc-dec.bmp"))
                idx += 1

        for i in range(100):
            idx = 0
            for m in modes:
                m.run_encryption(self.path, i)
                m.run_decryption("data/tux-enc.bmp", 0)
                self.benchmark_enc[idx].append(evaluate_benchmark("data/tux-enc-dec.bmp"))
                idx += 1

        x = np.linspace(0, 100, num=100)
        for i in range(len(modes)):
            plt.plot(x, self.benchmark_dec[i],label=labels[i])
        plt.legend(labels)
        plt.title("Przekłamane przy deszyfracji")
        plt.xlabel("Stopeiń przekłamania (%)")
        plt.ylabel("Ilość przekłamanych pikseli")
        plt.savefig("data/benchmark_dec.png")
        plt.show()

        for i in range(len(modes)):
            plt.plot(x, self.benchmark_enc[i], label=labels[i])
        plt.legend(labels)
        plt.title("Przekłamanie przy szyfrowaniu")
        plt.xlabel("Stopeiń przekłamania (%)")
        plt.ylabel("Ilość przekłamanych pikseli")
        plt.savefig("data/benchmark_enc.png")
        plt.show()


    # Run GUI
    def loop(self):
        self.mainloop()
