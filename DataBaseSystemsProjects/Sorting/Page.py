from Record import Record

PAGE_BLOCKING_FACTOR = 5


class Page:
    def __init__(self):
        self._data = [None for _ in range(PAGE_BLOCKING_FACTOR)]

    def __getitem__(self, key):
        return self._data[key]

    def __setitem__(self, key: int, value):
        self._data[key] = value

    def __len__(self):
        return len(self._data)

    def to_list(self):
        return [record.__dict__ if record else None for record in self._data]

    @classmethod
    def from_list(cls, data_list: list):
        length = len(data_list)
        ret = cls()
        for i in range(length):
            record = Record()
            for k, v in data_list[i].items():
                setattr(record, k, v)
            ret[i] = record
        return ret
