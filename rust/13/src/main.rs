use relative_path::RelativePath;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let t: i64 = contents.lines().next().unwrap().parse().unwrap();
    let buses: Vec<(i64, i64)> = contents
        .lines()
        .nth(1)
        .unwrap()
        .split(',')
        .enumerate()
        .filter(|(_, x)| *x != "x")
        .map(|(i, x)| (i as i64, x.parse().unwrap()))
        .collect();

    let waits: Vec<(i64, i64)> = buses.iter().map(|(_, x)| (x - (t % x), *x)).collect();

    return waits.iter().min().unwrap().0 * waits.iter().min().unwrap().1;
}

fn part2(contents: String) -> i64 {
    let buses: Vec<(i64, i64)> = contents
        .lines()
        .nth(1)
        .unwrap()
        .split(',')
        .enumerate()
        .filter(|(_, x)| *x != "x")
        .map(|(i, x)| (i as i64, x.parse().unwrap()))
        .collect();

    let big_n: i64 = buses.iter().map(|(_, x)| x).product();
    let y_i: Vec<i64> = buses.iter().map(|(_, x)| big_n / x).collect();

    let z_i: Vec<i64> = buses
        .iter()
        .zip(y_i.clone())
        .map(|((_, x), y)| (euclids_extended_algorithm(*x, y).2 + *x) % *x)
        .collect();

    return buses
        .iter()
        .zip(y_i.clone())
        .zip(z_i.clone())
        .map(|(((i, x), y), z)| (x - i) * y * z)
        .sum::<i64>()
        % big_n;
}

fn euclids_extended_algorithm(a: i64, b: i64) -> (i64, i64, i64) {
    if b == 0 {
        return (a, 1, 0);
    } else {
        let (d, x, y) = euclids_extended_algorithm(b, a % b);
        return (d, y, x - y * (a / b));
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 295);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 1068781);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "13".to_string();

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
        "\nPart 1:\nEarliest bus ID * wait time: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nEarliest timestamp where buses depart at position offsets: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}