use relative_path::RelativePath;
use std::collections::HashMap;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let mut tiles: HashSet<(i64, i64)> = HashSet::new();

    for line in contents.lines() {
        let mut pos: (i64, i64) = (0, 0);
        let mut p_char: char = 'e';

        for c in line.chars() {
            match c {
                'n' => pos.1 -= 1,
                's' => pos.1 += 1,
                'e' => {
                    if p_char != 's' {
                        pos.0 += 1;
                    }
                }
                'w' => {
                    if p_char != 'n' {
                        pos.0 -= 1;
                    }
                }
                _ => panic!("Invalid character"),
            }

            p_char = c;
        }

        if tiles.contains(&pos) {

            tiles.remove(&pos);
        } else {
            tiles.insert(pos);
        }
    }

    return tiles.len() as i64;
}

fn part2(contents: String) -> i64 {
    let mut tiles: HashSet<(i64, i64)> = HashSet::new();

    for line in contents.lines() {
        let mut pos: (i64, i64) = (0, 0);
        let mut p_char: char = 'e';

        for c in line.chars() {
            match c {
                'n' => pos.1 -= 1,
                's' => pos.1 += 1,
                'e' => {
                    if p_char != 's' {
                        pos.0 += 1;
                    }
                }
                'w' => {
                    if p_char != 'n' {
                        pos.0 -= 1;
                    }
                }
                _ => panic!("Invalid character"),
            }

            p_char = c;
        }

        if tiles.contains(&pos) {
            tiles.remove(&pos);
        } else {
            tiles.insert(pos);
        }
    }

    for _ in 0..100 {
        let mut new_tiles: HashMap<(i64, i64), i64> = HashMap::new();

        for pos in tiles.iter() {
            for adj in vec![(1, -1), (0, -1), (1, 0), (-1, 0), (0, 1), (-1, 1)].iter() {
                let new_pos = (pos.0 + adj.0, pos.1 + adj.1);

                if !new_tiles.contains_key(&new_pos) {
                    new_tiles.insert(new_pos, 0);
                }

                *new_tiles.get_mut(&new_pos).unwrap() += 1;
            }
        }

        tiles = new_tiles
            .iter()
            .filter_map(|(pos, count)| {
                if tiles.contains(pos) {
                    if *count == 1 || *count == 2 {
                        return Some(*pos);
                    }
                } else {
                    if *count == 2 {
                        return Some(*pos);
                    }
                }

                return None;
            })
            .collect();
    }

    return tiles.len() as i64;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 10);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 2208);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "24".to_string();

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
        "\nPart 1:\nBlack tiles: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nBlack tiles after 100 days: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}