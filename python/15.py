import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
def part1(data):
    """ 2020 Day 15 Part 1

    >>> part1(['1,3,2'])
    1
    >>> part1(['2,1,3'])
    10
    >>> part1(['1,2,3'])
    27
    >>> part1(['2,3,1'])
    78
    >>> part1(['3,2,1'])
    438
    >>> part1(['3,1,2'])
    1836
    """

    nums = [int(n) for n in data[0].split(',')]
    spoken = {d: i for i, d in enumerate(nums[:-1])}
    prev = nums[-1]
    for i in range(len(nums), 2020):
        if prev in spoken:
            spoken[prev], prev  = i - 1, i - 1 - spoken[prev]
        else:
            spoken[prev] = i - 1
            prev = 0

    return prev


def part2(data):
    """ 2020 Day 15 Part 2

    >>> part2(['0,3,6'])
    175594
    >>> part2(['1,3,2'])
    2578
    >>> part2(['2,1,3'])
    3544142
    >>> part2(['1,2,3'])
    261214
    >>> part2(['2,3,1'])
    6895259
    >>> part2(['3,2,1'])
    18
    >>> part2(['3,1,2'])
    362
    """

    nums = [int(n) for n in data[0].split(',')]
    spoken = {d: i for i, d in enumerate(nums[:-1])}
    prev = nums[-1]
    for i in range(len(nums), 30000000):
        if prev in spoken:
            spoken[prev], prev  = i - 1, i - 1 - spoken[prev]
        else:
            spoken[prev] = i - 1
            prev = 0

    return prev


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
        print(f"\nPart 1:\n2020th number spoken: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\n30000000th number spoken: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)