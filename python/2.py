import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
import re


def part1(data):
    """ 2020 Day 2 Part 1

    >>> part1(['1-3 a: abcde', '1-3 b: cdefg', '2-9 c: ccccccccc'])
    2
    """

    count = 0

    for line in data:
        counts = [int(x) for x in re.findall(r'\d+', line)]
        c = re.findall('[a-z]:', line)[0][0]
        pwd = re.split(' ', line)[-1]

        if counts[0] <= len([a for a in pwd if a == c]) <= counts[1]:
            count += 1

    return count


def part2(data):
    """ 2020 Day 2 Part 2

    >>> part2(['1-3 a: abcde', '1-3 b: cdefg', '2-9 c: ccccccccc'])
    1
    """

    count = 0

    for line in data:
        counts = [int(x) for x in re.findall(r'\d+', line)]
        c = re.findall('[a-z]:', line)[0][0]
        pwd = re.split(' ', line)[-1]

        if (pwd[counts[0] - 1] == c) ^ (pwd[counts[1] - 1] == c):
            count += 1

    return count


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
        print(f"\nPart 1:\nValid Passwords: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nValid Passwords: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)