import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
from collections import defaultdict


def part1(data):
    """ 2020 Day 6 Part 1

    >>> part1(['abc', '', 'a', 'b', 'c', '', 'ab', 'ac', '', 'a', 'a', 'a', 'a', '', 'b', ''])
    11
    """

    count = 0
    group = defaultdict(lambda: 0)
    inGroup = 0
    for line in data:
        if len(line) == 0:
            count += len(group)

            group = defaultdict(lambda: 0)
            inGroup = 0
            continue
        
        inGroup += 1
        for c in line:
            group[c] += 1

    count += len(group)

    return count


def part2(data):
    """ 2020 Day 6 Part 2

    >>> part2(['abc', '', 'a', 'b', 'c', '', 'ab', 'ac', '', 'a', 'a', 'a', 'a', '', 'b', ''])
    6
    """

    count = 0
    group = defaultdict(lambda: 0)
    inGroup = 0
    for line in data:
        if len(line) == 0:
            count += len([g for g in list(group.keys()) if group[g] == inGroup])

            group = defaultdict(lambda: 0)
            inGroup = 0
            continue
        
        inGroup += 1
        for c in line:
            group[c] += 1
    
    count += len([g for g in list(group.keys()) if group[g] == inGroup])

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
        print(f"\nPart 1:\nNumber of questions answered yes: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nNumber of questions answered yes by everyone in a group: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)