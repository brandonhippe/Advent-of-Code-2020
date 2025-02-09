use relative_path::RelativePath;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    return contents
        .lines()
        .map(|line| {
            line.chars()
                .map(|c| match c {
                    'F' | 'L' => 0,
                    'B' | 'R' => 1,
                    _ => panic!("Invalid character"),
                })
                .fold(0, |acc, x| (acc << 1) | x)
        })
        .max()
        .unwrap();
}

fn part2(contents: String) -> i64 {
    let mut seats: HashSet<i64> = HashSet::new();
    let mut min_seat = i64::MAX;
    let mut max_seat = i64::MIN;
    for line in contents.lines() {
        let seat: i64 = line
            .chars()
            .map(|c| match c {
                'F' | 'L' => 0,
                'B' | 'R' => 1,
                _ => panic!("Invalid character"),
            })
            .fold(0, |acc, x| (acc << 1) | x);


        min_seat = min_seat.min(seat);
        max_seat = max_seat.max(seat);
        seats.insert(seat);
    }

    let all_seats: HashSet<i64> = (min_seat..=max_seat).collect();

    return *all_seats.difference(&seats).next().unwrap();
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "5".to_string();

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
        "\nPart 1:\nHighest seat id: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nYour seat id: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}