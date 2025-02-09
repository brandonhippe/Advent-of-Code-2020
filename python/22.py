import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
def part1(data):
    """ 2020 Day 22 Part 1

    >>> part1(['Player 1:', '9', '2', '6', '3', '1', '', 'Player 2:', '5', '8', '4', '7', '10'])
    306
    """

    player1 = []
    player2 = []
    curr = player1
    for line in data:
        if len(line) == 0:
            curr = player2
            continue

        if line[0] == 'P':
            continue

        curr.append(int(line))

    p1, p2 = combat(player1, player2)
    winner = p1 if len(p1) != 0 else p2

    return sum([n * (len(winner) - i) for i, n in enumerate(winner)])


def part2(data):
    """ 2020 Day 22 Part 2

    >>> part2(['Player 1:', '9', '2', '6', '3', '1', '', 'Player 2:', '5', '8', '4', '7', '10'])
    291
    """

    player1 = []
    player2 = []
    curr = player1
    for line in data:
        if len(line) == 0:
            curr = player2
            continue

        if line[0] == 'P':
            continue

        curr.append(int(line))

    p1, p2 = combat(player1, player2, True)
    winner = p1 if len(p1) != 0 else p2

    return sum([n * (len(winner) - i) for i, n in enumerate(winner)])


def combat(player1, player2, recursive = False):
    gameStates = set()

    while min(len(player1), len(player2)) > 0:
        if (tuple(player1), tuple(player2)) in gameStates:
            return [player1 + player2, []]

        gameStates.add((tuple(player1), tuple(player2)))

        if recursive and player1[0] < len(player1) and player2[0] < len(player2):
            win = len(combat(player1[1:1+player1[0]], player2[1:1+player2[0]], True)[0]) > 0
        else:
            win = player1[0] > player2[0]

        if win:
            player1 = player1[1:] + [player1[0], player2[0]]
            player2 = player2[1:]
        else:
            player2 = player2[1:] + [player2[0], player1[0]]
            player1 = player1[1:]

    return [player1, player2]


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
        print(f"\nPart 1:\nWinner's score: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nWinner's score: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)