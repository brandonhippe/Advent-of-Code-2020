use regex::Regex;
use relative_path::RelativePath;
use std::collections::HashMap;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let mut and_bitmask: u64 = 0;
    let mut or_bitmask: u64 = u64::MAX;
    let mut memory: HashMap<u64, u64> = HashMap::new();
    let int_regex = Regex::new(r"\d+").unwrap();

    for line in contents.lines() {
        let mut parts = line.split(" = ");
        if parts.nth(0).unwrap() == "mask" {
            let mask = parts.nth(0).unwrap();
            and_bitmask = u64::from_str_radix(&mask.replace("X", "1"), 2).unwrap();
            or_bitmask = u64::from_str_radix(&mask.replace("X", "0"), 2).unwrap();
        } else {
            let (address, value) = (
                int_regex
                    .find(line)
                    .unwrap()
                    .as_str()
                    .parse::<u64>()
                    .unwrap(),
                parts.nth(0).unwrap().parse::<u64>().unwrap(),
            );
            memory.insert(address, (value & and_bitmask) | or_bitmask);
        }
    }

    return memory.values().sum::<u64>() as i64;
}


fn part2(contents: String) -> i64 {
    let mut or_bitmask: u64 = 0;
    let mut floating_bits: Vec<usize> = Vec::new();
    let mut memory: HashMap<u64, u64> = HashMap::new();
    let int_regex = Regex::new(r"\d+").unwrap();

    for line in contents.lines() {
        let mut parts = line.split(" = ");
        if parts.nth(0).unwrap() == "mask" {
            let mask = parts.nth(0).unwrap();
            or_bitmask = u64::from_str_radix(&mask.replace("X", "0"), 2).unwrap();
            floating_bits = mask.match_indices("X").map(|(i, _)| 35 - i).collect();
        } else {
            let (mut address, value) = (
                int_regex
                    .find(line)
                    .unwrap()
                    .as_str()
                    .parse::<u64>()
                    .unwrap(),
                parts.nth(0).unwrap().parse::<u64>().unwrap(),
            );

            address |= or_bitmask;

            for i in 0..2u64.pow(floating_bits.len() as u32) {
                let mut new_address = address;
                for (j, bit) in floating_bits.iter().enumerate() {
                    if i & (1 << j) != 0 {
                        new_address |= 1 << bit;
                    } else {
                        new_address &= !(1 << bit);
                    }
                }
                memory.insert(new_address, value);
            }
        }
    }

    return memory.values().sum::<u64>() as i64;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("p1_example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 165);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("p2_example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 208);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "14".to_string();

    let root = env::current_dir().unwrap();
    let path_str = if args.len() > 1 {
        args[1].clone()
    } else if root.ends_with(format!("{}", day)) {
        format!("../../../Inputs/{}_{}.txt", year, day)
    } else {
        format!("/Inputs/{}_{}.txt", year, day)
    };


    let contents = fs::read_to_string(if args.len() > 1 {path_str} else {RelativePath::new(&path_str).to_path(&root).display().to_string()})
        .expect("Should have been able to read the file");

    let part1_timer = Instant::now();
    println!(
        "\nPart 1:\nSum of values in memory: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nSum of values in memory: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}