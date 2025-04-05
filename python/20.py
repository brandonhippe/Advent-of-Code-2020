import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
import re
import numpy as np
from collections import defaultdict


def part1(data):
    """ 2020 Day 20 Part 1

    >>> part1(['Tile 2311:', '..##.#..#.', '##..#.....', '#...##..#.', '####.#...#', '##.##.###.', '##...#.###', '.#.#.#..##', '..#....#..', '###...#.#.', '..###..###', '', 'Tile 1951:', '#.##...##.', '#.####...#', '.....#..##', '#...######', '.##.#....#', '.###.#####', '###.##.##.', '.###....#.', '..#.#..#.#', '#...##.#..', '', 'Tile 1171:', '####...##.', '#..##.#..#', '##.#..#.#.', '.###.####.', '..###.####', '.##....##.', '.#...####.', '#.##.####.', '####..#...', '.....##...', '', 'Tile 1427:', '###.##.#..', '.#..#.##..', '.#.##.#..#', '#.#.#.##.#', '....#...##', '...##..##.', '...#.#####', '.#.####.#.', '..#..###.#', '..##.#..#.', '', 'Tile 1489:', '##.#.#....', '..##...#..', '.##..##...', '..#...#...', '#####...#.', '#..#.#.#.#', '...#.#.#..', '##.#...##.', '..##.##.##', '###.##.#..', '', 'Tile 2473:', '#....####.', '#..#.##...', '#.##..#...', '######.#.#', '.#...#.#.#', '.#########', '.###.#..#.', '########.#', '##...##.#.', '..###.#.#.', '', 'Tile 2971:', '..#.#....#', '#...###...', '#.#.###...', '##.##..#..', '.#####..##', '.#..####.#', '#..#.#..#.', '..####.###', '..#.#.###.', '...#.#.#.#', '', 'Tile 2729:', '...#.#.#.#', '####.#....', '..#.#.....', '....#..#.#', '.##..##.#.', '.#.####...', '####.#.#..', '##.####...', '##..#.##..', '#.##...##.', '', 'Tile 3079:', '#.#.#####.', '.#..######', '..#.......', '######....', '####.#..#.', '.#...#.##.', '#.#####.##', '..#.###...', '..#.......', '..#.###...'])
    20899048083289
    """

    lines = '\n'.join(line.strip('\n') for line in data if len(line) != 0)

    tiles = {num: tile for num, tile in zip([int(x) for x in re.findall(r'\d+', lines)], [np.asarray([list(l) for l in line.strip('\n').split('\n')]) for line in re.split(r'Tile \d+:', lines)[1:]])}
    sideInfo = defaultdict(lambda: set())

    for n, t in zip(tiles.keys(), tiles.values()):
        for i in range(4):
            sideInfo[''.join(np.rot90(t, i)[-1, :])].add(n)
            sideInfo[''.join(np.rot90(t, i)[-1, ::-1])].add(n)

    sideMatches = defaultdict(lambda: set())

    for side, s in zip(list(sideInfo.keys()), list(sideInfo.values())):
        if len(s) == 1:
            del sideInfo[side]
        else:
            for id in list(s):
                sideMatches[id] = sideMatches[id].union(s)
                sideMatches[id].remove(id)

    prod = 1
    corner = None
    for n, s in zip(sideMatches.keys(), sideMatches.values()):
        if len(s) == 2:
            prod *= n

            if corner is None:
                corner = n
                r = 0
                while ''.join(transform(tiles[corner], r)[-1, :]) not in sideInfo or ''.join(transform(tiles[corner], r)[:, -1]) not in sideInfo:
                    r += 1

                tiles[corner] = transform(tiles[corner], r)

    return prod


