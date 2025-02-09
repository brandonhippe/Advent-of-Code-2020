import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
import re
from collections import defaultdict


def part1(data):
    """ 2020 Day 21 Part 1

    >>> part1(['mxmxvkd kfcds sqjhc nhms (contains dairy, fish)', 'trh fvjkl sbzzf mxmxvkd (contains dairy)', 'sqjhc fvjkl (contains soy)', 'sqjhc mxmxvkd sbzzf (contains fish)'])
    5
    """

    ingredients = defaultdict(lambda: set())
    ingredientOccurrances = defaultdict(lambda: 0)
    allergens = defaultdict(lambda: set())
    
    for i, line in enumerate(data):
        line = line.replace(',', '')
        ings, alls = re.split(' \(contains ', line)
        for ingredient in re.split(' ', ings):
            ingredients[ingredient].add(i)
            ingredientOccurrances[ingredient] += 1

        for allergen in re.split(' ', alls[:-1]):
            allergens[allergen].add(i)

    possible = {a: set(ingredients.keys()) for a in allergens.keys()}

    for a in allergens.keys():
        for i in list(possible[a]):
            if ingredients[i].union(allergens[a]) != ingredients[i]:
                possible[a].remove(i)
                removePossible(a, possible)

    return sum([v for k, v in zip(ingredientOccurrances.keys(), ingredientOccurrances.values()) if k not in possible.values()])


def part2(data):
    """ 2020 Day 21 Part 2

    >>> part2(['mxmxvkd kfcds sqjhc nhms (contains dairy, fish)', 'trh fvjkl sbzzf mxmxvkd (contains dairy)', 'sqjhc fvjkl (contains soy)', 'sqjhc mxmxvkd sbzzf (contains fish)'])
    'mxmxvkd,sqjhc,fvjkl'
    """

    ingredients = defaultdict(lambda: set())
    ingredientOccurrances = defaultdict(lambda: 0)
    allergens = defaultdict(lambda: set())
    
    for i, line in enumerate(data):
        line = line.replace(',', '')
        ings, alls = re.split(' \(contains ', line)
        for ingredient in re.split(' ', ings):
            ingredients[ingredient].add(i)
            ingredientOccurrances[ingredient] += 1

        for allergen in re.split(' ', alls[:-1]):
            allergens[allergen].add(i)

    possible = {a: set(ingredients.keys()) for a in allergens.keys()}

    for a in allergens.keys():
        for i in list(possible[a]):
            if ingredients[i].union(allergens[a]) != ingredients[i]:
                possible[a].remove(i)
                removePossible(a, possible)

    return ','.join(e[1] for e in sorted(zip(possible.keys(), possible.values()), key=lambda e: e[0]))


def removePossible(allergen, possible):
    if len(possible[allergen]) != 1:
        return

    possible[allergen] = list(possible[allergen])[0]
    for a in possible.keys():
        if a == allergen:
            continue

        if possible[allergen] in possible[a]:
            possible[a].remove(possible[allergen])
            removePossible(a, possible)

    return


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
        print(f"\nPart 1:\nNumber of occurrances of ingredients that aren't allergens: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nCanonical dangerous ingredient list: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)