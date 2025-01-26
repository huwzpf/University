from IndexData import IndexData
from Node import Node

BYTES_PER_INT = 4
NODE_DELETED_CODE = -2


class IndexFile:
    def __init__(self, d):
        self.d = d
        self._file = open('indexes.bin', "wb+")
        self.write_count = 0
        self.read_count = 0
        self.file_end = 0

    def _get_record_size(self):
        n_keys = self.d * 2
        n_children = self.d * 2 + 1
        integers_per_key = 2
        integers_for_parent = 1
        return (n_keys * integers_per_key + n_children + integers_for_parent) * BYTES_PER_INT

    def append(self):
        self._file.seek(self.file_end)
        pos = self._file.tell()
        self.file_end += self._get_record_size()
        return pos

    def update_node(self, pos, data: Node):
        self._file.seek(pos)
        self._write_node(data)

    def _write_node(self, data):
        keys_indexes = [key.index for key in data.keys]
        keys_pointers = [key.pointer for key in data.keys]
        length = len(keys_indexes)

        if data.parent is None:
            self._file.write((-1).to_bytes(BYTES_PER_INT, "big", signed=True))
        else:
            self._file.write(data.parent.to_bytes(BYTES_PER_INT, "big", signed=True))

        if not data.is_leaf:
            self._file.write(data.children[0].to_bytes(BYTES_PER_INT, "big", signed=True))
        else:
            self._file.write((-1).to_bytes(BYTES_PER_INT, "big", signed=True))
        i = 0
        for _ in range(length):
            self._file.write(keys_indexes[i].to_bytes(BYTES_PER_INT, "big", signed=True))
            self._file.write(keys_pointers[i].to_bytes(BYTES_PER_INT, "big", signed=True))
            if data.is_leaf:
                self._file.write((-1).to_bytes(BYTES_PER_INT, "big", signed=True))
            else:
                self._file.write(data.children[i+1].to_bytes(BYTES_PER_INT, "big", signed=True))
            i += 1
        for _ in range(2 * self.d - i):
            for __ in range(3):
                self._file.write((-1).to_bytes(BYTES_PER_INT, "big", signed=True))
        self._file.flush()
        self.write_count += 1

    def _read_field_to_array(self, arr):
        data = int.from_bytes(self._file.read(BYTES_PER_INT), byteorder="big", signed=True)
        if data != -1:
            arr.append(data)

    def get_data(self, pos):
        self._file.seek(pos)
        keys_indexes = []
        keys_pointers = []
        children = []
        parent = int.from_bytes(self._file.read(BYTES_PER_INT), byteorder="big", signed=True)
        if parent == -1:
            parent = None

        self._read_field_to_array(children)
        for _ in range(self.d*2):
            self._read_field_to_array(keys_indexes)
            self._read_field_to_array(keys_pointers)
            self._read_field_to_array(children)
        keys = [IndexData(index, pointer) for index, pointer in zip(keys_indexes, keys_pointers)]

        self.read_count += 1
        return Node(keys=keys, children=children, parent=parent)

    def reorganize(self):
        empty_positions = []
        root_pos = None
        self._file.seek(0, 2)
        end_pos = self._file.tell()
        record_size = self._get_record_size()
        if end_pos % record_size != 0:
            print("INVALID FILE SIZE")
            return

        # find "holes"
        for pos in range(0, end_pos, record_size):
            self._file.seek(pos)
            data = self.get_data(pos)
            if data.parent == NODE_DELETED_CODE:
                empty_positions.append(pos)
            elif data.parent is None:
                root_pos = pos

        pos_to_move = end_pos - record_size
        moved_positions = {}
        new_root_pos = None
        for pos in empty_positions:
            while pos_to_move in empty_positions:
                pos_to_move -= record_size
            if pos > pos_to_move:
                break
            if pos_to_move <= 0:
                return
            if pos_to_move == root_pos:
                new_root_pos = pos

            node_to_move = self.get_data(pos_to_move)
            self.update_node(pos, node_to_move)
            moved_positions[pos_to_move] = pos
            print(f'INDEX FILE REORGANIZATION: Moved {pos_to_move} to {pos}')
            pos_to_move -= record_size

        for pos in range(0, end_pos, record_size):
            node = self.get_data(pos)
            data_changed = False
            for i in range(len(node.children)):
                if node.children[i] in moved_positions.keys():
                    node.children[i] = moved_positions[node.children[i]]
                    data_changed = True

            if node.parent in moved_positions.keys():
                node.parent = moved_positions[node.parent]
                data_changed = True

            if data_changed:
                self.update_node(pos, node)

        self._file.truncate(end_pos - len(empty_positions) * record_size)
        return new_root_pos

    def update_pointers(self, moved_pointers):
        self._file.seek(0, 2)
        end_pos = self._file.tell()
        record_size = self._get_record_size()

        for pos in range(0, end_pos, record_size):
            node = self.get_data(pos)
            data_changed = False
            for i in range(len(node.keys)):
                if node.keys[i].pointer in moved_pointers.keys():
                    node.keys[i].pointer = moved_pointers[node.keys[i].pointer]
                    data_changed = True

            if data_changed:
                self.update_node(pos, node)







