import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
import re


def part1(data):
    """ 2020 Day 7 Part 1

    >>> part1(['light red bags contain 1 bright white bag, 2 muted yellow bags.', 'dark orange bags contain 3 bright white bags, 4 muted yellow bags.', 'bright white bags contain 1 shiny gold bag.', 'muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.', 'shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.', 'dark olive bags contain 3 faded blue bags, 4 dotted black bags.', 'vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.', 'faded blue bags contain no other bags.', 'dotted black bags contain no other bags.'])
    4
    """

    bags = {Bag(line.strip('\n')).name: Bag(line.strip('\n')) for line in data}

    return len([b for b in bags.values() if b.canContain(bags, 'shiny gold')]) - 1


def part2(data):
    """ 2020 Day 7 Part 2

    >>> part2(['light red bags contain 1 bright white bag, 2 muted yellow bags.', 'dark orange bags contain 3 bright white bags, 4 muted yellow bags.', 'bright white bags contain 1 shiny gold bag.', 'muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.', 'shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.', 'dark olive bags contain 3 faded blue bags, 4 dotted black bags.', 'vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.', 'faded blue bags contain no other bags.', 'dotted black bags contain no other bags.'])
    32
    >>> part2(['shiny gold bags contain 2 dark red bags.', 'dark red bags contain 2 dark orange bags.', 'dark orange bags contain 2 dark yellow bags.', 'dark yellow bags contain 2 dark green bags.', 'dark green bags contain 2 dark blue bags.', 'dark blue bags contain 2 dark violet bags.', 'dark violet bags contain no other bags.'])
    126
    """

    bags = {Bag(line.strip('\n')).name: Bag(line.strip('\n')) for line in data}

    return bags['shiny gold'].numContained(bags, {})


class Bag:
    def __init__(self, bagText):
        bagCounts = [int(x) for x in re.findall(r'\d+', bagText)]
        data = re.split(' bag?s |, ', bagText)

        self.name = data[0]
        self.bags = {}
        if len(bagCounts) > 0:
            for i, text in enumerate(data[1:]):
                start = re.search(r'\d+', text).span()[1] + 1
                end = re.search('bag', text).span()[0] - 1
                self.bags[text[start:end]] = bagCounts[i]

    def canContain(self, allBags, goal):
        if self.name == goal:
            return True

        for b in self.bags.keys():
            if allBags[b].canContain(allBags, goal):
                return True

        return False

    def numContained(self, allBags, memo):
        total = 0
        for b, amt in zip(self.bags.keys(), self.bags.values()):
            if b not in memo:
                memo[b] = allBags[b].numContained(allBags, memo)

            total += amt * (1 + memo[b])

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
        print(f"\nPart 1:\nNumber of bags that can contain a shiny gold bag: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nNumber of bags contained within a shiny gold bag: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)