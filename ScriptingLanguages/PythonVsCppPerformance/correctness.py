from rfind import custom_rfind
from load_data import load_data
import os

def check_correctness(input_filename, output_filename, function):
    data = load_data(input_filename)
    with open(output_filename, 'w') as output_file:
        for str1, str2, start, end in data:
            ret = function(str1, str2, start, end)
            output_file.write(str(ret) + '\n')

if __name__ == "__main__":
    input_filename = "input.txt"
    custom_output_filename = "output-python-custom.txt"
    builtin_output_filename = "output-python-builtin.txt"
    cpp_output_filename = "output-cpp-custom.txt"
    check_correctness(input_filename, custom_output_filename, custom_rfind)
    check_correctness(input_filename, builtin_output_filename, lambda s1, s2, start, end: s1.rfind(s2, start, end))
    if os.path.exists(cpp_output_filename):
        with open(custom_output_filename, 'rb') as f1, open(builtin_output_filename, 'rb') as f2, open(cpp_output_filename, 'rb') as f3:
            content1 = f1.read()
            content2 = f2.read()
            content3 = f3.read()
            
            if content1 == content2 == content3:
                print('All files have exactly the same contents')
            else:
                print('All files DO NOT have exactly the same contents')