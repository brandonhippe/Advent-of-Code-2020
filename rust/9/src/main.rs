use relative_path::RelativePath;
use std::collections::HashMap;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String, preamble_len: i64) -> i64 {
    return find_invalid_number(
        contents
            .lines()
            .map(|x| x.parse::<i64>().unwrap())
            .collect(),
        preamble_len,
    );
}

fn part2(contents: String, preamble_len: i64) -> i64 {
    let numbers: Vec<i64> = contents
        .lines()
        .map(|x| x.parse::<i64>().unwrap())
        .collect();
    let invalid_number = find_invalid_number(numbers.clone(), preamble_len);

    if invalid_number == -1 {
        return -1;
    }

    for window_size in 3..numbers.len() {
        for nums in numbers.windows(window_size) {
            if nums.iter().sum::<i64>() == invalid_number {
                return nums.iter().min().unwrap() + nums.iter().max().unwrap();
            }
        }
    }


    return 0;
}

fn find_invalid_number(numbers: Vec<i64>, preamble_len: i64) -> i64 {
    let mut contributions: HashMap<i64, HashSet<i64>> = HashMap::new();
    let mut sums: HashMap<i64, HashSet<(i64, i64)>> = HashMap::new();
    for n in numbers[0..preamble_len as usize].iter() {
        contributions.insert(*n, HashSet::new());

        for m in contributions.clone().keys() {
            if *m != *n {
                let sum: i64 = *m + *n;
                sums.entry(sum).or_insert(HashSet::new()).insert((*m, *n));

                contributions.get_mut(n).unwrap().insert(sum);
                contributions.get_mut(m).unwrap().insert(sum);
            }
        }
    }

    for (i, n) in numbers[preamble_len as usize..].iter().enumerate() {
        if !sums.contains_key(n) {
            return *n;
        }

        let removing: i64 = numbers[i as usize];
        for sum in contributions.clone().get(&removing).unwrap() {
            let other = sum - removing;
            let sum_set = sums.get_mut(sum).unwrap();
            if sum_set.contains(&(removing, other)) {
                sum_set.remove(&(removing, other));
            } else {
                sum_set.remove(&(other, removing));
            }

            contributions.get_mut(&other).unwrap().remove(sum);

            if sum_set.len() == 0 {
                sums.remove(sum);
            }
        }

        contributions.remove(&removing);
        contributions.insert(*n, HashSet::new());

        for m in contributions.clone().keys() {
            if *m != *n {
                let sum: i64 = *m + *n;
                sums.entry(sum).or_insert(HashSet::new()).insert((*m, *n));

                contributions.get_mut(n).unwrap().insert(sum);
                contributions.get_mut(m).unwrap().insert(sum);
            }
        }
    }

    println!("{:?}", contributions);
    println!("{:?}", sums);

    return -1;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents, 5), 127);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents, 5), 62);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "9".to_string();

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
        "\nPart 1:\nInvalid number: {}\nRan in {:.5?}",
        part1(contents.clone(), 25),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nEncryption weakness: {}\nRan in {:.5?}",
        part2(contents.clone(), 25),
        part2_timer.elapsed()
    );
}