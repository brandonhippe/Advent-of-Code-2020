use relative_path::RelativePath;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let mut pos = (0, 0);
    let mut dir = (1, 0);

    for line in contents.lines() {
        let amt = line[1..].parse::<i64>().unwrap();
        match line.chars().nth(0).unwrap() {
            'N' => pos.1 += amt,
            'S' => pos.1 -= amt,
            'E' => pos.0 += amt,
            'W' => pos.0 -= amt,
            'L' => {
                let (x, y) = dir;
                match amt {
                    90 => dir = (-y, x),
                    180 => dir = (-x, -y),
                    270 => dir = (y, -x),
                    _ => panic!("Invalid angle"),
                }
            }
            'R' => {
                let (x, y) = dir;
                match amt {
                    90 => dir = (y, -x),
                    180 => dir = (-x, -y),
                    270 => dir = (-y, x),
                    _ => panic!("Invalid angle"),
                }
            }
            'F' => {

                pos.0 += dir.0 * amt;
                pos.1 += dir.1 * amt;
            }
            _ => panic!("Invalid instruction"),
        }
    }

    return pos.0.abs() + pos.1.abs();
}

fn part2(contents: String) -> i64 {
    let mut way = (10, 1);
    let mut pos = (0, 0);

    for line in contents.lines() {
        let amt = line[1..].parse::<i64>().unwrap();

        match line.chars().nth(0).unwrap() {
            'N' => way.1 += amt,
            'S' => way.1 -= amt,
            'E' => way.0 += amt,
            'W' => way.0 -= amt,
            'L' => {
                let (x, y) = way;
                match amt {
                    90 => way = (-y, x),
                    180 => way = (-x, -y),
                    270 => way = (y, -x),
                    _ => panic!("Invalid angle"),
                }
            }
            'R' => {
                let (x, y) = way;
                match amt {
                    90 => way = (y, -x),
                    180 => way = (-x, -y),
                    270 => way = (-y, x),
                    _ => panic!("Invalid angle"),
                }
            }
            'F' => {
                pos.0 += way.0 * amt;
                pos.1 += way.1 * amt;
            }
            _ => panic!("Invalid instruction"),
        }
    }

    return pos.0.abs() + pos.1.abs();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 25);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 286);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "12".to_string();

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
        "\nPart 1:\nDistance from origin: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nDistance from origin: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}