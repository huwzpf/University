from Tape import *
from FibGen import FibGen


def remove_dummy_runs(long_tape, output_tape, short_tape):
    target_runs = short_tape.get_runs() - long_tape.dummy_runs
    while short_tape.get_runs() != target_runs:
        record = short_tape.read(True)
        output_tape.write(record)
    long_tape.dummy_runs = 0


class Sorter:
    def __init__(self, f: File):
        n_tapes = 3

        self.sorted_tape = None
        self.file = f
        self.tapes = [Tape(File(f'tape{i}', False)) for i in range(n_tapes)]
        self.phase_counter = 0
        self.read_counter = 0
        self. write_counter = 0
        self.initial_runs = 0
        self.overall_ops = 0

    def switch_tapes(self, current_tape):
        if current_tape == self.tapes[1]:
            return self.tapes[2]
        else:
            return self.tapes[1]

    def distribute(self):
        fib_gen = FibGen()

        fib_num = next(fib_gen)
        prev_rec = Record(0, 0)

        input_tape = Tape(self.file)
        input_tape.start_read()
        record = input_tape.read()

        self.tapes[1].start_write()
        self.tapes[2].start_write()

        current_tape = self.tapes[1]

        while record:
            if prev_rec > record:
                if current_tape.get_runs() == fib_num:
                    current_tape = self.switch_tapes(current_tape)
                    fib_num = next(fib_gen)

            current_tape.write(record)
            prev_rec = record
            record = input_tape.read()

        self.tapes[1].end_write()
        self.tapes[2].end_write()

        if current_tape.get_runs() != fib_num:
            current_tape.dummy_runs = fib_num - current_tape.get_runs()

        self.read_counter += self.file.get_read_count()

    def print_tapes(self):
        for i, tape in enumerate(self.tapes):
            tape_reads = tape.get_read_count()
            tape_writes = tape.get_write_count()

            print(f'Tape{i}: ')
            print(f'RUNS: {tape.get_runs()}, DUMMY RUNS: {tape.dummy_runs}')
            print(f'WRITES: {tape_writes}, READS: {tape_reads}')
            tape.start_read()
            rec = tape.read()
            while rec:
                print(rec)
                rec = tape.read()
            tape.stop_read()
            tape._file._read_count = tape_reads
            tape._file._write_count = tape_writes

    def sort(self, verbose=False, runs_before_distribution=None):
        self.initial_runs = runs_before_distribution
        self.distribute()
        if self.initial_runs is None:
            self.initial_runs = sum([tape.get_runs() for tape in self.tapes])
        print(f'Initial runs: {self.initial_runs}')

        if verbose:
            print('AFTER DISTRIBUTION')
            self.print_tapes()

        do_next_iter = True
        while do_next_iter:
            do_next_iter = self.merge()
            if verbose:
                print(f'AFTER PHASE: {self.phase_counter}')
                self.print_tapes()
            self.phase_counter += 1

        for tape in self.tapes:
            self.write_counter += tape.get_write_count()
            self.read_counter += tape.get_read_count()

        print(f'File sorted in {self.phase_counter} phases')

        self.overall_ops = self.write_counter + self.read_counter

        print(f'WRITES: {self.write_counter} + READS: {self.read_counter} = {self.overall_ops}')

        return self.sorted_tape

    def merge(self):
        self.tapes.sort(key=lambda tape: tape.get_runs() + tape.dummy_runs)
        output_tape = self.tapes[0]
        short_tape = self.tapes[1]
        long_tape = self.tapes[2]

        if long_tape.get_runs() == 1 and short_tape.get_runs() == 0:
            self.sorted_tape = long_tape
            return False

        short_tape.start_read()
        long_tape.start_read()
        output_tape.start_write()

        if long_tape.dummy_runs > 0:
            remove_dummy_runs(long_tape, output_tape, short_tape)

        short_tape_runs = short_tape.get_runs()
        long_tape_runs = long_tape.get_runs()

        short_tape_record = None
        long_tape_record = None

        while short_tape.get_runs() != 0:
            while True:
                if short_tape_record is None:
                    short_tape_record = short_tape.read(True)
                if long_tape_record is None:
                    long_tape_record = long_tape.read(True)

                if short_tape_record < long_tape_record:
                    output_tape.write(short_tape_record)
                    short_tape_record = None
                else:
                    output_tape.write(long_tape_record)
                    long_tape_record = None

                if (short_tape.get_runs() == short_tape_runs - 1 and short_tape_record is None) or \
                        (long_tape.get_runs() == long_tape_runs - 1 and long_tape_record is None):
                    break

            if long_tape_record is None:
                output_tape.write(short_tape_record)
                while short_tape.get_runs() != short_tape_runs - 1:
                    short_tape_record = short_tape.read(True)
                    output_tape.write(short_tape_record)

                short_tape_record = None
            else:
                output_tape.write(long_tape_record)
                while long_tape.get_runs() != long_tape_runs - 1:
                    long_tape_record = long_tape.read(True)
                    output_tape.write(long_tape_record)
                long_tape_record = None

            short_tape_runs = short_tape.get_runs()
            long_tape_runs = long_tape.get_runs()

        output_tape.end_write()
        long_tape.stop_read()
        short_tape.stop_read()

        if output_tape.get_runs() > 1 or long_tape.get_runs() > 0:
            return True
        else:
            self.sorted_tape = output_tape
            return False