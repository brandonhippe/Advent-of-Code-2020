use relative_path::RelativePath;
use std::collections::HashMap;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let mut space: HashMap<(i64, i64, i64), i64> = HashMap::new();
    for (y, line) in contents.lines().enumerate() {
        for (x, c) in line.chars().enumerate() {
            if c == '#' {
                space.insert((x as i64, y as i64, 0), 1);
            }
        }
    }

    for _ in 0..6 {
        let mut new_space: HashMap<(i64, i64, i64), i64> = HashMap::new();
        for pos in space.keys() {
            for x in -1..=1 {
                for y in -1..=1 {
                    for z in -1..=1 {
                        if x == 0 && y == 0 && z == 0 {
                            continue;
                        }

                        let new_pos = (pos.0 + x, pos.1 + y, pos.2 + z);
                        new_space
                            .get_mut(&new_pos)
                            .map(|v| *v += 1)
                            .unwrap_or_else(|| {
                                new_space.insert(new_pos, 1);
                            });
                    }
                }

            }
        }

        new_space.retain(|k, v| *v == 3 || (space.contains_key(k) && *v == 2));
        space = new_space;
    }

    return space.len() as i64;
}

fn part2(contents: String) -> i64 {
    let mut space: HashMap<(i64, i64, i64, i64), i64> = HashMap::new();
    for (y, line) in contents.lines().enumerate() {
        for (x, c) in line.chars().enumerate() {
            if c == '#' {
                space.insert((x as i64, y as i64, 0, 0), 1);
            }
        }
    }

    for _ in 0..6 {
        let mut new_space: HashMap<(i64, i64, i64, i64), i64> = HashMap::new();
        for pos in space.keys() {
            for x in -1..=1 {
                for y in -1..=1 {
                    for z in -1..=1 {
                        for w in -1..=1 {
                            if x == 0 && y == 0 && z == 0 && w == 0 {
                                continue;
                            }

                            let new_pos = (pos.0 + x, pos.1 + y, pos.2 + z, pos.3 + w);
                            new_space
                                .get_mut(&new_pos)
                                .map(|v| *v += 1)
                                .unwrap_or_else(|| {
                                    new_space.insert(new_pos, 1);
                                });
                        }
                    }
                }
            }
        }

        new_space.retain(|k, v| *v == 3 || (space.contains_key(k) && *v == 2));
        space = new_space;
    }

    return space.len() as i64;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 112);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 848);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "17".to_string();

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
        "\nPart 1:\n3d Cubes on after 6 cycles: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\n4d Hypercubes on after 6 cycles: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}