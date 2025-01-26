import copy

from Record import Record
import struct

BLOCKING_FACTOR = 4


class DataFile:
    def __init__(self):
        self._file = open('data.bin', "wb+")
        self.record_size = len(struct.pack('>ddi', 0, 0, 0))
        self.read_count = 0
        self.write_count = 0

    def _get_page_size(self):
        return self.record_size * BLOCKING_FACTOR

    def _write_page(self, pos, data):
        if pos % self._get_page_size() != 0:
            print("INCORRECT POSITION")
            return

        page_to_write = copy.copy(data)
        if len(data) != BLOCKING_FACTOR:
            for _ in range(len(data), BLOCKING_FACTOR):
                page_to_write.append(Record(-1, -1, -1))

        if len(page_to_write) != BLOCKING_FACTOR:
            print('INCORRECT SIZE')
            return

        self._file.seek(pos)
        for i in range(BLOCKING_FACTOR):
            self._file.write(struct.pack('>ddi', page_to_write[i].radius, page_to_write[i].height, page_to_write[i].index))
        self._file.flush()
        self.write_count += 1

    def _read_page(self, pos):
        if pos % self._get_page_size() != 0:
            print("INCORRECT POSITION")
            return
        data = []
        self._file.seek(pos)
        buffer = self._file.read(self._get_page_size())
        it = struct.iter_unpack('>ddi', buffer)
        while True:
            try:
                [radius, height, index] = list(next(it))
                data.append(Record(radius, height, index))
            except StopIteration:
                break
        self.read_count += 1
        return data

    def append(self, record: Record):
        self._file.seek(0, 2)
        pos = self._file.tell()
        if pos == 0:
            self._write_page(pos, [record])
            self._file.flush()
            return 0

        pos -= self._get_page_size()
        last_page = self._read_page(pos)
        last_empty_record = 0
        for i in range(BLOCKING_FACTOR):
            if last_page[i].index != -1:
                last_empty_record += 1
            else:
                break
        if last_empty_record < BLOCKING_FACTOR:
            last_page[last_empty_record] = record
            self._write_page(pos, last_page)
            self._file.flush()
            return pos + last_empty_record * self.record_size
        else:
            pos += self._get_page_size()
            self._write_page(pos, [record])
            self._file.flush()
            return pos

    def read_record(self, pos):
        page_pos = (pos // self._get_page_size())*self._get_page_size()
        page = self._read_page(page_pos)
        record_position = (pos - page_pos) // self.record_size
        return page[record_position]

    def read_all_file(self):
        self._file.seek(0, 2)
        end_pos = self._file.tell()
        self._file.seek(0)
        pos = self._file.tell()
        while pos != end_pos:
            record = self.read_record(pos)
            if record.index != -1:
                print(f'RECORD: position: {pos} - height: {record.height}, radius: {record.radius}, index: {record.index}')
            else:
                print(f'RECORD: position: {pos} - EMPTY')
            pos += self.record_size

    def mark_record_deleted(self, pos):
        self.update_record(pos, Record(-1, -1, -1))

    def update_record(self, pos, record: Record):
        page_pos = (pos // self._get_page_size()) * self._get_page_size()
        page = self._read_page(page_pos)
        record_position = (pos - page_pos) // self.record_size
        page[record_position] = record
        self._write_page(page_pos, page)

    def reorganize(self):
        self._file.seek(0, 2)
        end_pos = self._file.tell()
        if end_pos == 0:
            return {}
        pos = 0
        empty_records = []
        while pos != end_pos:
            self._file.seek(pos)
            page = self._read_page(pos)
            for idx, record in enumerate(page):
                if record.index == -1:
                    empty_records.append(pos + idx * self.record_size)

            pos += self._get_page_size()
        moved_records = {}
        pos -= self.record_size
        page_iter = BLOCKING_FACTOR - 1
        page_pos = end_pos - self._get_page_size()
        page = self._read_page(page_pos)
        for empty_pos in empty_records:
            while pos in empty_records:
                page, page_iter, pos, page_pos = self._get_next_record(page, page_iter, page_pos, pos)

            if page is None or empty_pos > pos:
                break
            self.update_record(empty_pos, page[page_iter])
            print(f'DATA FILE REORGANIZATION: Moved {pos} to {empty_pos}')
            self.update_record(pos, Record(-1, -1, -1))
            moved_records[pos] = empty_pos

            page, page_iter, pos, page_pos = self._get_next_record(page, page_iter, page_pos, pos)
            if page is None:
                break
        moved_pages = len(empty_records) // BLOCKING_FACTOR
        self._file.truncate(end_pos - moved_pages * self._get_page_size())
        return moved_records

    def _get_next_record(self, page, page_iter, page_pos, pos):
        pos -= self.record_size
        page_iter -= 1
        if page_iter < 0:
            page_pos -= self._get_page_size()
            if page_pos < 0:
                return None, None, None, None
            page = self._read_page(page_pos)
            page_iter = BLOCKING_FACTOR - 1
        return page, page_iter, pos, page_pos





