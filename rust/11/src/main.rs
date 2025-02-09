use relative_path::RelativePath;
use std::collections::HashMap;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let mut available: HashSet<(i64, i64)> = HashSet::new();
    let mut filled: HashSet<(i64, i64)> = HashSet::new();

    for (y, line) in contents.lines().enumerate() {
        for (x, c) in line.chars().enumerate() {
            match c {
                'L' => {
                    available.insert((x as i64, y as i64));
                }
                '#' => {
                    filled.insert((x as i64, y as i64));
                }
                _ => (),
            }
        }
    }

    loop {
        let (new_available, new_filled) = iterate(available.clone(), filled.clone(), false);
        if available
            .symmetric_difference(&new_available)
            .collect::<Vec<_>>()
            .len()
            == 0
        {
            break;
        }


        available = new_available;
        filled = new_filled;
    }

    return filled.len() as i64;
}

fn part2(contents: String) -> i64 {
    let mut available: HashSet<(i64, i64)> = HashSet::new();
    let mut filled: HashSet<(i64, i64)> = HashSet::new();

    for (y, line) in contents.lines().enumerate() {
        for (x, c) in line.chars().enumerate() {
            match c {
                'L' => {
                    available.insert((x as i64, y as i64));
                }
                '#' => {
                    filled.insert((x as i64, y as i64));
                }
                _ => (),
            }
        }
    }

    loop {
        let (new_available, new_filled) = iterate(available.clone(), filled.clone(), true);
        if available
            .symmetric_difference(&new_available)
            .collect::<Vec<_>>()
            .len()
            == 0
        {
            break;
        }

        available = new_available;
        filled = new_filled;
    }

    return filled.len() as i64;
}

fn iterate(
    available: HashSet<(i64, i64)>,
    filled: HashSet<(i64, i64)>,
    p2: bool,
) -> (HashSet<(i64, i64)>, HashSet<(i64, i64)>) {
    let mut new_available: HashSet<(i64, i64)> = HashSet::new();
    let mut new_filled: HashSet<(i64, i64)> = HashSet::new();

    let mut neighbor_counts: HashMap<(i64, i64), i64> =
        HashMap::from_iter(available.symmetric_difference(&filled).map(|&pos| (pos, 0)));
    let min_x: i64 = *neighbor_counts.keys().map(|(x, _)| x).min().unwrap();
    let max_x: i64 = *neighbor_counts.keys().map(|(x, _)| x).max().unwrap();
    let min_y: i64 = *neighbor_counts.keys().map(|(_, y)| y).min().unwrap();
    let max_y: i64 = *neighbor_counts.keys().map(|(_, y)| y).max().unwrap();

    for pos in filled.iter() {
        for y_offset in -1..=1 {
            for x_offset in -1..=1 {
                if y_offset == 0 && x_offset == 0 {
                    continue;
                }

                let mut n_pos = (pos.0 + x_offset, pos.1 + y_offset);
                while p2
                    && !(neighbor_counts.contains_key(&n_pos))
                    && n_pos.0 >= min_x
                    && n_pos.0 <= max_x
                    && n_pos.1 >= min_y
                    && n_pos.1 <= max_y
                {
                    n_pos = (n_pos.0 + x_offset, n_pos.1 + y_offset);
                }

                if neighbor_counts.contains_key(&n_pos) {
                    *neighbor_counts.get_mut(&n_pos).unwrap() += 1;
                }
            }
        }
    }

    for (pos, count) in neighbor_counts.iter() {
        if available.contains(pos) {
            if *count == 0 {
                new_filled.insert(*pos);
            } else {
                new_available.insert(*pos);
            }
        } else if filled.contains(pos) {
            if (!p2 && *count >= 4) || (p2 && *count >= 5) {
                new_available.insert(*pos);
            } else {
                new_filled.insert(*pos);
            }
        }
    }

    return (new_available, new_filled);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 37);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 26);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "11".to_string();

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
        "\nPart 1:\nNumber of filled seats: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nNumber of filled seats: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}