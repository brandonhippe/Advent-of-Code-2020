use relative_path::RelativePath;
use std::collections::VecDeque;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let mut cups = VecDeque::new();
    for c in contents.chars() {
        cups.push_back(c.to_digit(10).unwrap() as i64);
    }

    let num_cups = cups.len() as i64;

    for _ in 0..100 {
        let mut target = cups[0] - 1;
        if target == 0 {
            target = num_cups;
        }

        cups.rotate_left(1);
        let c1 = cups.pop_front().unwrap();
        let c2 = cups.pop_front().unwrap();
        let c3 = cups.pop_front().unwrap();

        while c1 == target || c2 == target || c3 == target {
            target -= 1;
            if target == 0 {
                target = num_cups;
            }
        }

        let rotate_amt = (cups.iter().position(|&x| x == target).unwrap() + 1) % num_cups as usize;
        cups.rotate_left(rotate_amt);
        cups.push_front(c3);

        cups.push_front(c2);
        cups.push_front(c1);
        cups.rotate_right(rotate_amt);
    }

    let rotate_amt = (cups.iter().position(|&x| x == 1).unwrap()) % num_cups as usize;
    cups.rotate_left(rotate_amt);

    return cups.iter().skip(1).fold(0, |acc, x| acc * 10 + x);
}

fn part2(contents: String) -> i64 {
    let mut cups: Vec<i64> = vec![0; 10_000_001];
    let input: Vec<i64> = Vec::from_iter(contents.chars().map(|c| c.to_digit(10).unwrap() as i64));

    cups[0] = input[0];
    for i in 1..=input.len() {
        let index = input.iter().position(|&x| x == i as i64).unwrap();
        cups[i] = if index == input.len() - 1 {
            10
        } else {
            input[index + 1]
        };
    }

    for i in 10..1_000_000 {
        cups[i] = i as i64 + 1;
    }

    cups[1_000_000] = input[0];

    for _ in 0..10_000_000 {
        let current = cups[0] as usize;
        let held = cups[current];
        let mut after_held = held;
        let mut removed: Vec<i64> = Vec::new();
        for _ in 0..3 {
            removed.push(after_held);
            after_held = cups[after_held as usize];
        }

        let mut dest = cups[0] + 999_998;
        dest %= 1_000_000;
        while removed.contains(&(dest + 1)) {
            dest += 999_999;
            dest %= 1_000_000;
        }

        let after_dest = cups[(dest + 1) as usize];

        cups[(dest + 1) as usize] = held;
        cups[removed[2] as usize] = after_dest;

        cups[current] = after_held;
        cups[0] = after_held;
    }

    return cups[1] * cups[cups[1] as usize];
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents = "389125467".to_string();

        assert_eq!(part1(contents), 67384529);
    }

    #[test]
    fn p2_test() {
        let contents = "389125467".to_string();

        assert_eq!(part2(contents), 149245887792);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "23".to_string();

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
        "\nPart 1:\nOrder of cups after 1: {}\nRan in {:.5?}",
        part1(contents.clone().lines().next().unwrap().to_string()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nProduct of two cups after 1: {}\nRan in {:.5?}",
        part2(contents.clone().lines().next().unwrap().to_string()),
        part2_timer.elapsed()
    );
}