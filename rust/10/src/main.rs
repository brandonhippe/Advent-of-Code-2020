use cached::proc_macro::cached;
use relative_path::RelativePath;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let mut nums: Vec<i64> = contents
        .lines()
        .map(|x| x.parse::<i64>().unwrap())
        .collect();
    nums.push(0);
    nums.sort();
    nums.push(nums[nums.len() - 1] + 3);

    let mut ones = 0;
    let mut threes = 0;
    for (n1, n2) in nums.windows(2).map(|x| (x[0], x[1])) {
        if n2 - n1 == 1 {
            ones += 1;
        } else if n2 - n1 == 3 {
            threes += 1;
        }
    }

    return ones * threes;
}

fn part2(contents: String) -> i64 {
    let mut nums: Vec<i64> = contents
        .lines()
        .map(|x| x.parse::<i64>().unwrap())
        .collect();
    nums.push(0);
    nums.sort();

    nums.push(nums[nums.len() - 1] + 3);

    let mut arrangements: i64 = 1;
    let mut current_string: Vec<i64> = vec![0];
    for ix in 1..nums.len() {
        if nums[ix] - nums[ix - 1] == 3 {
            arrangements *= count_arrangements(current_string.clone());
            current_string = vec![nums[ix]];
        } else {
            current_string.push(nums[ix]);
        }
    }

    return arrangements;
}

#[cached]
fn count_arrangements(nums: Vec<i64>) -> i64 {
    if nums.len() == 1 {
        return 1;
    }

    let mut count = 0;
    for i in 1..nums.len() {
        if nums[i] - nums[0] <= 3 {
            count += count_arrangements(nums[i..].to_vec());
        } else {
            break;
        }
    }

    return count;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 220);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 19208);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "10".to_string();

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
        "\nPart 1:\nDifferences of 1 * Differences of 3: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nNumber of paths: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}