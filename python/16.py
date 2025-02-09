import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
import re


def part1(data):
    """ 2020 Day 16 Part 1

    >>> part1(['class: 1-3 or 5-7', 'row: 6-11 or 33-44', 'seat: 13-40 or 45-50', '', 'your ticket:', '7,1,14', '', 'nearby tickets:', '7,3,47', '40,4,50', '55,2,20', '38,6,12'])
    71
    """

    ranges = {}
    i = 0
    while len(data[i]) > 0:
        ranges[re.split(':', data[i])[0]] = [int(x) for x in re.findall('\d+', data[i])]
        i += 1

    i += 2
    otherTickets = {tuple(int(x) for x in re.findall('\d+', line)) for line in data[i + 3:]}

    errorRate = 0
    for t in list(otherTickets):
        for n in t:
            valid = False
            for r in ranges.values():
                for i in range(0, len(r), 2):
                    if r[i] <= n <= r[i + 1]:
                        valid = True
                        break

                if valid:
                    break

            if not valid:
                errorRate += n

    return errorRate


def part2(data, testing = False):
    """ 2020 Day 16 Part 2

    >>> part2(['class: 0-1 or 4-19', 'row: 0-5 or 8-19', 'seat: 0-13 or 16-19', '', 'your ticket:', '11,12,13', '', 'nearby tickets:', '3,9,18', '15,1,5', '5,14,9'], True)
    {'row': 11, 'class': 12, 'seat': 13}
    """

    ranges = {}
    i = 0
    while len(data[i]) > 0:
        ranges[re.split(':', data[i])[0]] = [int(x) for x in re.findall('\d+', data[i])]
        i += 1

    i += 2
    mine = [int(x) for x in re.findall('\d+', data[i])]
    otherTickets = {tuple(int(x) for x in re.findall('\d+', line)) for line in data[i + 3:]}

    for t in list(otherTickets):
        validTicket = True
        for n in t:
            valid = False
            for r in ranges.values():
                for i in range(0, len(r), 2):
                    if r[i] <= n <= r[i + 1]:
                        valid = True
                        break

                if valid:
                    break

            if not valid:
                validTicket = False

        if not validTicket:
            otherTickets.remove(t)

    validFields = {i: set(ranges.keys()) for i in range(len(ranges))}

    for t in list(otherTickets):
        for ix, n in enumerate(t):
            for k, r in zip(ranges.keys(), ranges.values()):
                valid = False
                for i in range(0, len(r), 2):
                    if r[i] <= n <= r[i + 1]:
                        valid = True
                        break
                
                if not valid and k in validFields[ix]:
                    validFields[ix].remove(k)
                    clearFound(validFields, ix)

    if testing:
        return {v: mine[k] for k, v in validFields.items()}

    product = 1
    for i, n in enumerate(mine):
        if validFields[i].split(' ')[0] == 'departure':
            product *= n

    return product


def clearFound(validFields, ix):
    if len(validFields[ix]) == 1:
        validFields[ix] = list(validFields[ix])[0]
        for i in range(len(validFields)):
            if i == ix or validFields[ix] not in validFields[i]:
                continue

            validFields[i].remove(validFields[ix])
            clearFound(validFields, i)


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
        print(f"\nPart 1:\nTicket scanning error rate: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nProduct of fields that start with departure: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)