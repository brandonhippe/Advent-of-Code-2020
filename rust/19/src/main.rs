use relative_path::RelativePath;
use std::env;
use std::fs;
use std::time::Instant;
use regex::Regex;
use std::collections::HashMap;

fn part1(contents: String) -> i64 {
    let mut rules: HashMap<i64, String> = HashMap::new();
    for line in contents.lines() {
        if line == "" {
            break;
        }
        
        let rule_num = line.split(":").next().unwrap().parse::<usize>().unwrap();
        let rule = line.split(":").nth(1).unwrap().trim().to_string().replace("\"", "");

        if rule.find("|").is_some() {
            rules.insert(rule_num as i64, format!("({})", rule));
        } else {
            rules.insert(rule_num as i64, rule);
        }
    }

    let rule_0 = Regex::new(format!("^{}$", build_rule(rules.clone(), 0, false).replace(" ", "").as_str()).as_str()).unwrap();
    return contents.split("\n\n").nth(1).unwrap().lines().filter(|line| rule_0.is_match(line)).count() as i64;
}

fn part2(contents: String) -> i64 {
    let mut rules: HashMap<i64, String> = HashMap::new();
    for line in contents.lines() {
        if line == "" {
            break;
        }
        

        let rule_num = line.split(":").next().unwrap().parse::<usize>().unwrap();
        let rule = line.split(":").nth(1).unwrap().trim().to_string().replace("\"", "");
        
        if rule.find("|").is_some() {
            rules.insert(rule_num as i64, format!("({})", rule));
        } else {
            rules.insert(rule_num as i64, rule);
        }
    }
    
    let rule_0 = format!("^{}$", build_rule(rules.clone(), 0, true).replace(" ", "").as_str());
    let mut n = 1;
    let mut count = 0;
    loop {
        let rule_0 = Regex::new(rule_0.replace("n", n.to_string().as_str()).as_str()).unwrap();
        let matches = contents.split("\n\n").nth(1).unwrap().lines().filter(|line| rule_0.is_match(line)).count();
        if matches == 0 {
            break;
        }
        count += matches;
        n += 1;
    }

    return count as i64;
}

fn build_rule(rules: HashMap<i64, String>, rule_num: i64, self_ref: bool) -> String {
    let rule = rules.get(&rule_num).unwrap();
    let mut new_rule = rule.clone();

    if rule.contains("a") || rule.contains("b") {
        return new_rule.replace("(", "").replace(")", "");
    }

    for part in rule.split(" ") {
        if part != "|" {
            let part_num = part.replace('(', "").replace(')', "").parse::<i64>().unwrap();
            let part_num_regex = Regex::new(format!("[( ]?({})[ )]?", part_num).as_str()).unwrap();
            let part_rule = build_rule(rules.clone(), part_num, self_ref);

            let cap = part_num_regex.captures_iter(&new_rule).next().unwrap();
            let start = cap.get(1).unwrap().start();
            let end = cap.get(1).unwrap().end();

            if self_ref && rule_num == 11 {
                new_rule = format!("{}({}){{n}}{}", &new_rule[..start], part_rule, &new_rule[end..]);
            } else {
                new_rule = format!("{}{}{}", &new_rule[..start], part_rule, &new_rule[end..]);
            }
        }
    }

    if self_ref && rule_num == 8 {
        return format!("({})+", new_rule.replace(" ", ""));
    }

    return format!("{}", new_rule.replace(" ", ""));
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 3);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 12);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "19".to_string();

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
        "\nPart 1:\nNumber of strings that match rule 0: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nNumber of strings that match rule 0: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}