class Node:
    def __init__(self, keys=None, children=None, parent=None):
        if keys is None:
            keys = []
        if children is None:
            children = []

        self.keys = keys
        self.children = children
        self.parent = parent

    @property
    def is_leaf(self):
        return len(self.children) == 0

    @property
    def n_keys(self):
        return len(self.keys)

    def to_string(self):
        repr_str = '{'
        if self.is_leaf:
            for idx, key in enumerate(self.keys):
                repr_str += f'{key.index}'
                if idx != len(self.keys) - 1:
                    repr_str += ', '
        else:

            repr_str += '['
            repr_str += str(self.children[0])
            repr_str += ']'
            for idx, key in enumerate(self.keys):
                repr_str += f', {key.index}, '
                repr_str += '['
                repr_str += str(self.children[idx + 1])
                repr_str += ']'
        repr_str += '}'
        return repr_str

