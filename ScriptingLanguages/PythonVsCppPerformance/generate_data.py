import random
import string

def generate_random_string(length):
    """Generate a random string of specified length."""
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

if __name__ == "__main__":
    num_samples = 10000
    search_space_restriction_probability = 0.1
    max_substring_length = 3
    filename = "input.txt"

    samples = []
    
    for _ in range(num_samples):
        string_length = random.randint(100, 1000)  
        substring_length = random.randint(1, max_substring_length)
        main_string = generate_random_string(string_length)
        search_string = generate_random_string(substring_length)

        if random.random() < search_space_restriction_probability:       
            end = random.randint(1, substring_length)
            start = random.randint(0, end)
            samples.append(f"\"{main_string}\",\"{search_string}\",{start},{end}\n")
        else:
            samples.append(f"\"{main_string}\",\"{search_string}\"\n")
    
    with open(filename, 'w') as f:
        for sample in list(set(samples)):
            f.write(sample)
            