import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
def part1(data):
    """ 2020 Day 23 Part 1

    >>> part1(['389125467'])
    67384529
    """

    cups = [int(n) for n in data[0]]
    for _ in range(100):
        dest = cups[0] - 2
        dest %= len(cups)
        while dest + 1 in cups[1:4]:
            dest -= 1
            dest %= len(cups)

        dest = cups.index(dest + 1)
        cups = cups[4:dest + 1] + cups[1:4] + cups[dest + 1:] + [cups[0]]

    return int(''.join(str(n) for n in cups[cups.index(1) + 1:] + cups[:cups.index(1)]))


def part2(data):
    """ 2020 Day 23 Part 2

    >>> part2(['389125467'])
    149245887792
    """

    cups = [int(n) for n in data[0]]
    cups = [cups[0]] + [cups[cups.index(i) + 1] if cups.index(i) < len(cups) - 1 else 10 for i in range(1, 10)] + [i + 1 for i in range(10, 1_000_000)] + [cups[0]]

    for _ in range(10_000_000):
        current = cups[0]
        held = cups[current]
        afterHeld = held
        removed = []
        for _ in range(3):
            removed.append(afterHeld)
            afterHeld = cups[afterHeld]

        dest = cups[0] - 2
        dest %= 1_000_000
        while dest + 1 in removed:
            dest -= 1
            dest %= 1_000_000

        afterDest = cups[dest + 1]

        cups[dest + 1] = held
        cups[removed[-1]] = afterDest
        
        cups[current] = afterHeld
        cups[0] = afterHeld
        # print(cups[:10])

    return cups[1] * cups[cups[1]]


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
        print(f"\nPart 1:\nDigits on cups after 1: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nProduct of cups counterclockwise of 1: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)