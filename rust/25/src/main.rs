use relative_path::RelativePath;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let public_keys: Vec<i64> = contents.lines().map(|line| line.parse().unwrap()).collect();

    let mut value = 7;
    let mut loop_size = 1;
    let mut public_key: i64 = 0;

    while public_key == 0 {
        value *= 7;
        value %= 20201227;

        loop_size += 1;

        for i in 0..2 {
            if public_keys[i] == value {
                public_key = public_keys[(i + 1) % 2];
                break;
            }
        }
    }

    let mut encryption_key = 1;

    for _ in 0..loop_size {
        encryption_key *= public_key;
        encryption_key %= 20201227;
    }

    return encryption_key;
}


fn part2(_contents: String) -> String {
    return "Christmas has been saved!".to_string();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents = "5764801\n17807724\n".to_string();

        assert_eq!(part1(contents), 14897079);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "25".to_string();

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
        "\nPart 1:\nEncryption key: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\n{}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}