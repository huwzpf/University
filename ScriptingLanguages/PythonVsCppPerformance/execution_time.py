from rfind import custom_rfind
from load_data import load_data
import time


def get_execution_time(n, data, function):
    total_duration_ns = 0
    for _ in range(n):
        for str1, str2, start, end in data:
            start_time = time.perf_counter_ns()
            ret = function(str1, str2, start, end)
            end_time = time.perf_counter_ns()
            total_duration_ns += (end_time - start_time)
    return total_duration_ns

def get_clock_precision():
    # based on https://peps.python.org/pep-0564/
    LOOPS = 10 ** 6
    min_dt = [abs(time.perf_counter_ns() - time.perf_counter_ns()) for _ in range(LOOPS)]
    min_dt = min(filter(bool, min_dt))
    return min_dt


if __name__ == "__main__":
    input_filename = "input.txt"
    clock_precision_ns = get_clock_precision()
    
    print(f'Clock precision: {clock_precision_ns} nanoseconds')
    data = load_data(input_filename)
    for n in [1, 10, 100, 1000]:
        custom_time = get_execution_time(n, data, custom_rfind)
        custom_relative_error = (clock_precision_ns * len(data)) / custom_time;
        print(f'Custom Python function - N = {n} - Total execution time = {custom_time} ns - Relative error = {custom_relative_error}')
        builtin_time = get_execution_time(n, data, lambda s1, s2, start, end: s1.rfind(s2, start, end))
        builtin_relative_error = (clock_precision_ns * len(data)) / builtin_time;
        print(f'Builtin function - N = {n} - Total execution time = {builtin_time} ns - Relative error = {builtin_relative_error}')