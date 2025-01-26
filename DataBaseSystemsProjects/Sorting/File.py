import json

from Record import Record
from Page import *


class File:
    def __init__(self, filename: str, read: bool = True):
        self._filename = filename
        self._read_count = 0
        self._write_count = 0
        self._length = 0
        self._read_pointer = None
        self._file_beg = 0

        if not read:
            # create new empty file
            open(self._filename, 'w').close()

        self._file = open(self._filename, 'a+')

        if read:
            self._file.seek(0)
            line = self._file.readline()
            while line:
                self._length += 1
                line = self._file.readline()

    def __del__(self):
        self._file.close()

    def __len__(self):
        return self._length

    def write_page(self, p):
        self._file.seek(0, 2)
        for record in p.to_list():
            if record is None:
                break
            record_data = json.dumps(record) + '\n'
            self._file.write(record_data)
            self._length += 1

        self._file.flush()
        self._write_count += 1

    def load_page(self):
        if self._read_pointer is None:
            self._file.seek(self._file_beg)
        else:
            self._file.seek(self._read_pointer)
        line = self._file.readline()
        page_list = []
        while line:
            record = json.loads(line)
            page_list.append(record)
            if len(page_list) == PAGE_BLOCKING_FACTOR:
                self._read_count += 1
                self._read_pointer = self._file.tell()
                return Page.from_list(page_list)
            line = self._file.readline()

        self._read_pointer = None

        if len(page_list) == 0:
            return None
        else:
            self._read_count += 1
            return Page.from_list(page_list)

    def remove_line(self):
        self._file.seek(self._file_beg)
        self._file.readline()
        self._file_beg = self._file.tell()

    def reset_file(self):
        open(self._filename, 'w').close()
        self._file = open(self._filename, 'a+')
        self._length = 0
        self._file_beg = self._file.seek(0)

    def get_write_count(self):
        return self._write_count

    def get_read_count(self):
        return self._read_count

    def reset_read(self):
        self._read_pointer = None

    def reset_read_count(self):
        self._read_count = 0