from enum import Enum


class CollisionResult(Enum):
    MULTIPLIED = 0
    KILLED_OPPONENT = 1
    DIED = 2
    OPPONENT_ESCAPED = 3
    ESCAPED = 4
    BOTH_DIED = 5
