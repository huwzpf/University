from IndexFile import IndexFile
from Node import Node
from Utils import value, key


class PagesCache:
    # Least Recently Used Cache
    def __init__(self, d, size=10):
        self._page_list = []
        self._file = IndexFile(d)
        self.size = size

    def get_write_count(self):
        return self._file.write_count

    def get_read_count(self):
        return self._file.read_count

    def set_write_count(self, count):
        self._file.write_count = count

    def set_read_count(self, count):
        self._file.read_count = count

    def reorganize_file(self):
        return self._file.reorganize()

    def update_pointers(self, moved_pointers):
        self._file.update_pointers(moved_pointers)

    def _find_page(self, pos):
        for idx, entry in enumerate(self._page_list):
            if key(entry) == pos:
                return idx
        return None

    def _fetch_page_from_cache(self, idx):
        self._page_list.append(self._page_list.pop(idx))

    def store_last_record(self):
        self._page_list.insert(0, self._page_list.pop(-1))
        self._pop_page_from_cache()

    def _pop_page_from_cache(self):
        self._file.update_node(key(self._page_list[0]), value(self._page_list[0]))
        self._page_list.pop(0)

    def _fetch_page(self, pos):
        if len(self._page_list) >= self.size:
            self._pop_page_from_cache()

        node = self._file.get_data(pos)
        self._page_list.append({pos: node})

    def get_page(self, pos) -> Node:
        idx = self._find_page(pos)
        if idx is None:
            self._fetch_page(pos)
        else:
            self._fetch_page_from_cache(idx)
        ret = value(self._page_list[-1])
        return ret

    def add_page(self):
        if len(self._page_list) >= self.size:
            self._pop_page_from_cache()

        new_node = Node()
        pos = self._file.append()
        self._page_list.append({pos: new_node})
        return pos

    def pop_all_pages(self):
        while len(self._page_list) > 0:
            self._pop_page_from_cache()
