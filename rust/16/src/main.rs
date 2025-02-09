use regex::Regex;
use relative_path::RelativePath;
use std::collections::HashMap;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let range_re = Regex::new(r"(\d+)-(\d+)").unwrap();

    let range_lines = contents.split("\n\n").next().unwrap();
    let mut ranges: Vec<(i64, i64)> = Vec::new();

    for line in range_lines.lines() {
        for cap in range_re.captures_iter(line) {
            let start = cap[1].parse::<i64>().unwrap();
            let end = cap[2].parse::<i64>().unwrap();
            ranges.push((start, end));
        }
    }

    ranges.sort_by(|a, b| a.0.cmp(&b.0));

    let mut error_rate: i64 = 0;
    let nearby_tickets = contents.split("\n\n").nth(2).unwrap();
    for line in nearby_tickets.lines().skip(1) {
        for num in line.split(",") {
            let n = num.parse::<i64>().unwrap();
            let mut valid = false;

            let mut start_ix = 0;
            let mut end_ix = ranges.len() - 1;

            while start_ix < end_ix {

                let mid = (start_ix + end_ix) / 2;
                let range = ranges.get(mid).unwrap();

                if n < range.0 {
                    end_ix = mid - 1;
                } else if n > range.1 {
                    start_ix = mid + 1;
                } else {
                    valid = true;
                    break;
                }
            }

            if start_ix == end_ix {
                let range = ranges.get(start_ix).unwrap();
                if n >= range.0 && n <= range.1 {
                    valid = true;
                }
            }

            if !valid {
                error_rate += n;
            }
        }
    }

    return error_rate;
}

fn part2(contents: String) -> i64 {
    let range_re = Regex::new(r"(\d+)-(\d+)").unwrap();

    let range_lines = contents.split("\n\n").next().unwrap();
    let mut ranges: Vec<(i64, i64)> = Vec::new();
    let mut range_fields: HashMap<String, Vec<(i64, i64)>> = HashMap::new();

    for line in range_lines.lines() {
        let field = line.split(":").next().unwrap();
        let mut field_ranges: Vec<(i64, i64)> = Vec::new();
        for cap in range_re.captures_iter(line) {
            let start = cap[1].parse::<i64>().unwrap();
            let end = cap[2].parse::<i64>().unwrap();
            ranges.push((start, end));
            field_ranges.push((start, end));
        }

        range_fields.insert(field.to_string(), field_ranges);
    }

    ranges.sort_by(|a, b| a.0.cmp(&b.0));

    let nearby_tickets = contents.split("\n\n").nth(2).unwrap();
    let mut possible_fields: Vec<HashSet<String>> = Vec::new();
    for line in nearby_tickets.lines().skip(1) {
        let mut valid = false;
        for (ix, num) in line.split(",").enumerate() {
            if ix >= possible_fields.len() {
                possible_fields.push(HashSet::from_iter(range_fields.keys().map(|x| x.clone())));
            }

            let n = num.parse::<i64>().unwrap();
            valid = false;

            let mut start_ix = 0;
            let mut end_ix = ranges.len() - 1;

            while start_ix < end_ix {
                let mid = (start_ix + end_ix) / 2;
                let range = ranges.get(mid).unwrap();

                if n < range.0 {
                    end_ix = mid - 1;
                } else if n > range.1 {
                    start_ix = mid + 1;
                } else {
                    valid = true;
                    break;
                }
            }

            if start_ix == end_ix {
                let range = ranges.get(start_ix).unwrap();
                if n >= range.0 && n <= range.1 {
                    valid = true;
                }
            }

            if !valid {
                break;
            }
        }

        if !valid {
            continue;
        }

        for (ix, num) in line.split(",").enumerate() {
            let n = num.parse::<i64>().unwrap();

            for (field, field_ranges) in range_fields.iter() {
                let mut valid = false;
                for range in field_ranges {
                    if n >= range.0 && n <= range.1 {
                        valid = true;
                        break;
                    }
                }

                if !valid {
                    possible_fields[ix].remove(field);
                }
            }
        }
    }

    while possible_fields.iter().any(|x| x.len() > 1) {
        let mut found = HashSet::new();
        for field in possible_fields.iter() {
            if field.len() == 1 {
                found.insert(field.iter().next().unwrap().clone());
            }
        }

        for field in possible_fields.iter_mut() {
            if field.len() > 1 {
                field.retain(|x| !found.contains(x));
            }
        }
    }

    let my_ticket = contents
        .split("\n\n")
        .nth(1)
        .unwrap()
        .lines()
        .nth(1)
        .unwrap()
        .split(",")
        .map(|x| x.parse::<i64>().unwrap())
        .collect::<Vec<i64>>();

    if possible_fields.len() >= 6 {
        return possible_fields
            .iter()
            .enumerate()
            .filter(|(_, x)| x.iter().next().unwrap().starts_with("departure"))
            .map(|(ix, _)| my_ticket[ix])
            .product();
    } else {
        return possible_fields
            .iter()
            .enumerate()
            .map(|(ix, _)| my_ticket[ix])
            .product();
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("p1_example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 71);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("p2_example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 1716);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "16".to_string();

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
        "\nPart 1:\nTicket scanning error rate: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nProduct of departure fields: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}