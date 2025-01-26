from IndexFile import *
from PageCache import PagesCache
from DataFile import DataFile
from Record import Record


class Tree:
    def __init__(self, d):
        self.d = d
        self.cache = PagesCache(d)
        self.root_pos = self.cache.add_page()
        self._data_file = DataFile()
        self.deletion_count = 0
        self.deletion_threshold = 30

    def get_alpha(self):
        file_ops = self.get_file_ops()
        data_reads_before = file_ops["data"]["read"]
        data_writes_before = file_ops["data"]["write"]
        index_reads_before = file_ops["index"]["read"]
        index_writes_before = file_ops["index"]["write"]

        coeff_sum, n = self.get_node_alpha(self.root_pos)

        self._data_file.read_count = data_reads_before
        self._data_file.write_count = data_writes_before
        self.cache.set_read_count(index_reads_before)
        self.cache.set_write_count(index_writes_before)

        return coeff_sum / n

    def get_node_alpha(self, node_pos):
        coeff = len(self.cache.get_page(node_pos).keys) / (self.d * 2)
        n = 1
        if not self.cache.get_page(node_pos).is_leaf:
            for child in self.cache.get_page(node_pos).children:
                dc, dn = self.get_node_alpha(child)
                coeff += dc
                n += dn
        return coeff, n

    def get_file_ops(self):
        return {"index": {"read": self.cache.get_read_count(), "write": self.cache.get_write_count()},
                "data": {"read": self._data_file.read_count, "write": self._data_file.write_count}}

    def compare_file_ops(self, file_ops_before, print_result=True):
        file_ops_after = self.get_file_ops()
        index_d_read = file_ops_after["index"]["read"] - file_ops_before["index"]["read"]
        index_d_write = file_ops_after["index"]["write"] - file_ops_before["index"]["write"]
        data_d_read = file_ops_after["data"]["read"] - file_ops_before["data"]["read"]
        data_d_write = file_ops_after["data"]["write"] - file_ops_before["data"]["write"]
        if print_result:
            print(f'INDEX FILE: \n READS: {index_d_read},'
                  f' WRITES: {index_d_write} \n'
                  f'DATA FILE: \n READS: {data_d_read},'
                  f' WRITES: {data_d_write} \n')
        return index_d_read + index_d_write + data_d_read + data_d_write

    def add_data(self, record: Record, verbose=True):
        if verbose:
            print(f'INSERTING DATA - {record}\n')
            file_ops_before = self.get_file_ops()

        self._insert(record)
        if verbose:
            self.compare_file_ops(file_ops_before)

    def print_value(self, index_data: IndexData):
        data = self._data_file.read_record(index_data.pointer)
        repr_str = f'[KEY: {index_data.index} VALUE: {data}]'
        print(repr_str)

    def print_node(self, node_pos):
        keys_len = len(self.cache.get_page(node_pos).keys)
        if keys_len == 0:
            return

        if not self.cache.get_page(node_pos).is_leaf:
            self.print_node(self.cache.get_page(node_pos).children[0])

        for i in range(keys_len):
            self.print_value(self.cache.get_page(node_pos).keys[i])
            if not self.cache.get_page(node_pos).is_leaf:
                self.print_node(self.cache.get_page(node_pos).children[i+1])

    def print_data(self):
        file_ops = self.get_file_ops()
        data_reads_before = file_ops["data"]["read"]
        data_writes_before = file_ops["data"]["write"]
        index_reads_before = file_ops["index"]["read"]
        index_writes_before = file_ops["index"]["write"]

        self.print_node(self.root_pos)
        self.cache.pop_all_pages()

        self._data_file.read_count = data_reads_before
        self._data_file.write_count = data_writes_before
        self.cache.set_read_count(index_reads_before)
        self.cache.set_write_count(index_writes_before)

    def read_data(self, index: int):
        search_result = self.search(index)
        if search_result["idx"] is None:
            print("RECORD DOES NOT EXIST! ")
            return
        self.cache.pop_all_pages()
        pointer = self.cache.get_page(search_result["node"]).keys[search_result["idx"]].pointer
        return self._data_file.read_record(pointer)

    def update_data(self, record: Record):
        print("UPDATING DATA\n")
        file_ops_before = self.get_file_ops()
        search_result = self.search(record.index)
        if search_result["idx"] is None:
            print("RECORD DOES NOT EXIST! ")
            return
        self._data_file.update_record(
            self.cache.get_page(search_result["node"]).keys[search_result["idx"]].pointer, record)

        self.cache.pop_all_pages()
        self.compare_file_ops(file_ops_before)

    def print_tree(self, node_pos=None):
        if node_pos is None:
            node_pos = self.root_pos

        node = self.cache.get_page(node_pos)
        node_str = f'{node_pos} - '
        node_str += node.to_string()
        print(node_str)

        if not node.is_leaf:
            for child in node.children:
                self.print_tree(child)

    def reorganize(self):
        root = self.cache.reorganize_file()
        if root is not None:
            self.root_pos = root

        moved_records = self._data_file.reorganize()
        self.cache.update_pointers(moved_records)

    def visualize(self):
        file_ops = self.get_file_ops()
        data_reads_before = file_ops["data"]["read"]
        data_writes_before = file_ops["data"]["write"]
        index_reads_before = file_ops["index"]["read"]
        index_writes_before = file_ops["index"]["write"]

        print("\nDATA FILE\n")
        self._data_file.read_all_file()
        print("\nTREE:\n")
        self.print_tree()
        self.cache.pop_all_pages()

        self._data_file.read_count = data_reads_before
        self._data_file.write_count = data_writes_before
        self.cache.set_read_count(index_reads_before)
        self.cache.set_write_count(index_writes_before)

    def _is_full(self, node):
        return node.n_keys >= 2 * self.d

    def _is_within_capacity(self, node):
        return node.n_keys > self.d

    def _is_underfilled(self, node):
        if node.parent is None:
            return node.n_keys < 1

        return node.n_keys < self.d

    def _is_overfilled(self, node):
        return node.n_keys > 2 * self.d

    def search(self, index: int, node_pos=None):
        if node_pos is None:
            node_pos = self.root_pos

        node = self.cache.get_page(node_pos)
        selected_idx = 0
        selected_key = None
        for k in node.keys:
            if k.index < index:
                selected_idx += 1
            else:
                selected_key = k
                break
        if selected_key is not None and selected_key.index == index:
            return {"node": node_pos, "idx": selected_idx}

        if node.is_leaf:
            return {"node": node_pos, "idx": None}
        else:
            return self.search(index, node.children[selected_idx])

    def _get_key_idx(self, keys, index):
        idx = 0
        for k in keys:
            if k.index < index:
                idx += 1
            else:
                break
        return idx

    def _insert_not_full(self, node_pos, key_to_insert: IndexData, child_to_insert=None):
        idx = self._get_key_idx(self.cache.get_page(node_pos).keys, key_to_insert.index)
        self.cache.get_page(node_pos).keys.insert(idx, key_to_insert)
        if child_to_insert is not None:
            self.cache.get_page(node_pos).children.insert(idx + 1, child_to_insert)

    def _get_siblings(self, parent_pos, node_pos):
        ret = []
        leaf_idx = self.cache.get_page(parent_pos).children.index(node_pos)
        if leaf_idx > 0:
            self.cache.get_page(self.cache.get_page(parent_pos).children[leaf_idx - 1])
            ret.append({"sibling_pos": self.cache.get_page(parent_pos).children[leaf_idx - 1],
                        "where": "left"})

        if leaf_idx < len(self.cache.get_page(parent_pos).children) - 1:
            ret.append({"sibling_pos": self.cache.get_page(parent_pos).children[leaf_idx + 1],
                        "where": "right"})

        return ret

    def _split(self, node_pos):
        new_node_pos = self.cache.add_page()

        # append half keys to new node
        for _ in range(self.d):
            self.cache.get_page(new_node_pos).keys.append(self.cache.get_page(node_pos).keys.pop(self.d + 1))
            if not self.cache.get_page(node_pos).is_leaf:
                self.cache.get_page(new_node_pos).children.append(
                    self.cache.get_page(node_pos).children.pop(self.d + 1))

        # append children to new node
        if not self.cache.get_page(node_pos).is_leaf:
            self.cache.get_page(new_node_pos).children.append(self.cache.get_page(node_pos).children.pop(self.d + 1))
            # change parent value in children that were assigned to new node
            for child_pos in self.cache.get_page(new_node_pos).children:
                child = self.cache.get_page(child_pos)
                child.parent = new_node_pos
                self.cache.store_last_record()

        # handle splitting root
        if self.cache.get_page(node_pos).parent is None:
            # splitting root, tree grows
            new_root_pos = self.cache.add_page()
            self.cache.get_page(new_root_pos).keys.append(self.cache.get_page(node_pos).keys.pop(self.d))
            self.cache.get_page(new_root_pos).children = [node_pos, new_node_pos]
            self.cache.get_page(node_pos).parent = new_root_pos
            self.cache.get_page(new_node_pos).parent = new_root_pos
            self.root_pos = new_root_pos
        else:
            self.cache.get_page(new_node_pos).parent = self.cache.get_page(node_pos).parent
            self._insert_into_node(self.cache.get_page(node_pos).parent,
                                   self.cache.get_page(node_pos).keys.pop(self.d), new_node_pos)

    def _try_insertion_compensation(self, node_pos):
        if self.cache.get_page(node_pos).parent is None:
            return False

        if not self.cache.get_page(node_pos).is_leaf:
            return False

        siblings = self._get_siblings(self.cache.get_page(node_pos).parent, node_pos)
        non_full_siblings = list(filter(lambda sibling:
                                        not self._is_full(self.cache.get_page(sibling["sibling_pos"])), siblings))
        if len(non_full_siblings) == 0:
            return False

        sibling_to_compensate_pos = non_full_siblings[0]["sibling_pos"]
        where = non_full_siblings[0]["where"]

        self._compensate(node_pos, sibling_to_compensate_pos, where)

        return True

    def _try_deletion_compensation(self, node_pos):
        if self.cache.get_page(node_pos).parent is None:
            return False

        if not self.cache.get_page(node_pos).is_leaf:
            return False

        siblings = self._get_siblings(self.cache.get_page(node_pos).parent, node_pos)
        siblings_within_capacity = list(filter(lambda sibling:
                                        self._is_within_capacity(self.cache.get_page(sibling["sibling_pos"])), siblings))
        if len(siblings_within_capacity) == 0:
            return False

        sibling_to_compensate_pos = siblings_within_capacity[0]["sibling_pos"]
        where = siblings_within_capacity[0]["where"]

        self._compensate(node_pos, sibling_to_compensate_pos, where)

        return True

    def _compensate(self, node_pos, sibling_to_compensate_pos, where):
        idx = self.cache.get_page(self.cache.get_page(node_pos).parent).children.index(node_pos)

        if where == "left":
            idx -= 1
            all_keys = self._get_values_for_compensation(idx, node_pos, sibling_to_compensate_pos)
        else:
           all_keys = self._get_values_for_compensation(idx, sibling_to_compensate_pos, node_pos)
        keys_mid_point = len(all_keys) // 2
        self.cache.get_page(self.cache.get_page(node_pos).parent).keys[idx] = all_keys[keys_mid_point]

        if where == "left":
            self.cache.get_page(node_pos).keys = all_keys[keys_mid_point + 1:]
            self.cache.get_page(sibling_to_compensate_pos).keys = all_keys[:keys_mid_point]

        else:
            self.cache.get_page(sibling_to_compensate_pos).keys = all_keys[keys_mid_point + 1:]
            self.cache.get_page(node_pos).keys = all_keys[:keys_mid_point]

    def _get_values_for_compensation(self, parent_key_idx, first_node_pos, sibling_to_compensate_pos):
        all_keys = self.cache.get_page(sibling_to_compensate_pos).keys + [
            self.cache.get_page(self.cache.get_page(first_node_pos).parent).keys[parent_key_idx]] + self.cache.get_page(
            first_node_pos).keys
        return all_keys

    def _insert_into_node(self, node_pos, key_to_insert: IndexData, child_to_insert=None):
        self._insert_not_full(node_pos, key_to_insert, child_to_insert)

        if self._is_overfilled(self.cache.get_page(node_pos)):
            if self._try_insertion_compensation(node_pos):
                return

            self._split(node_pos)

    def _get_successor(self, node_pos, idx_in_node):
        child_pos = self.cache.get_page(node_pos).children[idx_in_node + 1]
        while not self.cache.get_page(child_pos).is_leaf:
            child_pos = self.cache.get_page(child_pos).children[0]
        return child_pos

    def _insert(self, record_to_insert: Record):
        search_result = self.search(record_to_insert.index)
        if search_result["idx"] is not None:
            print("ALREADY EXISTS!")
            return

        pos = self._data_file.append(record_to_insert)
        key_to_insert = IndexData(record_to_insert.index, pos)
        self._insert_into_node(search_result["node"], key_to_insert)

        self.cache.pop_all_pages()

    def _delete_from_node(self, node_pos, key_idx):
        # move all keys after deleted one to left
        for i in range(key_idx, len(self.cache.get_page(node_pos).keys) - 1):
            self.cache.get_page(node_pos).keys[i] = self.cache.get_page(node_pos).keys[i + 1]
        self.cache.get_page(node_pos).keys.pop(-1)

        # move all children after deleted one to left
        if not self.cache.get_page(node_pos).is_leaf:
            for i in range(key_idx + 1, len(self.cache.get_page(node_pos).children) - 1):
                self.cache.get_page(node_pos).children[i] = self.cache.get_page(node_pos).children[i + 1]
            self.cache.get_page(node_pos).children.pop(-1)

        if self._is_underfilled(self.cache.get_page(node_pos)):
            if self._try_deletion_compensation(node_pos):
                return

            # merge :(
            if self.cache.get_page(node_pos).parent is None:
                # root
                self.cache.get_page(node_pos).parent = NODE_DELETED_CODE
                if len(self.cache.get_page(node_pos).children) == 0:

                    self.root_pos = self.root_pos = self.cache.add_page()
                    return

                self.root_pos = self.cache.get_page(node_pos).children[0]
                self.cache.get_page(self.root_pos).parent = None
                return

            sibling_to_merge = self._get_siblings(self.cache.get_page(node_pos).parent, node_pos)[0]

            idx = self.cache.get_page(self.cache.get_page(node_pos).parent).children.index(node_pos)

            if sibling_to_merge["where"] == "left":
                idx -= 1

            key_from_parent = self.cache.get_page(self.cache.get_page(node_pos).parent).keys[idx]

            # child that will be deleted will always be right one
            if sibling_to_merge["where"] == "left":

                for child in self.cache.get_page(node_pos).children:
                    self.cache.get_page(child).parent = (sibling_to_merge["sibling_pos"])
                    self.cache.store_last_record()

                self.cache.get_page(sibling_to_merge["sibling_pos"]).keys +=\
                    [key_from_parent] + self.cache.get_page(node_pos).keys
                self.cache.get_page(sibling_to_merge["sibling_pos"]).children += self.cache.get_page(node_pos).children

                self.cache.get_page(node_pos).parent = NODE_DELETED_CODE
                self._delete_from_node(self.cache.get_page(sibling_to_merge["sibling_pos"]).parent, idx)

            else:
                for child in self.cache.get_page(sibling_to_merge["sibling_pos"]).children:
                    self.cache.get_page(child).parent = node_pos
                    self.cache.store_last_record()

                self.cache.get_page(node_pos).keys +=\
                    [key_from_parent] + self.cache.get_page(sibling_to_merge["sibling_pos"]).keys
                self.cache.get_page(node_pos).children += self.cache.get_page(sibling_to_merge["sibling_pos"]).children

                self.cache.get_page(sibling_to_merge["sibling_pos"]).parent = NODE_DELETED_CODE
                self._delete_from_node(self.cache.get_page(node_pos).parent, idx)

    def delete(self, key_to_delete, verbose=True):
        if self.deletion_count >= self.deletion_threshold:
            self.deletion_count = 0
            print("REORGANIZING")
            self.reorganize()
            self.cache.pop_all_pages()
        if verbose:
            print(f'DELETING DATA - {key_to_delete}\n')
            file_ops_before = self.get_file_ops()

        search_result = self.search(key_to_delete)
        if search_result["idx"] is None:
            print("RECORD DOES NOT EXIST! ")
            return

        self._data_file.mark_record_deleted(self.cache.get_page(search_result["node"]).keys[search_result["idx"]].pointer)
        if not self.cache.get_page(search_result["node"]).is_leaf:
            succesor_leaf_pos = self._get_successor(search_result["node"], search_result["idx"])
            succesor_leaf_idx = 0
            self.cache.get_page(search_result["node"]).keys[search_result["idx"]] =\
                self.cache.get_page(succesor_leaf_pos).keys[0]
        else:
            succesor_leaf_pos=search_result["node"]
            succesor_leaf_idx = search_result["idx"]

        self._delete_from_node(succesor_leaf_pos, succesor_leaf_idx)

        self.cache.pop_all_pages()
        if verbose:
            self.compare_file_ops(file_ops_before)

        self.deletion_count += 1

