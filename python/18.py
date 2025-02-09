import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
def part1(data):
    """ 2020 Day 18 Part 1

    >>> part1(['1 + 2 * 3 + 4 * 5 + 6'])
    71
    >>> part1(['1 + (2 * 3) + (4 * (5 + 6))'])
    51
    >>> part1(['2 * 3 + (4 * 5)'])
    26
    >>> part1(['5 + (8 * 3 + 9 + 3 * 4 * 3)'])
    437
    >>> part1(['5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))'])
    12240
    >>> part1(['((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2'])
    13632
    """

    return sum([e.evalP1() for e in [Expression(line) for line in data]])


def part2(data):
    """ 2020 Day 18 Part 2

    >>> part2(['1 + 2 * 3 + 4 * 5 + 6'])
    231
    >>> part2(['1 + (2 * 3) + (4 * (5 + 6))'])
    51
    >>> part2(['2 * 3 + (4 * 5)'])
    46
    >>> part2(['5 + (8 * 3 + 9 + 3 * 4 * 3)'])
    1445
    >>> part2(['5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))'])
    669060
    >>> part2(['((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2'])
    23340
    """

    return sum([e.evalP2() for e in [Expression(line) for line in data]])


class Expression:
    def __init__(self, text, sub=False):
        if sub:
            text = text[1:-1]

        self.ops = []
        self.vals = []
        bits = text.split(' ')
        i = 0
        while i < len(bits):
            if bits[i][0] == '(':
                opened = 0
                for c in bits[i]:
                    if c == '(':
                        opened += 1

                j = i + 1
                while opened != 0:
                    for c in bits[j]:
                        if c == '(':
                            opened += 1
                        elif c == ')':
                            opened -= 1

                    j += 1

                self.vals.append(Expression(' '.join(bits[i:j]), True))
                i = j
            else:
                try:
                    self.vals.append(int(bits[i]))
                except ValueError:
                    self.ops.append(bits[i])

                i += 1

    def evalP1(self):
        self.ops.reverse()
        self.vals.reverse()
        while len(self.vals) != 1:
            left = self.vals.pop(-1)
            right = self.vals.pop(-1)
            if isinstance(left, Expression):
                left = left.evalP1()

            if isinstance(right, Expression):
                right = right.evalP1()

            op = self.ops.pop(-1)

            if op == '+':
                self.vals.append(left + right)
            else:
                self.vals.append(left * right)

        return self.vals[0]

    def evalP2(self):
        for i, v in enumerate(self.vals):
            if isinstance(v, Expression):
                self.vals[i] = v.evalP2()

        for i, op in enumerate(self.ops):
            if op == '+':
                self.vals = self.vals[:i] + [1, self.vals[i] + self.vals[i + 1]] + self.vals[i + 2:]

        product = 1
        for v in self.vals:
            product *= v

        return product


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
        print(f"\nPart 1:\nSum of evaluated expressions: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nSum of evaluated expressions: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)