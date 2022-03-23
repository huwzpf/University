def variation_without_repetition(head, tail, n=0):
    if n == 0:
        return [[head]]
    else:
        ret = []
        for i in range(len(tail)):
            ret += variation_without_repetition(head + [tail[i]], tail[:i] + tail[(1 + i):], n - 1)
        return ret


def variation_with_repetition(head, tail, n=0):
    if n == 0:
        return [[head]]
    else:
        ret = []
        for i in range(len(tail)):
            ret += variation_with_repetition(head + [tail[i]], tail, n-1)
        return ret


def combination(head, tail, n=0):
    if n == 0:
        return [[head]]
    elif len(tail) == n:
        return [head + tail]
    else:
        ret = []
        for i in range(len(tail)):
            ret += combination(head + [tail[i]], tail[(1 + i):], n - 1)
        return ret


def generate_combinations(arr, n):
    arr.sort()
    ret = combination([], arr, n)
    print(f"Number of combinations generated: {len(ret)}   ( length {n}, from {len(arr)} elements )")
    return ret


def generate_permutations(arr):
    ret = variation_without_repetition([], arr, len(arr))
    print(f"Number of permutations generated: {len(ret)}   ( from {len(arr)} elements )")
    return ret


def generate_variations_without_repetition(arr, n):
    ret = variation_without_repetition([], arr, n)
    print(f"Number of variations without repetition generated: {len(ret)}   ( length {n}, from {len(arr)} elements )")
    return ret


def generate_variations_with_repetition(arr, n):
    ret = variation_with_repetition([], arr, n)
    print(f"Number of variations with repetition generated: {len(ret)}   ( length {n}, from {len(arr)} elements )")
    return ret


def main():
    elements = [1, 2, 3, 4, 5, 6]
    p = generate_permutations(elements)
    c = generate_combinations(elements, 4)
    v_r = generate_variations_with_repetition(elements, 4)
    v_nr = generate_variations_without_repetition(elements, 4)
    # print(p)
    # print(c)
    # print(v_r)
    # print(v_nr)


if __name__ == '__main__':
    main()


