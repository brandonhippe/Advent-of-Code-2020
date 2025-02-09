use relative_path::RelativePath;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let nums: HashSet<i64> = contents
        .lines()
        .map(|x| x.parse::<i64>().unwrap())
        .collect();

    for num in nums.iter() {
        if nums.contains(&(2020 - num)) {
            return num * (2020 - num);
        }
    }

    return -1;
}

fn part2(contents: String) -> i64 {
    let nums: HashSet<i64> = contents
        .lines()
        .map(|x| x.parse::<i64>().unwrap())
        .collect();

    for num in nums.iter() {
        for num2 in nums.iter() {
            if num == num2 || num + num2 >= 2020 {
                continue;
            }

            if nums.contains(&(2020 - num - num2)) {
                return num * num2 * (2020 - num - num2);

            }
        }
    }

    return -1;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 514579);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 241861950);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "1".to_string();

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
        "\nPart 1:\nProduct of 2 numbers that sum to 2020: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nProduct of 3 numbers that sum to 2020: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}