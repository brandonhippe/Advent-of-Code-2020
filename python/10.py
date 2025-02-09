import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
from functools import cache


def part1(data):
    """ 2020 Day 10 Part 1

    >>> part1(['16', '10', '15', '5', '1', '11', '7', '19', '6', '12', '4'])
    35
    >>> part1(['28', '33', '18', '42', '31', '14', '46', '20', '48', '47', '24', '23', '49', '45', '19', '38', '39', '11', '1', '32', '25', '35', '8', '17', '7', '9', '4', '2', '34', '10', '3'])
    220
    """

    nums = [0] + sorted([int(line) for line in data])
    nums.append(nums[-1] + 3)

    return len([1 for i in range(1, len(nums)) if nums[i] - nums[i - 1] == 1]) * len([3 for i in range(1, len(nums)) if nums[i] - nums[i - 1] == 3])


def part2(data):
    """ 2020 Day 10 Part 2

    >>> part2(['16', '10', '15', '5', '1', '11', '7', '19', '6', '12', '4'])
    8
    >>> part2(['28', '33', '18', '42', '31', '14', '46', '20', '48', '47', '24', '23', '49', '45', '19', '38', '39', '11', '1', '32', '25', '35', '8', '17', '7', '9', '4', '2', '34', '10', '3'])
    19208
    """

    nums = [0] + sorted([int(line) for line in data])
    nums.append(nums[-1] + 3)

    return adapterCombinations(tuple(nums))


@cache
def adapterCombinations(data):
    if len(data) == 1:
        return 1

    total = 0
    i = 1
    while i < len(data) and data[i] <= data[0] + 3:
        total += adapterCombinations(data[i:])
        i += 1

    return total


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
        print(f"\nPart 1:\nDifferences of 1 * Differences of 3: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nNumber of ways to arrange adapters: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)