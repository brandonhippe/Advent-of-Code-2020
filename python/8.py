import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
from collections import defaultdict


def part1(data):
    """ 2020 Day 8 Part 1
    
    >>> part1(['nop +0', 'acc +1', 'jmp +4', 'acc +3', 'jmp -3', 'acc -99', 'acc +1', 'jmp -4', 'acc +6'])
    5
    """

    return runCode([line.split(' ') for line in data])[0]


def part2(data):
    """ 2020 Day 8 Part 2

    >>> part2(['nop +0', 'acc +1', 'jmp +4', 'acc +3', 'jmp -3', 'acc -99', 'acc +1', 'jmp -4', 'acc +6'])
    8
    """

    instructions = [line.split(' ') for line in data]

    for i, (op, val), in enumerate(instructions):
        if op == 'acc':
            continue

        acc, result = runCode(instructions[:i] + [['jmp' if op == 'nop' else 'nop', val]] + instructions[i + 1:])
        if result:
            break

    return acc


def runCode(instructions):
    regs = defaultdict(lambda: 0)
    ran = set()
    while regs['PC'] not in ran and 0 <= regs['PC'] < len(instructions):
        ran.add(regs['PC'])
        op, val = instructions[regs['PC']]
        if op == 'acc':
            regs['acc'] += int(val)
        elif op == 'jmp':
            regs['PC'] += int(val) - 1

        regs['PC'] += 1

    return [regs['acc'], regs['PC'] not in ran]


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
        print(f"\nPart 1:\nValue in accumulator before running any line of code twice: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nValue in accumulator after running fixed program: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)