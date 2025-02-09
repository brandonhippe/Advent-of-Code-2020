use relative_path::RelativePath;
use std::env;
use std::fs;
use std::time::Instant;
use std::collections::HashMap;

fn part1(contents: String) -> i64 {
    let mut sum: i64 = 0;
    for line in contents.lines() {
        let mut occurrances: HashMap<i64, i64> = HashMap::from_iter(line.split(",").enumerate().map(|(i, x)| (x.parse::<i64>().unwrap(), i as i64)).collect::<Vec<_>>().iter().cloned());
        let mut last = line.split(",").last().unwrap().parse::<i64>().unwrap();

        for i in occurrances.len() as i64..2020 {
            let next = match occurrances.get(&last) {
                Some(x) => i - x - 1,
                None => 0,
            };

            occurrances.insert(last, i - 1);
            last = next;
        }

        sum += last;
    }

    return sum;
}

fn part2(contents: String) -> i64 {
    let mut sum: i64 = 0;
    for line in contents.lines() {
        let mut occurrances: HashMap<i64, i64> = HashMap::from_iter(line.split(",").enumerate().map(|(i, x)| (x.parse::<i64>().unwrap(), i as i64)).collect::<Vec<_>>().iter().cloned());
        let mut last = line.split(",").last().unwrap().parse::<i64>().unwrap();

        for i in occurrances.len() as i64..30000000 {

            let next = match occurrances.get(&last) {
                Some(x) => i - x - 1,
                None => 0,
            };

            occurrances.insert(last, i - 1);
            last = next;
        }

        sum += last;
    }

    return sum;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let expected = vec![436, 1, 10, 27, 78, 438, 1836];
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        for (line, res) in contents.lines().zip(expected.iter()) {
            assert_eq!(part1(line.to_string()), *res);
        }
    }

    #[test]
    fn p2_test() {
        let expected = vec![175594, 2578, 3544142, 261214, 6895259, 18, 362];
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        for (line, res) in contents.lines().zip(expected.iter()) {
            assert_eq!(part2(line.to_string()), *res);
        }
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "15".to_string();

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
        "\nPart 1:\n2020th number spoken: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\n30000000th number spoken: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}