def part2(data):
    """ 2020 Day 20 Part 2

    >>> part2(['Tile 2311:', '..##.#..#.', '##..#.....', '#...##..#.', '####.#...#', '##.##.###.', '##...#.###', '.#.#.#..##', '..#....#..', '###...#.#.', '..###..###', '', 'Tile 1951:', '#.##...##.', '#.####...#', '.....#..##', '#...######', '.##.#....#', '.###.#####', '###.##.##.', '.###....#.', '..#.#..#.#', '#...##.#..', '', 'Tile 1171:', '####...##.', '#..##.#..#', '##.#..#.#.', '.###.####.', '..###.####', '.##....##.', '.#...####.', '#.##.####.', '####..#...', '.....##...', '', 'Tile 1427:', '###.##.#..', '.#..#.##..', '.#.##.#..#', '#.#.#.##.#', '....#...##', '...##..##.', '...#.#####', '.#.####.#.', '..#..###.#', '..##.#..#.', '', 'Tile 1489:', '##.#.#....', '..##...#..', '.##..##...', '..#...#...', '#####...#.', '#..#.#.#.#', '...#.#.#..', '##.#...##.', '..##.##.##', '###.##.#..', '', 'Tile 2473:', '#....####.', '#..#.##...', '#.##..#...', '######.#.#', '.#...#.#.#', '.#########', '.###.#..#.', '########.#', '##...##.#.', '..###.#.#.', '', 'Tile 2971:', '..#.#....#', '#...###...', '#.#.###...', '##.##..#..', '.#####..##', '.#..####.#', '#..#.#..#.', '..####.###', '..#.#.###.', '...#.#.#.#', '', 'Tile 2729:', '...#.#.#.#', '####.#....', '..#.#.....', '....#..#.#', '.##..##.#.', '.#.####...', '####.#.#..', '##.####...', '##..#.##..', '#.##...##.', '', 'Tile 3079:', '#.#.#####.', '.#..######', '..#.......', '######....', '####.#..#.', '.#...#.##.', '#.#####.##', '..#.###...', '..#.......', '..#.###...'])
    273
    """

    lines = '\n'.join(line.strip('\n') for line in data if len(line) != 0)

    tiles = {num: tile for num, tile in zip([int(x) for x in re.findall(r'\d+', lines)], [np.asarray([list(l) for l in line.strip('\n').split('\n')]) for line in re.split(r'Tile \d+:', lines)[1:]])}
    sideInfo = defaultdict(lambda: set())

    for n, t in zip(tiles.keys(), tiles.values()):
        for i in range(4):
            sideInfo[''.join(np.rot90(t, i)[-1, :])].add(n)
            sideInfo[''.join(np.rot90(t, i)[-1, ::-1])].add(n)

    sideMatches = defaultdict(lambda: set())

    for side, s in zip(list(sideInfo.keys()), list(sideInfo.values())):
        if len(s) == 1:
            del sideInfo[side]
        else:
            for id in list(s):
                sideMatches[id] = sideMatches[id].union(s)
                sideMatches[id].remove(id)

    corner = None
    for n, s in zip(sideMatches.keys(), sideMatches.values()):
        if len(s) == 2 and not corner:
            corner = n
            r = 0
            while ''.join(transform(tiles[corner], r)[-1, :]) not in sideInfo or ''.join(transform(tiles[corner], r)[:, -1]) not in sideInfo:
                r += 1

            tiles[corner] = transform(tiles[corner], r)

    image = assembleImage(corner, tiles, sideInfo, sideMatches)
    monster = np.asarray([list(l) for l in ['                  # ', '#    ##    ##    ###', ' #  #  #  #  #  #   ']])

    r = 0
    monsters = 0
    while monsters == 0:
        monsters, roughness = getRoughness(transform(image, r), monster)
        r += 1

    return roughness


def transform(input_img, amt):
    output_img = np.rot90(input_img, amt)
    if amt >= 4:
        output_img = np.flipud(output_img)

    return output_img


def assembleImage(corner, tiles, sideInfo, sideMatches):
    line = tiles[corner][1:-1, 1:-1]

    curr = corner
    while ''.join(tiles[curr][:, -1]) in sideInfo:
        rightSide = ''.join(tiles[curr][:, -1])
        curr = list(sideInfo[rightSide].intersection(sideMatches[curr]))[0]

        r = 0
        while ''.join(transform(tiles[curr], r)[:, 0]) != rightSide:
            r += 1

        tiles[curr] = transform(tiles[curr], r)

        line = np.concatenate([line, tiles[curr][1:-1, 1:-1]], axis=1)

    bottomSide = ''.join(tiles[corner][-1, :])
    if bottomSide in sideInfo:
        newCorner = list(sideInfo[bottomSide].intersection(sideMatches[corner]))[0]

        r = 0
        while ''.join(transform(tiles[newCorner], r)[0, :]) != bottomSide:
            r += 1

        tiles[newCorner] = transform(tiles[newCorner], r)

        return np.concatenate([line, assembleImage(newCorner, tiles, sideInfo, sideMatches)], axis=0)
    else:
        return line
    

def getRoughness(image, monster):
    monsters = 0
    water = {(i, j) for i in range(len(image[0])) for j in range(len(image)) if image[j, i] == '#'}

    for y in range(len(image) - len(monster)):
        for x in range(len(image[y]) - len(monster[0])):
            if all(image[y + j, x + i] == monster[j, i] if monster[j, i] == '#' else True for i in range(len(monster[0])) for j in range(len(monster))):
                monsters += 1
                for j in range(len(monster)):
                    for i in range(len(monster[j])):
                        if monster[j, i] == '#' and (x + i, y + j) in water:
                            water.remove((x + i, y + j))

    return (monsters, len(water))


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
        print(f"\nPart 1:\nProduct of Corner image IDs: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nWater Roughness: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)