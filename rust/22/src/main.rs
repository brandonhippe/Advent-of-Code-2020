use relative_path::RelativePath;
use std::collections::HashSet;
use std::collections::VecDeque;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let mut p1 = VecDeque::new();
    let mut p2 = VecDeque::new();
    let mut current: &mut VecDeque<i64> = &mut p1;
    for line in contents.lines() {
        if line.starts_with("Player") {
            continue;
        }

        if line.is_empty() {
            current = &mut p2;
            continue;
        }

        current.push_back(line.parse().unwrap());
    }

    return combat(p1, p2, false).into_iter().sum::<i64>();
}

fn part2(contents: String) -> i64 {
    let mut p1 = VecDeque::new();
    let mut p2 = VecDeque::new();
    let mut current: &mut VecDeque<i64> = &mut p1;
    for line in contents.lines() {
        if line.starts_with("Player") {
            continue;
        }


        if line.is_empty() {
            current = &mut p2;
            continue;
        }

        current.push_back(line.parse().unwrap());
    }

    return combat(p1, p2, true).into_iter().sum::<i64>();
}

fn combat(mut p1: VecDeque<i64>, mut p2: VecDeque<i64>, recursive: bool) -> Vec<i64> {
    let mut seen: HashSet<(VecDeque<i64>, VecDeque<i64>)> = HashSet::new();

    while p1.len() > 0 && p2.len() > 0 && !seen.contains(&(p1.clone(), p2.clone())) {
        seen.insert((p1.clone(), p2.clone()));
        let c1 = p1.pop_front().unwrap();
        let c2 = p2.pop_front().unwrap();

        if recursive && p1.len() as i64 >= c1 && p2.len() as i64 >= c2 {
            let sub_p1 = p1.iter().take(c1 as usize).cloned().collect();
            let sub_p2 = p2.iter().take(c2 as usize).cloned().collect();
            let scores = combat(sub_p1, sub_p2, true);

            if scores[0] > scores[1] {
                p1.push_back(c1);
                p1.push_back(c2);
            } else {
                p2.push_back(c2);
                p2.push_back(c1);
            }
        } else {
            if c1 > c2 {
                p1.push_back(c1);
                p1.push_back(c2);
            } else {
                p2.push_back(c2);
                p2.push_back(c1);
            }
        }
    }

    if seen.contains(&(p1.clone(), p2.clone())) {
        p1.extend(p2.clone());
        p2.clear();
    }

    return vec![
        p1.iter()
            .enumerate()
            .map(|(i, v)| v * (p1.len() - i) as i64)
            .sum(),
        p2.iter()
            .enumerate()
            .map(|(i, v)| v * (p2.len() - i) as i64)
            .sum(),
    ];
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 306);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 291);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "22".to_string();

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
        "\nPart 1:\nCombat Winner's Score: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nRecursive Combat Winner's Score: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}