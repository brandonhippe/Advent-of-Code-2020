use relative_path::RelativePath;
use std::env;
use std::fs;
use std::time::Instant;
use std::collections::HashSet;

fn part1(contents: String) -> i64 {
    let trees: HashSet<(i64, i64)> = contents
        .split("\n")
        .enumerate()
        .flat_map(|(y, line)| {
            line.chars().enumerate().filter_map(move |(x, c)| {
                if c == '#' {
                    Some((x as i64, y as i64))
                } else {
                    None
                }
            })
        })
        .collect();

    let x_max = contents.lines().next().unwrap().len() as i64;
    let y_max = contents.lines().count() as i64;

    return trees_hit(trees, 3, 1, x_max, y_max);
}

fn part2(contents: String) -> i64 {
    let trees: HashSet<(i64, i64)> = contents
        .split("\n")
        .enumerate()
        .flat_map(|(y, line)| {
            line.chars().enumerate().filter_map(move |(x, c)| {
                if c == '#' {
                    Some((x as i64, y as i64))

                } else {
                    None
                }
            })
        })
        .collect();

    let x_max = contents.lines().next().unwrap().len() as i64;
    let y_max = contents.lines().count() as i64;

    return [(1, 1), (3, 1), (5, 1), (7, 1), (1, 2)]
        .iter()
        .map(|(right, down)| trees_hit(trees.clone(), *right, *down, x_max, y_max))
        .product();
}

fn trees_hit(trees: HashSet<(i64, i64)>, right: i64, down: i64, x_max: i64, y_max: i64) -> i64 {
    let mut x = 0;
    let mut y = 0;
    let mut count = 0;

    while y < y_max {
        if trees.contains(&(x, y)) {
            count += 1;
        }
        x = (x + right) % x_max;
        y += down;
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

        assert_eq!(part1(contents), 7);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 336);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "3".to_string();

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
        "\nPart 1:\nTrees hit: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nProduct of trees hit: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}