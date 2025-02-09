use regex::Regex;
use relative_path::RelativePath;
use std::collections::HashMap;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let valid_set: HashSet<&str> = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]
        .iter()
        .cloned()
        .collect();
    let field_re = Regex::new(r"(\w+):").unwrap();

    let mut valid_count = 0;
    let mut fields = HashSet::new();
    for line in contents.lines() {
        if line.is_empty() {
            if fields.is_superset(&valid_set) {
                valid_count += 1;
            }
            fields.clear();
        } else {
            for cap in field_re.captures_iter(line) {
                fields.insert(cap.get(1).unwrap().as_str());
            }
        }
    }

    if fields.is_superset(&valid_set) {
        valid_count += 1;
    }

    return valid_count;

}

fn part2(contents: String) -> i64 {
    let field_re = Regex::new(r"(\w+):(\S+)").unwrap();

    let mut valid_count = 0;
    let mut fields: HashMap<&str, &str> = HashMap::new();
    for line in contents.lines() {
        if line.is_empty() {
            valid_count += valid_p2(&fields);
            fields.clear();
        } else {
            for cap in field_re.captures_iter(line) {
                fields.insert(cap.get(1).unwrap().as_str(), cap.get(2).unwrap().as_str());
            }
        }
    }

    valid_count += valid_p2(&fields);

    return valid_count;
}

fn valid_p2(fields: &HashMap<&str, &str>) -> i64 {
    let valid_set: HashSet<&str> = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]
        .iter()
        .cloned()
        .collect();
    if !fields
        .keys()
        .map(|k| *k)
        .collect::<HashSet<&str>>()
        .is_superset(&valid_set)
    {
        return 0;
    }

    let byr = fields.get("byr").unwrap().parse::<i64>().unwrap();
    if byr < 1920 || byr > 2002 {
        return 0;
    }

    let iyr = fields.get("iyr").unwrap().parse::<i64>().unwrap();
    if iyr < 2010 || iyr > 2020 {
        return 0;
    }

    let eyr = fields.get("eyr").unwrap().parse::<i64>().unwrap();
    if eyr < 2020 || eyr > 2030 {
        return 0;
    }

    let hgt = fields.get("hgt").unwrap();

    if !Regex::new(r"^\d+(cm|in)$").unwrap().is_match(hgt) {
        return 0;
    }

    let hgt_unit = &hgt[hgt.len() - 2..];
    let hgt_val = hgt[..hgt.len() - 2].parse::<i64>().unwrap();
    if hgt_unit == "cm" {
        if hgt_val < 150 || hgt_val > 193 {
            return 0;
        }
    } else if hgt_unit == "in" {
        if hgt_val < 59 || hgt_val > 76 {
            return 0;
        }
    } else {
        return 0;
    }

    let hcl = fields.get("hcl").unwrap();
    if !Regex::new(r"^#[0-9a-f]{6}$").unwrap().is_match(hcl) {
        return 0;
    }

    let ecl = fields.get("ecl").unwrap();
    if !Regex::new(r"^(amb|blu|brn|gry|grn|hzl|oth)$")
        .unwrap()
        .is_match(ecl)
    {
        return 0;
    }

    if !Regex::new(r"^\d{9}$")
        .unwrap()
        .is_match(fields.get("pid").unwrap())
    {
        return 0;
    }

    return 1;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 2);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 0);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "4".to_string();

    let root = env::current_dir().unwrap();
    let path_str = if args.len() > 1 {
        args[1].clone()
    } else if root.ends_with(format!("rust_{}_{}", year, day)) {
        format!("../../../Inputs/{}_{}.txt", year, day)
    } else {
        format!("/Inputs/{}_{}.txt", year, day)
    };


    let contents = fs::read_to_string(if args.len() > 1 {path_str} else {RelativePath::new(&path_str).to_path(&root).display().to_string()})
        .expect("Should have been able to read the file");

    let part1_timer = Instant::now();
    println!(
        "\nPart 1:\nValid passports: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nValid passports: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}