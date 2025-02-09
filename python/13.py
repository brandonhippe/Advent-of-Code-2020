import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
import re


def part1(data):
    """ 2020 Day 13 Part 1

    >>> part1(['939', '7,13,x,x,59,x,31,19'])
    295
    """

    t = int(data[0])
    buses = {int(x): i for i, x in enumerate(re.split(',', data[1])) if x != 'x'}

    t1 = t
    while not any([t1 % b == 0 for b in buses.keys()]):
        t1 += 1

    return (t1 - t) * ([b for b in buses.keys() if t1 % b == 0][0])


def part2(data):
    """ 2020 Day 13 Part 2

    >>> part2(['939', '7,13,x,x,59,x,31,19'])
    1068781
    """

    buses = {int(x): i for i, x in enumerate(re.split(',', data[1])) if x != 'x'}

    t = 0
    step = 1
    while len(buses) > 0:
        for b in list(buses.keys())[::-1]:
            if (t + buses[b]) % b == 0:
                step *= b
                del buses[b]

        t += step

    return t - step


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
        print(f"\nPart 1:\nShortest time to wait for bus multiplied by bus ID: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nEarliest time step where buses depart at their offsets: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)