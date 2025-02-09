use regex::Regex;
use relative_path::RelativePath;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let re = Regex::new(r"(\d+)-(\d+) (\w): (\w+)").unwrap();
    return contents
        .lines()
        .map(|line| {
            let caps = re.captures(line).unwrap();
            let min = caps[1].parse::<i64>().unwrap();
            let max = caps[2].parse::<i64>().unwrap();
            let letter = &caps[3];
            let password = &caps[4];

            let count = password.matches(letter).count() as i64;
            if count >= min && count <= max {
                1
            } else {
                0
            }
        })
        .sum();
}

fn part2(contents: String) -> i64 {
    let re = Regex::new(r"(\d+)-(\d+) (\w): (\w+)").unwrap();
    return contents
        .lines()
        .map(|line| {
            let caps = re.captures(line).unwrap();
            let min = caps[1].parse::<i64>().unwrap();
            let max = caps[2].parse::<i64>().unwrap();

            let letter = &caps[3];
            let password = &caps[4].chars().collect::<Vec<char>>();

            ((password[min as usize - 1] == letter.chars().next().unwrap())
                ^ (password[max as usize - 1] == letter.chars().next().unwrap())) as i64
        })
        .sum();
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

        assert_eq!(part2(contents), 1);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "2".to_string();

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
        "\nPart 1:\nValid passwords: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nValid passwords: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}