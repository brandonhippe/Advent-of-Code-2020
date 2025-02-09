use relative_path::RelativePath;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let instructions: Vec<(&str, i64)> = contents
        .lines()
        .map(|x| {
            let mut split = x.split(" ");
            let op = split.next().unwrap();
            let arg = split.next().unwrap().parse::<i64>().unwrap();
            (op, arg)
        })
        .collect();

    return run_code(&instructions).0;
}

fn part2(contents: String) -> i64 {
    let instructions: Vec<(&str, i64)> = contents
        .lines()
        .map(|x| {
            let mut split = x.split(" ");
            let op = split.next().unwrap();
            let arg = split.next().unwrap().parse::<i64>().unwrap();
            (op, arg)
        })
        .collect();

    for i in 0..instructions.len() {
        let mut new_instructions = instructions.clone();
        match new_instructions[i].0 {
            "jmp" => new_instructions[i].0 = "nop",

            "nop" => new_instructions[i].0 = "jmp",
            _ => continue,
        }

        let (acc, finished) = run_code(&new_instructions);
        if finished {
            return acc;
        }
    }

    return -1;
}

fn run_code(instructions: &Vec<(&str, i64)>) -> (i64, bool) {
    let mut visited: HashSet<i64> = HashSet::new();
    let mut acc = 0;
    let mut pc = 0;
    while !visited.contains(&pc) && pc < instructions.len() as i64 {
        if pc as usize == instructions.len() {
            return (acc, true);
        }
        visited.insert(pc);
        let (op, arg) = instructions[pc as usize];
        match op {
            "acc" => {
                acc += arg;
                pc += 1;
            }
            "jmp" => {
                pc += arg;
            }
            "nop" => {
                pc += 1;
            }
            _ => panic!("Unknown op: {}", op),
        }
    }

    return (acc, pc as usize == instructions.len());
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 5);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 8);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "8".to_string();

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
        "\nPart 1:\nValue in accumulator before running any instruction twice: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nValue in accumulator before program terminates: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}