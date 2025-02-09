import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
def part1(data):
    """ 2020 Day 12 Part 1

    >>> part1(['F10', 'N3', 'F7', 'R90', 'F11'])
    25
    """

    pos = (0, 0)
    facing = (1, 0)

    for line in data:
        if line[0] == 'N':
            pos = (pos[0], pos[1] - int(line[1:]))
        elif line[0] == 'E':
            pos = (pos[0] + int(line[1:]), pos[1])
        elif line[0] == 'S':
            pos = (pos[0], pos[1] + int(line[1:]))
        elif line[0] == 'W':
            pos = (pos[0] - int(line[1:]), pos[1])
        elif line[0] == 'F':
            pos = (pos[0] + facing[0] * int(line[1:]), pos[1] + facing[1] * int(line[1:]))
        else:
            for _ in range(int(line[1:]) // 90):
                facing = ((1 if line[0] == 'L' else -1) * facing[1], (1 if line[0] == 'R' else -1) * facing[0])

    return manhatDist(pos, (0, 0))


def part2(data):
    """ 2020 Day 12 Part 2

    >>> part2(['F10', 'N3', 'F7', 'R90', 'F11'])
    286
    """

    pos = (0, 0)
    waypointOffset = (10, -1)

    for line in data:
        if line[0] == 'N':
            waypointOffset = (waypointOffset[0], waypointOffset[1] - int(line[1:]))
        elif line[0] == 'E':
            waypointOffset = (waypointOffset[0] + int(line[1:]), waypointOffset[1])
        elif line[0] == 'S':
            waypointOffset = (waypointOffset[0], waypointOffset[1] + int(line[1:]))
        elif line[0] == 'W':
            waypointOffset = (waypointOffset[0] - int(line[1:]), waypointOffset[1])
        elif line[0] == 'F':
            pos = (pos[0] + waypointOffset[0] * int(line[1:]), pos[1] + waypointOffset[1] * int(line[1:]))
        else:
            for _ in range(int(line[1:]) // 90):
                waypointOffset = ((1 if line[0] == 'L' else -1) * waypointOffset[1], (1 if line[0] == 'R' else -1) * waypointOffset[0])

    return manhatDist(pos, (0, 0))


def manhatDist(p1, p2):
    return sum([abs(c1 - c2) for c1, c2 in zip(p1, p2)])


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
        print(f"\nPart 1:\nManhattan distance to final destination: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nManhattan distance to final destination: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)