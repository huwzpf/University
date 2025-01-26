from File import *


class Tape:
    def __init__(self, f: File):
        self._file = f
        self._runs = 0
        self._current_page = None
        self._page_idx = 0
        self._last_record = Record(0, 0)
        self.dummy_runs = 0
        self._next_record = None

    def get_file(self):
        return self._file

    def start_write(self):
        self._current_page = Page()
        self._page_idx = 0

    def write(self, record: Record):
        if record < self._last_record or self._runs == 0:
            self._runs += 1

        if self._page_idx == PAGE_BLOCKING_FACTOR:
            self.save_page()
            self._current_page = Page()
            self._page_idx = 0

        self._current_page[self._page_idx] = record
        self._page_idx += 1
        self._last_record = record

    def end_write(self):
        self.save_page()
        self._current_page = Page()
        self._page_idx = 0
        self._last_record = Record(0, 0)

    def save_page(self):
        self._file.write_page(self._current_page)
        self._page_idx = 0
        self._page_idx = 0

    def start_read(self):
        self._page_idx = 0
        self._current_page = self._file.load_page()
        self._next_record = self._get_next_record()

    def _get_next_record(self):
        if self._page_idx == PAGE_BLOCKING_FACTOR:
            self._current_page = self._file.load_page()
            self._page_idx = 0
        if self._current_page is None:
            return None
        record = self._current_page[self._page_idx]
        self._page_idx += 1
        return record

    def read(self, remove_runs=False):
        if self._next_record is None:
            return None

        record = self._next_record
        self._next_record = self._get_next_record()

        if remove_runs:
            if record is not None and self._next_record is not None and record > self._next_record:
                self._runs -= 1

            if record is None or self._next_record is None:
                self._runs -= 1
        if remove_runs and self._runs == 0:
            self._file.reset_file()
        if remove_runs:
            self._file.remove_line()
        return record

    def stop_read(self):
        self._file.reset_read()

    def get_runs(self):
        return self._runs

    def get_write_count(self):
        return self._file.get_write_count()

    def get_read_count(self):
        return self._file.get_read_count()