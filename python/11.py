import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
from copy import deepcopy
from collections import defaultdict


def part1(data):
    """ 2020 Day 11 Part 1

    >>> part1(['L.LL.LL.LL', 'LLLLLLL.LL', 'L.L.L..L..', 'LLLL.LL.LL', 'L.LL.LL.LL', 'L.LLLLL.LL', '..L.L.....', 'LLLLLLLLLL', 'L.LLLLLL.L', 'L.LLLLL.LL'])
    37
    """

    seats = set()
    for y, line in enumerate(data):
        for x, l in enumerate(line):
            if l != '.':
                seats.add((x, y))

    currSeats = set()
    previous = seats
    while previous != currSeats:
        previous = deepcopy(currSeats)
        currSeats = iterateP1(currSeats, seats)

    return len(currSeats)


def part2(data):
    """ 2020 Day 11 Part 2

    >>> part2(['L.LL.LL.LL', 'LLLLLLL.LL', 'L.L.L..L..', 'LLLL.LL.LL', 'L.LL.LL.LL', 'L.LLLLL.LL', '..L.L.....', 'LLLLLLLLLL', 'L.LLLLLL.L', 'L.LLLLL.LL'])
    26
    """

    seats = set()
    for y, line in enumerate(data):
        for x, l in enumerate(line):
            if l != '.':
                seats.add((x, y))

    neighbors = defaultdict(lambda: [])
    for x, y in list(seats):
        for yOff in range(-1, 2):
            for xOff in range(-1, 2):
                if xOff == 0 and yOff == 0:
                    continue
                
                currX, currY = x + xOff, y + yOff
                while 0 <= currX < len(data[0]) and 0 <= currY < len(data[0]) and (currX, currY) not in seats:
                    currX += xOff
                    currY += yOff

                if (currX, currY) in seats:
                    neighbors[(x, y)].append((currX, currY))

    currSeats = set()
    previous = seats
    while previous != currSeats:
        previous = deepcopy(currSeats)
        currSeats = iterateP2(currSeats, neighbors)

    return len(currSeats)


def iterateP1(currSeats, seats):
    newSeats = set()

    for x, y in list(seats):
        neighbors = len([1 for xOff, yOff in [[-1, -1], [-1, 0], [-1, 1], [0, -1], [0, 1], [1, -1], [1, 0], [1, 1]] if (x + xOff, y + yOff) in currSeats])

        if ((x, y) not in currSeats and neighbors == 0) or ((x, y) in currSeats and neighbors < 4):
            newSeats.add((x, y))

    return newSeats


def iterateP2(seats, neighbors):
    newSeats = set()

    for (x, y), ns in zip(neighbors.keys(), neighbors.values()):
        nCount = len([1 for nX, nY in ns if (nX, nY) in seats])

        if ((x, y) not in seats and nCount == 0) or ((x, y) in seats and nCount < 5):
            newSeats.add((x, y))

    return newSeats


def main(input_path: Optional[Path | str]=None, verbose: bool=False) -> Tuple[Tuple[Any, float]]:
    if not input_path:
        if not (input_path := sys.argv[1] if len(sys.argv) > 1 else None):
            year, day = re.findall(r'\d+', str(__file__))[-2:]
            input_path = Path(Path(__file__).parent.parent.parent, "Inputs", f"{year}_{day}.txt")
    
    with open(input_path, encoding='UTF-8') as f:
        data = [line.strip('\n') for line in f.readlines()]

    with Timer() as p1_time:
        p1 = part1(data)

    if verbose:
        print(f"\nPart 1:\nNumber of seats occupied after settling: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nNumber of seats occupied after settling: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)