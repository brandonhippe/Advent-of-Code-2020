import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
from itertools import combinations


def part1(data, preamble = 25):
    """ 2020 Day 9 Part 1

    >>> part1(['35', '20', '15', '25', '47', '40', '62', '55', '65', '95', '102', '117', '150', '182', '127', '219', '299', '277', '309', '576'], 5)
    127
    """

    nums = [int(x) for x in data]

    for i, val in enumerate(nums):
        if i < preamble:
            continue

        if val not in [sum(c) for c in combinations(nums[i - preamble:i], 2)]:
            break

    return val


def part2(data, preamble = 25):
    """ 2020 Day 9 Part 2

    >>> part2(['35', '20', '15', '25', '47', '40', '62', '55', '65', '95', '102', '117', '150', '182', '127', '219', '299', '277', '309', '576'], 5)
    62
    """

    nums = [int(x) for x in data]
    for i, val in enumerate(nums):
        if i < preamble:
            continue

        if val not in [sum(c) for c in combinations(nums[i - preamble:i], 2)]:
            break

    for l in range(2, len(nums)):
        found = False
        for i in range(len(nums) - i - 1):
            if sum(nums[i:i + l]) == val:
                found = True
                break

        if found:
            break

    return min(nums[i:i + l]) + max(nums[i:i + l])


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
        print(f"\nPart 1:\nFirst value that is not the sum of two of the previous 25: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nSum of largest and smallest values in contiguous string that sum to invalid number: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)