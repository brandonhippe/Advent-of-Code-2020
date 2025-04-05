import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
import re
from itertools import product


def part1(data):
    """ 2020 Day 19 Part 1

    >>> part1(['0: 4 1 5', '1: 2 3 | 3 2', '2: 4 4 | 5 5', '3: 4 5 | 5 4', '4: "a"', '5: "b"', '', 'ababbb', 'bababa', 'abbbab', 'aaabbb', 'aaaabbb'])
    2
    >>> part1(['42: 9 14 | 10 1', '9: 14 27 | 1 26', '10: 23 14 | 28 1', '1: "a"', '11: 42 31', '5: 1 14 | 15 1', '19: 14 1 | 14 14', '12: 24 14 | 19 1', '16: 15 1 | 14 14', '31: 14 17 | 1 13', '6: 14 14 | 1 14', '2: 1 24 | 14 4', '0: 8 11', '13: 14 3 | 1 12', '15: 1 | 14', '17: 14 2 | 1 7', '23: 25 1 | 22 14', '28: 16 1', '4: 1 1', '20: 14 14 | 1 15', '3: 5 14 | 16 1', '27: 1 6 | 14 18', '14: "b"', '21: 14 1 | 1 14', '25: 1 1 | 1 14', '22: 14 14', '8: 42', '26: 14 22 | 1 20', '18: 15 15', '7: 14 5 | 1 21', '24: 14 1', '', 'abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa', 'bbabbbbaabaabba', 'babbbbaabbbbbabbbbbbaabaaabaaa', 'aaabbbbbbaaaabaababaabababbabaaabbababababaaa', 'bbbbbbbaaaabbbbaaabbabaaa', 'bbbababbbbaaaaaaaabbababaaababaabab', 'ababaaaaaabaaab', 'ababaaaaabbbaba', 'baabbaaaabbaaaababbaababb', 'abbbbabbbbaaaababbbbbbaaaababb', 'aaaaabbaabaaaaababaa', 'aaaabbaaaabbaaa', 'aaaabbaabbaaaaaaabbbabbbaaabbaabaaa', 'babaaabbbaaabaababbaabababaaab', 'aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba'])
    3
    """

    rules = {}
    messages = []
    for line in data:
        if len(messages) != 0 or len(line) == 0:
            messages.append(line)
            continue

        ruleNum, rText = line.split(':')
        if len(re.findall(r'\d+', rText)) != 0:
            rules[int(ruleNum)] = [[int(x) for x in re.findall(r'\d+', o)] for o in rText.split('|')]
        else:
            rules[int(ruleNum)] = rText.strip('" ')

    messages.pop(0)
    
    validLens = {}
    for r in rules.keys():
        if r not in validLens:
            validLens[r] = messageLen(r, rules, validLens)

    return len([m for m in messages if valid(m, 0, rules, validLens, False)])


def part2(data):
    """ 2020 Day 19 Part 2

    >>> part2(['42: 9 14 | 10 1', '9: 14 27 | 1 26', '10: 23 14 | 28 1', '1: "a"', '11: 42 31', '5: 1 14 | 15 1', '19: 14 1 | 14 14', '12: 24 14 | 19 1', '16: 15 1 | 14 14', '31: 14 17 | 1 13', '6: 14 14 | 1 14', '2: 1 24 | 14 4', '0: 8 11', '13: 14 3 | 1 12', '15: 1 | 14', '17: 14 2 | 1 7', '23: 25 1 | 22 14', '28: 16 1', '4: 1 1', '20: 14 14 | 1 15', '3: 5 14 | 16 1', '27: 1 6 | 14 18', '14: "b"', '21: 14 1 | 1 14', '25: 1 1 | 1 14', '22: 14 14', '8: 42', '26: 14 22 | 1 20', '18: 15 15', '7: 14 5 | 1 21', '24: 14 1', '', 'abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa', 'bbabbbbaabaabba', 'babbbbaabbbbbabbbbbbaabaaabaaa', 'aaabbbbbbaaaabaababaabababbabaaabbababababaaa', 'bbbbbbbaaaabbbbaaabbabaaa', 'bbbababbbbaaaaaaaabbababaaababaabab', 'ababaaaaaabaaab', 'ababaaaaabbbaba', 'baabbaaaabbaaaababbaababb', 'abbbbabbbbaaaababbbbbbaaaababb', 'aaaaabbaabaaaaababaa', 'aaaabbaaaabbaaa', 'aaaabbaabbaaaaaaabbbabbbaaabbaabaaa', 'babaaabbbaaabaababbaabababaaab', 'aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba'])
    12
    """

    rules = {}
    messages = []
    for line in data:
        if len(messages) != 0 or len(line) == 0:
            messages.append(line)
            continue

        ruleNum, rText = line.split(':')
        if len(re.findall(r'\d+', rText)) != 0:
            rules[int(ruleNum)] = [[int(x) for x in re.findall(r'\d+', o)] for o in rText.split('|')]
        else:
            rules[int(ruleNum)] = rText.strip('" ')

    messages.pop(0)

    rules[8].append([42, 8])
    rules[11].append([42, 11, 31])
    
    validLens = {}
    for r in rules.keys():
        if r not in validLens:
            validLens[r] = messageLen(r, rules, validLens)

    return len([m for m in messages if valid(m, 0, rules, validLens, True)])


def messageLen(rule, rules, validLens):
    if isinstance(rules[rule], str):
        return [1, set()]

    total = 0
    recursRepeats = set()
    for i in range(len(rules[rule])):
        if rule in rules[rule][i]:
            recursRepeats.add(rule)
            continue

        for subRule in rules[rule][i]:
            if subRule not in validLens:
                validLens[subRule] = messageLen(subRule, rules, validLens)

            if i == 0:
                total += validLens[subRule][0]

            recursRepeats = recursRepeats.union(validLens[subRule][1])

    return [total, recursRepeats]


def valid(message, rule, rules, validLens, recursive):
    if isinstance(rules[rule], str):
        return message == rules[rule]

    if len(validLens[rule][1]) == 0 and len(message) != validLens[rule][0]:
        return False

    for subRules in rules[rule]:
        if not recursive and rule in subRules:
            continue

        for repeats in product(*([n for n in range(1, 2 + ((len(message) - validLens[rule][0]) // validLens[sR][0] if recursive and len(validLens[sR][1]) != 0 else 0))] for sR in subRules)):
            splits = [0]

            for mult, sR in zip(repeats, subRules):
                splits.append(splits[-1] + mult * validLens[sR][0])

            if splits[-1] == len(message) and all(valid(message[splits[i]:splits[i + 1]], subRules[i], rules, validLens, recursive) for i in range(len(subRules))):
                return True

    return False


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
        print(f"\nPart 1:\nValid messages: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nValid messages: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)