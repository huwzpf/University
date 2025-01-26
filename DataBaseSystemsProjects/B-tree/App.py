import random
import matplotlib.pyplot as plt
from Tree import Tree
import numpy as np
from Record import Record


class App:
    def __init__(self, d=2):
        self.tree = Tree(d)

    def run_in_interactive_mode(self):
        quit_cmd = "q"
        insert_cmd = "i"
        delete_cmd = "d"
        update_cmd = "u"
        reorganize_cmd = "r"
        visualize_cmd = "v"
        print_cmd = "p"
        generate_random_cmd = "g"

        while True:
            cmd = input(f'Type {insert_cmd} to insert data, {delete_cmd} to delete data, {update_cmd} to update data,'
                        f' {reorganize_cmd} to reorganize files, {visualize_cmd} to print content of both files,'
                        f' {print_cmd} to print stored data, {generate_random_cmd} to generate random records,'
                        f' {quit_cmd} to quit\n')
            if cmd == insert_cmd:
                idx = input("Index: ")
                r = input("Radius: ")
                h = input("Height: ")
                self.tree.add_data(Record(index=int(idx), radius=float(r), height=float(h)))
            elif cmd == delete_cmd:
                idx = input("Index: ")
                self.tree.delete(int(idx))
            elif cmd == update_cmd:
                idx = input("Index: ")
                r = input("Radius: ")
                h = input("Height: ")
                self.tree.update_data(Record(index=int(idx), radius=float(r), height=float(h)))
            elif cmd == reorganize_cmd:
                self.tree.reorganize()
            elif cmd == visualize_cmd:
                self.tree.visualize()
            elif cmd == print_cmd:
                self.tree.print_data()
            elif cmd == generate_random_cmd:
                n = input("N: ")
                self.generate_random_tree(int(n))
            elif cmd == quit_cmd:
                break
            else:
                print("INCORRECT COMMAND")

    def run_in_load_mode(self):
        insert_cmd = "i"
        delete_cmd = "d"
        update_cmd = "u"
        reorganize_cmd = "r"
        with open("test.txt", "r") as file:
            for line in file:
                cmd = []
                for word in line.split():
                    cmd.append(word)
                if cmd[0] == insert_cmd:
                    if len(cmd) != 4:
                        print("INCORRECT COMMAND")
                    else:
                        idx = cmd[1]
                        r = cmd[2]
                        h = cmd[3]
                        self.tree.add_data(Record(index=int(idx), radius=float(r), height=float(h)))
                elif cmd[0] == delete_cmd:
                    if len(cmd) != 2:
                        print("INCORRECT COMMAND")
                    else:
                        idx = cmd[1]
                        self.tree.delete(int(idx))
                elif cmd[0] == update_cmd:
                    if len(cmd) != 4:
                        print("INCORRECT COMMAND")
                    else:
                        idx = cmd[1]
                        r = cmd[2]
                        h = cmd[3]
                        self.tree.update_data(Record(index=int(idx), radius=float(r), height=float(h)))
                elif cmd[0] == reorganize_cmd:
                    self.tree.reorganize()
                else:
                    print("INCORRECT COMMAND")
        self.tree.visualize()
        print("\n\n\n")
        self.tree.print_data()


    def run_cli(self):
        cmd = self.load_cli()
        if cmd is None:
            return

        if cmd == "interactive":
            self.run_in_interactive_mode()
        elif cmd == "load":
            self.run_in_load_mode()

    def load_cli(self):
        interactive_mode_cmd = 'i'
        load_cmd = 'l'
        quit_cmd = 'q'
        exp_cmd = 'e'

        while True:
            cmd = input(f'Type "{interactive_mode_cmd}" to start in interactive mode,'
                        f' "{load_cmd}" to load data from file, {exp_cmd} to run experiment'
                        f' "{quit_cmd}" to quit \n')

            if cmd == quit_cmd:
                return None

            if cmd == interactive_mode_cmd:
                return "interactive"

            if cmd == load_cmd:
                return "load"

            if cmd == exp_cmd:
                self.exp()
                return None

            print("INCORRECT COMMAND")

    def generate_random_tree(self, n, verbose=True):
        rand_range = range(1, n * 10)
        keys = random.sample(rand_range, n + 1)
        for idx in keys[:-1]:
            self.tree.add_data(Record(index=idx, height=1, radius=1), verbose)
        return {"existing": random.choice(keys), "not-existing": keys[-1]}

    def print_results1(self, results:dict):
        for n in results.keys():
            d_inserts = []
            d_deletions = []
            d_nums = []
            for d in results[n].keys():
                a_inserts = []
                a_deletes = []
                a_alphas = []
                for result in results[n][d].values():
                    a_inserts.append(result["insert"])
                    a_deletes.append(result["delete"])
                    a_alphas.append(result["alpha"])

                fig, ax = plt.subplots(2, 1)
                b1, a1 = np.polyfit(a_alphas, a_inserts, deg=1)
                b2, a2 = np.polyfit(a_alphas, a_deletes, deg=1)
                print(f'PARAMS for d = {d} n = {n}')
                print(f'a1 = {a1}, b1 = {b1}, a2 = {a2}, b2 = {b2}')
                xseq = np.linspace(min(a_alphas), max(a_alphas), num=100)
                ax[0].scatter(a_alphas, a_inserts)
                ax[0].plot(xseq, a1 + b1 * xseq)
                ax[0].set_title("Insertions")
                ax[0].set_ylabel("disk operations")

                ax[1].scatter(a_alphas, a_deletes)
                ax[1].plot(xseq, a2 + b2 * xseq)
                ax[1].set_title("Deletions")
                ax[1].set_xlabel("alpha value")
                ax[1].set_ylabel("disk operations")
                fig.suptitle(f'D = {d}, n = {n} - Disk operations(alpha)')
                plt.subplots_adjust(hspace=0.4)
                plt.savefig(f'Ad{d}n{n}.png')

                d_nums.append(d)
                d_deletions.append(np.mean(a_deletes))
                d_inserts.append(np.mean(a_inserts))

            fig, ax = plt.subplots(2, 1)
            b1, a1 = np.polyfit(d_nums, d_inserts, deg=1)
            b2, a2 = np.polyfit(d_nums, d_deletions, deg=1)
            xseq = np.linspace(min(d_nums), max(d_nums), num=100)
            ax[0].scatter(d_nums, d_inserts)
            ax[0].plot(xseq, a1 + b1 * xseq)
            ax[0].set_title("Insertions")
            ax[0].set_ylabel("disk operations")

            ax[1].scatter(d_nums, d_deletions)
            ax[1].plot(xseq, a2 + b2 * xseq)
            ax[1].set_title("Deletions")
            ax[1].set_xlabel("d value")
            ax[1].set_ylabel("disk operations")
            fig.suptitle(f'n = {n} - Disk operations(D)')
            plt.subplots_adjust(hspace=0.4)
            plt.savefig(f'Dn{n}.png')


    def exp(self):
        n_vals = [200, 800]
        d_vals = [2,4, 6, 8, 10]
        results = {}
        iters = 1000
        for n in n_vals:
            results[n] = {}
            for d in d_vals:
                results[n][d] = {}
                for i in range(iters):
                    self.tree = Tree(d)
                    data = self.generate_random_tree(n, False)
                    alpha = self.tree.get_alpha()
                    file_ops_before = self.tree.get_file_ops()
                    self.tree.add_data(Record(index=data["not-existing"], radius=1, height=1), False)
                    insert = self.tree.compare_file_ops(file_ops_before, False)
                    file_ops_before = self.tree.get_file_ops()
                    self.tree.delete(data["existing"], False)
                    delete = self.tree.compare_file_ops(file_ops_before, False)
                    results[n][d][i] = {"insert": insert, "delete": delete, "alpha": alpha}
                print(f'D = {d}, N ={n} DONE')

        self.print_results1(results)

