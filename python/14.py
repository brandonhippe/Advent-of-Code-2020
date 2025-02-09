import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
import re


def part1(data):
    """ 2020 Day 14 Part 1

    >>> part1(['mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X', 'mem[8] = 11', 'mem[7] = 101', 'mem[8] = 0'])
    165
    """

    mem = {}
    for line in data:
        if line[1] == 'a':
            mask = re.split(' ', line)[-1]
        else:
            addr, val = [int(x) for x in re.findall('\d+', line)]
            ones = int(''.join(['1' if c == '1' else '0' for c in mask]), 2)
            zeros = int(''.join(['0' if c == '0' else '1' for c in mask]), 2)
            mem[addr] = (val & zeros) | ones

    return sum(list(mem.values()))


def part2(data):
    """ 2020 Day 14 Part 2

    >>> part2(['mask = 000000000000000000000000000000X1001X', 'mem[42] = 100', 'mask = 00000000000000000000000000000000X0XX', 'mem[26] = 1'])
    208
    """

    mem = {}
    for line in data:
        if line[1] == 'a':
            mask = re.split(' ', line)[-1]
        else:
            addr, val = [int(x) for x in re.findall('\d+', line)]
            ones = int(''.join(['1' if c == '1' else '0' for c in mask]), 2)


            baseAddr = list('0' * (36 - len(bin(addr | ones)[2:])) + bin(addr | ones)[2:])
            for n in range(2 ** len([c for c in mask if c == 'X'])):
                b = '0' * (len([c for c in mask if c == 'X']) - len(bin(n)[2:])) + bin(n)[2:]
                addr = baseAddr[:]
                j = 0
                for i, c in enumerate(mask):
                    if c == 'X':
                        addr[i] = b[j]
                        j += 1

                mem[int(''.join(addr), 2)] = val

    return sum(list(mem.values()))


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
        print(f"\nPart 1:\nSum of all values in memory: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nSum of all values in memory: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)