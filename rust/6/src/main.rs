use relative_path::RelativePath;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let mut count = 0;
    let mut group = HashSet::new();

    for line in contents.lines() {
        if line.is_empty() {
            count += group.len() as i64;
            group.clear();
        } else {
            for c in line.chars() {
                group.insert(c);
            }
        }
    }

    count += group.len() as i64;

    return count;
}

fn part2(contents: String) -> i64 {
    let mut count = 0;

    for group in contents.split("\n\n") {
        let mut group_set: HashSet<char> = group.chars().filter(|c| !c.is_whitespace()).collect();
        for person in group.lines() {
            let person_set: HashSet<char> = person.chars().collect();
            group_set = group_set.intersection(&person_set).cloned().collect();
        }


        count += group_set.len() as i64;
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

        assert_eq!(part1(contents), 11);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 6);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "6".to_string();

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
        "\nPart 1:\nQuestions answered yes to by a group: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nQuestions answered yes to by everyone in a group: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}