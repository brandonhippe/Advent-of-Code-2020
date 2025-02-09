use cached::proc_macro::cached;
use relative_path::RelativePath;
use std::collections::HashMap;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    return determine_allergens(contents).0;
}

fn part2(contents: String) -> String {
    return determine_allergens(contents).1;
}

#[cached]
fn determine_allergens(contents: String) -> (i64, String) {
    let mut allergen_map: HashMap<String, HashSet<String>> = HashMap::new();
    let mut ingredient_occurrances: HashMap<String, i64> = HashMap::new();
    for line in contents.lines() {
        let allergens: Vec<String> = line.split(" (contains ").collect::<Vec<&str>>()[1]
            .split(", ")
            .map(|x| x.replace(')', "").to_string())
            .collect();

        let ingredients: HashSet<String> = line.split(" (contains ").collect::<Vec<&str>>()[0]
            .split(" ")
            .map(|x| x.to_string())
            .collect();

        for allergen in allergens {
            if allergen_map.contains_key(&allergen) {
                let current_ingredients = allergen_map.get_mut(&allergen).unwrap();
                let new_ingredients = current_ingredients

                    .intersection(&ingredients)
                    .cloned()
                    .collect();
                *current_ingredients = new_ingredients;
            } else {
                allergen_map.insert(allergen.clone(), ingredients.clone());
            }
        }

        for ingredient in ingredients {
            if ingredient_occurrances.contains_key(&ingredient) {
                *ingredient_occurrances.get_mut(&ingredient).unwrap() += 1;
            } else {
                ingredient_occurrances.insert(ingredient, 1);
            }
        }
    }

    while allergen_map.values().any(|x| x.len() > 1) {
        let known_ingredients: HashSet<String> = allergen_map
            .values()
            .filter(|x| x.len() == 1)
            .map(|x| x.iter().next().unwrap().clone())
            .collect();
        for (_, ingredients) in allergen_map.iter_mut() {
            if ingredients.len() > 1 {
                *ingredients = ingredients
                    .difference(&known_ingredients)
                    .cloned()
                    .collect();
            }
        }
    }

    let mut dangerous_ingredients: Vec<_> = allergen_map.iter().collect();
    dangerous_ingredients.sort_by(|a, b| a.0.cmp(b.0));
    let dangerous_ingredients: Vec<String> = dangerous_ingredients
        .iter()
        .map(|x| x.1.iter().next().unwrap().clone())
        .collect();

    return (
        ingredient_occurrances
            .iter()
            .filter(|(k, _)| !dangerous_ingredients.contains(k))
            .map(|(_, v)| *v)
            .sum(),
        dangerous_ingredients.join(","),
    );
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

        assert_eq!(part2(contents), "mxmxvkd,sqjhc,fvjkl".to_string());
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "21".to_string();

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
        "\nPart 1:\nOccurrances of safe ingredients: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nDangerous ingredient list: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}