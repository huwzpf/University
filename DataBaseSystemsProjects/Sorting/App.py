import math
import random

from Sorter import Sorter
from File import File
from Record import Record
from Page import PAGE_BLOCKING_FACTOR
from Tape import Tape


class App:
    def __init__(self):
        self.file_to_sort = None
        self.sorted_file = None
        self.n_min = 0
        self.n_max = 1

    def get_theoretical_phases(self, runs):
        return math.log2(runs) * 1.45

    def get_theoretical_disk_ops(self, runs, N, b):
        return 2*(N/b)*(1.04 * math.log2(runs) + 1)

    def load_file(self, filename="sample_file"):
        self.file_to_sort = File(filename, read=True)

    def generate_random_file(self, length, filename="sample_file"):
        with open(filename, 'w') as file:
            for i in range(length):
                r = Record(random.uniform(self.n_min, self.n_max), random.uniform(self.n_min, self.n_max))
                file.write(f'{r.to_string()}\n')

        self.load_file(filename)

    def load_file_from_stdin(self, filename="sample_file"):
        load_single_record_cmd = 'l'
        add_rand_cmd = 'r'
        save_cmd = 's'
        quit_cmd = 'q'

        with open(filename, 'w') as file:
            while True:
                cmd = input(f'Type "{load_single_record_cmd}" to load single record from STDIN,'
                            f' "{add_rand_cmd}" to add random_record, "{save_cmd}" to save,'
                            f' "{quit_cmd}" to quit \n')

                if cmd == quit_cmd:
                    return False
                elif cmd == save_cmd:
                    self.load_file(filename)
                    return True
                elif cmd == load_single_record_cmd:
                    r = input("R = ")
                    h = input("H = ")
                    rec = Record(float(r), float(h))
                    file.write(f'{rec.to_string()}\n')
                elif cmd == add_rand_cmd:
                    rec = Record(random.uniform(self.n_min, self.n_max), random.uniform(self.n_min, self.n_max))
                    file.write(f'{rec.to_string()}\n')
                else:
                    print("Incorrect command")

    def cli_load(self):
        load_file_cmd = 'lf'
        generate_rand_file_cmd = 'gr'
        load_records_cmd = 'lr'
        quit_cmd = 'q'

        while True:
            cmd = input(f'Type "{load_file_cmd}" to load whole file,'
                        f' "{generate_rand_file_cmd}" to generate file with random records,'
                        f' "{load_records_cmd}" to read records from STDIN "{quit_cmd}" to quit \n')

            if cmd == quit_cmd:
                return False

            if cmd == generate_rand_file_cmd:
                length = input("File length to generate: ")
                self.generate_random_file(int(length))
                return True

            if cmd == load_file_cmd:
                name = input("File name: ")
                self.load_file(name)
                return True

            if cmd == load_records_cmd:
                return self.load_file_from_stdin()

            print("Incorrect command")

    def print_loaded_file(self):
        print('LOADED FILE: ')
        t = Tape(self.file_to_sort)
        t.start_read()
        rec = t.read()
        while rec:
            print(rec)
            rec = t.read()
        t.stop_read()
        self.file_to_sort.reset_read_count()

    def run_cli(self):
        quit_cmd = 'q'
        if not self.cli_load():
            return

        self.print_loaded_file()

        v = None
        s = Sorter(self.file_to_sort)

        while v is None:
            v = input("Run in verbose mode ? (y/n)")
            if v == quit_cmd:
                break
            elif v == 'y':
                self.sorted_file = s.sort(verbose=True)
                break
            elif v == 'n':
                self.sorted_file = s.sort(verbose=False)
                break
            else:
                v = None

        self.sorted_file.start_read()
        rec = self.sorted_file.read()
        while rec:
            print(rec)
            rec = self.sorted_file.read()

    def get_initial_runs(self):
        ret = 0
        t = Tape(self.file_to_sort)
        t.start_read()
        last_rec = t.read()
        rec = t.read()
        while rec:
            if rec < last_rec:
                ret += 1

            last_rec = rec
            rec = t.read()

        t.stop_read()
        self.file_to_sort.reset_read_count()
        return ret

    def get_stats(self, len):
        self.generate_random_file(length=len)
        runs = self.get_initial_runs()
        s = Sorter(self.file_to_sort)
        s.sort(runs_before_distribution=runs)
        theoretical_phases = self.get_theoretical_phases(s.initial_runs)
        real_phases = s.phase_counter
        theoretical_ops = self.get_theoretical_disk_ops(s.initial_runs, len, PAGE_BLOCKING_FACTOR)
        real_ops = s.overall_ops
        return {"phases": {"real": real_phases, "theoretical": theoretical_phases},
                "disk operations": {"real": real_ops, "theoretical": theoretical_ops}}




