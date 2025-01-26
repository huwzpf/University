def custom_rfind(s, sub, start=None, end=None):
    # If start is None, set it to beggining of the string
    if start is None:
        start = 0

    # If end is None, set it to the length of the string
    if end is None or end > len(s):
        end = len(s)

    # Adjust negative indices
    if start < 0:
        start = max(0, len(s) + start)
    if end < 0:
        end = max(0, len(s) + end)

    # Check if search space is valid
    search_space_len = end - start
    if search_space_len < 0:
        return -1

    # Check if the substring is longer than the search space
    sub_len = len(sub)
    if sub_len == 0:
        return end
    if sub_len > search_space_len:
        return -1

    # Iterate over the string in reverse within the specified range
    for i in range(end - sub_len, start - 1, -1):
        if s[i:i+sub_len] == sub:
            return i
    return -1
