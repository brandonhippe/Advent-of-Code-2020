import re
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

sys.path.append(str(Path(__file__).parent.parent.parent))
from Modules.timer import Timer
import re


def part1(data):
    """ 2020 Day 4 Part 1

    >>> part1(['ecl:gry pid:860033327 eyr:2020 hcl:#fffffd', 'byr:1937 iyr:2017 cid:147 hgt:183cm', '', 'iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884', 'hcl:#cfa07d byr:1929', '', 'hcl:#ae17e1 iyr:2013', 'eyr:2024', 'ecl:brn pid:760753108 byr:1931', 'hgt:179cm', '', 'hcl:#cfa07d eyr:2025 pid:166559648', 'iyr:2011 ecl:brn hgt:59in', ''])
    2
    """

    passports = []
    currPassport = {}
    for line in data:
        if len(line) == 0:
            passports.append(currPassport)
            currPassport = {}
            continue

        for field in re.split(' ', line):
            k, v = re.split(':', field)
            currPassport[k] = v

    if currPassport:
        passports.append(currPassport)

    valid = 0
    for i, p in enumerate(passports):
        if len(p) == 8 or (len(p) == 7 and 'cid' not in p):
            valid += 1

    return valid


def part2(data):
    """ 2020 Day 4 Part 2

    >>> part2(['eyr:1972 cid:100', 'hcl:#18171d ecl:amb hgt:170 pid:186cm iyr:2018 byr:1926', '', 'iyr:2019', 'hcl:#602927 eyr:1967 hgt:170cm', 'ecl:grn pid:012533040 byr:1946', '', 'hcl:dab227 iyr:2012', 'ecl:brn hgt:182cm pid:021572410 eyr:2020 byr:1992 cid:277', '', 'hgt:59cm ecl:zzz', 'eyr:2038 hcl:74454a iyr:2023', 'pid:3556412378 byr:2007', '', 'pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980', 'hcl:#623a2f', '', 'eyr:2029 ecl:blu cid:129 byr:1989', 'iyr:2014 pid:896056539 hcl:#a97842 hgt:165cm', '', 'hcl:#888785', 'hgt:164cm byr:2001 iyr:2015 cid:88', 'pid:545766238 ecl:hzl', 'eyr:2022', '', 'iyr:2010 hgt:158cm hcl:#b6652a ecl:blu byr:1944 eyr:2021 pid:093154719', ''])
    4
    """

    passports = []
    currPassport = {}
    for line in data:
        if len(line) == 0:
            passports.append(currPassport)
            currPassport = {}
            continue

        for field in re.split(' ', line):
            k, v = re.split(':', field)
            currPassport[k] = v

    if currPassport:
        passports.append(currPassport)

    valid = 0
    for p in passports:
        if len(p) == 8 or (len(p) == 7 and 'cid' not in p):
            try:
                if not (1920 <= int(p['byr']) <= 2002):
                    continue
                if not (2010 <= int(p['iyr']) <= 2020):
                    continue
                if not (2020 <= int(p['eyr']) <= 2030):
                    continue
                if ('cm' in p['hgt'] and not (150 <= int(p['hgt'][:-2]) <= 193)) or ('in' in p['hgt'] and not (59 <= int(p['hgt'][:-2]) <= 76)) or not ('cm' in p['hgt'] or 'in' in p['hgt']):
                    continue
                if not (p['hcl'][0] == '#' and int(p['hcl'][1:], 16) is not None):
                    continue
                if not (p['ecl'] in ['amb', 'blu', 'brn', 'gry', 'grn', 'hzl', 'oth']):
                    continue
                if not (len(p['pid']) == 9 and int(p['pid']) is not None):
                    continue

                valid += 1
            except ValueError:
                continue

    return valid


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
        print(f"\nPart 1:\nValid passports: {p1}\nRan in {p1_time.elapsed:0.4f} seconds")

    with Timer() as p2_time:
        p2 = part2(data)

    if verbose:
        print(f"\nPart 2:\nValid passports with valid field data: {p2}\nRan in {p2_time.elapsed:0.4f} seconds")

    return [(p1, p1_time.elapsed), (p2, p2_time.elapsed)]


if __name__ == "__main__":
    main(verbose=True)