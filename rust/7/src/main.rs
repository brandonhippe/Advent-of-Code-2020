use regex::Regex;
use relative_path::RelativePath;
use std::collections::HashMap;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let bag_re = Regex::new(r"(\w+ \w+) bags contain").unwrap();
    let bag_contents_re = Regex::new(r"(\d+) (\w+ \w+) bags?").unwrap();

    let mut bags: HashMap<String, Bag> = HashMap::new();
    for line in contents.lines() {
        let bag_name = bag_re
            .captures(line)
            .unwrap()
            .get(1)
            .unwrap()
            .as_str()
            .to_string();
        let mut contents = Vec::new();
        for cap in bag_contents_re.captures_iter(line) {
            let count = cap.get(1).unwrap().as_str().parse::<i64>().unwrap();
            let name = cap.get(2).unwrap().as_str().to_string();
            contents.push((count, name));
        }
        bags.insert(bag_name.clone(), Bag::new(bag_name, contents));
    }

    return bags
        .values()
        .filter(|bag| bag.can_contain("shiny gold", &bags))
        .count() as i64;
}


fn part2(contents: String) -> i64 {
    let bag_re = Regex::new(r"(\w+ \w+) bags contain").unwrap();
    let bag_contents_re = Regex::new(r"(\d+) (\w+ \w+) bags?").unwrap();

    let mut bags: HashMap<String, Bag> = HashMap::new();
    for line in contents.lines() {
        let bag_name = bag_re
            .captures(line)
            .unwrap()
            .get(1)
            .unwrap()
            .as_str()
            .to_string();
        let mut contents = Vec::new();
        for cap in bag_contents_re.captures_iter(line) {
            let count = cap.get(1).unwrap().as_str().parse::<i64>().unwrap();
            let name = cap.get(2).unwrap().as_str().to_string();
            contents.push((count, name));
        }
        bags.insert(bag_name.clone(), Bag::new(bag_name, contents));
    }

    return bags.get("shiny gold").unwrap().total_contain(&bags);
}

#[derive(Debug, Clone)]
struct Bag {
    name: String,
    contents: Vec<(i64, String)>,
}

impl Bag {
    fn new(name: String, contents: Vec<(i64, String)>) -> Bag {
        Bag { name, contents }
    }

    fn can_contain(&self, bag: &str, bags: &HashMap<String, Bag>) -> bool {
        for (_, name) in &self.contents {
            if name == bag {
                return true;
            }
            if bags.get(name).unwrap().can_contain(bag, bags) {
                return true;
            }
        }

        return false;
    }

    fn total_contain(&self, bags: &HashMap<String, Bag>) -> i64 {
        let mut count = 0;
        for (num, name) in &self.contents {
            count += num + num * bags.get(name).unwrap().total_contain(bags);
        }

        return count;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 4);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 32);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "7".to_string();

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
        "\nPart 1:\nNumber of bags that can contain a shiny gold bag: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nNumber of bags a shiny gold bag can contain: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}