def load_data(input_filename):
    data = []
    with open(input_filename, 'r') as input_file:
        for line in input_file:
            parts = line.strip().split(',')
            str1 = parts[0].strip(' "')
            str2 = parts[1].strip(' "')
            nums = [int(part.strip()) for part in parts[2:]]
            start = None
            end = None
            if len(nums) >= 2:
                start =  nums[0]
                end =  nums[1]
            elif len(nums) == 1:
                start =  nums[0]
            data.append((str1, str2, start, end))
    return data