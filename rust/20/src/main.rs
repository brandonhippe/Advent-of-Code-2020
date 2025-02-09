use fancy_regex::Regex;
use relative_path::RelativePath;
use std::collections::HashMap;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let mut tile_num = 0;
    let mut tiles: HashMap<i64, Vec<String>> = HashMap::new();

    for line in contents.lines() {
        if line.is_empty() {
            continue;
        }

        if line.starts_with("Tile") {
            tile_num = line
                .split_whitespace()
                .nth(1)
                .unwrap()
                .trim_end_matches(':')
                .parse::<i64>()
                .unwrap();

            continue;
        }

        if !tiles.contains_key(&tile_num) {
            tiles.insert(tile_num, Vec::new());
        }

        tiles.get_mut(&tile_num).unwrap().push(line.to_string());
    }


    let mut edge_occurrances: HashMap<String, Vec<i64>> = HashMap::new();
    for (tile_num, tile) in tiles.iter() {
        let top = tile[0].to_string();
        let bottom = tile[tile.len() - 1].to_string();
        let left = tile
            .iter()
            .map(|row| row.chars().next().unwrap())
            .collect::<String>();
        let right = tile
            .iter()
            .map(|row| row.chars().last().unwrap())
            .collect::<String>();
        let top_reversed = top.chars().rev().collect::<String>();
        let bottom_reversed = bottom.chars().rev().collect::<String>();
        let left_reversed = left.chars().rev().collect::<String>();
        let right_reversed = right.chars().rev().collect::<String>();

        for edge in vec![
            top,
            bottom,
            left,
            right,
            top_reversed,
            bottom_reversed,
            left_reversed,
            right_reversed,
        ] {
            if !edge_occurrances.contains_key(&edge) {
                edge_occurrances.insert(edge.clone(), Vec::new());
            }

            edge_occurrances.get_mut(&edge).unwrap().push(*tile_num);
        }
    }

    return tiles
        .iter()
        .filter(|(_, tile)| {
            let top = tile[0].to_string();
            let bottom = tile[tile.len() - 1].to_string();
            let left = tile
                .iter()
                .map(|row| row.chars().next().unwrap())
                .collect::<String>();
            let right = tile
                .iter()
                .map(|row| row.chars().last().unwrap())
                .collect::<String>();
            let top_reversed = top.chars().rev().collect::<String>();
            let bottom_reversed = bottom.chars().rev().collect::<String>();
            let left_reversed = left.chars().rev().collect::<String>();
            let right_reversed = right.chars().rev().collect::<String>();

            let top_count = edge_occurrances.get(&top).unwrap();
            let bottom_count = edge_occurrances.get(&bottom).unwrap();
            let left_count = edge_occurrances.get(&left).unwrap();
            let right_count = edge_occurrances.get(&right).unwrap();
            let top_reversed_count = edge_occurrances.get(&top_reversed).unwrap();
            let bottom_reversed_count = edge_occurrances.get(&bottom_reversed).unwrap();
            let left_reversed_count = edge_occurrances.get(&left_reversed).unwrap();
            let right_reversed_count = edge_occurrances.get(&right_reversed).unwrap();

            return vec![
                top_count.len(),
                bottom_count.len(),
                left_count.len(),
                right_count.len(),
                top_reversed_count.len(),
                bottom_reversed_count.len(),
                left_reversed_count.len(),
                right_reversed_count.len(),
            ]
            .iter()
            .filter(|count| **count == 1)
            .count()
                == 4;
        })
        .map(|(tile_num, _)| *tile_num)
        .product::<i64>();
}

fn part2(contents: String) -> i64 {
    let mut tile_num = 0;
    let mut tiles: HashMap<i64, Vec<String>> = HashMap::new();

    for line in contents.lines() {
        if line.is_empty() {
            continue;
        }

        if line.starts_with("Tile") {
            tile_num = line
                .split_whitespace()
                .nth(1)
                .unwrap()
                .trim_end_matches(':')
                .parse::<i64>()
                .unwrap();

            continue;
        }

        if !tiles.contains_key(&tile_num) {
            tiles.insert(tile_num, Vec::new());
        }

        tiles.get_mut(&tile_num).unwrap().push(line.to_string());
    }

    let mut edge_occurrances: HashMap<String, Vec<i64>> = HashMap::new();
    for (tile_num, tile) in tiles.iter() {
        let top = tile[0].to_string();
        let bottom = tile[tile.len() - 1].to_string();
        let left = tile
            .iter()
            .map(|row| row.chars().next().unwrap())
            .collect::<String>();
        let right = tile
            .iter()
            .map(|row| row.chars().last().unwrap())
            .collect::<String>();
        let top_reversed = top.chars().rev().collect::<String>();
        let bottom_reversed = bottom.chars().rev().collect::<String>();
        let left_reversed = left.chars().rev().collect::<String>();
        let right_reversed = right.chars().rev().collect::<String>();

        for edge in vec![
            top,
            bottom,
            left,
            right,
            top_reversed,
            bottom_reversed,
            left_reversed,
            right_reversed,
        ] {
            if !edge_occurrances.contains_key(&edge) {
                edge_occurrances.insert(edge.clone(), Vec::new());
            }

            edge_occurrances.get_mut(&edge).unwrap().push(*tile_num);
        }
    }

    let mut tile_arr: Vec<Vec<(i64, i64)>> = Vec::new();
    let mut p_tile = tiles
        .iter()
        .filter(|(_, tile)| {
            let top = tile[0].to_string();
            let bottom = tile[tile.len() - 1].to_string();
            let left = tile
                .iter()
                .map(|row| row.chars().next().unwrap())
                .collect::<String>();
            let right = tile
                .iter()
                .map(|row| row.chars().last().unwrap())
                .collect::<String>();
            let top_reversed = top.chars().rev().collect::<String>();
            let bottom_reversed = bottom.chars().rev().collect::<String>();
            let left_reversed = left.chars().rev().collect::<String>();
            let right_reversed = right.chars().rev().collect::<String>();

            let top_count = edge_occurrances.get(&top).unwrap();
            let bottom_count = edge_occurrances.get(&bottom).unwrap();
            let left_count = edge_occurrances.get(&left).unwrap();
            let right_count = edge_occurrances.get(&right).unwrap();
            let top_reversed_count = edge_occurrances.get(&top_reversed).unwrap();
            let bottom_reversed_count = edge_occurrances.get(&bottom_reversed).unwrap();
            let left_reversed_count = edge_occurrances.get(&left_reversed).unwrap();
            let right_reversed_count = edge_occurrances.get(&right_reversed).unwrap();

            vec![
                top_count.len(),
                bottom_count.len(),
                left_count.len(),
                right_count.len(),
                top_reversed_count.len(),
                bottom_reversed_count.len(),
                left_reversed_count.len(),
                right_reversed_count.len(),
            ]
            .iter()
            .filter(|count| **count == 1)
            .count()
                == 4
        })
        .map(|(tile_num, _)| *tile_num)
        .next()
        .unwrap();

    let mut p_or: i64 = 0;
    let mut oriented_tile = tiles.get(&p_tile).unwrap().clone();
    for i in 0..8 {
        let top = oriented_tile[0].to_string();
        let left = oriented_tile
            .iter()
            .map(|row| row.chars().next().unwrap())
            .collect::<String>();
        let right = oriented_tile
            .iter()
            .map(|row| row.chars().last().unwrap())
            .collect::<String>();
        let bottom = oriented_tile[oriented_tile.len() - 1].to_string();
        if edge_occurrances.get(&top).unwrap().len() == 1
            && edge_occurrances.get(&left).unwrap().len() == 1
            && edge_occurrances.get(&right).unwrap().len() == 2
            && edge_occurrances.get(&bottom).unwrap().len() == 2
        {
            p_or = i;
            break;
        }

        if i % 2 == 0 {
            // Rotate 90 degrees clockwise
            oriented_tile = (0..oriented_tile.len())
                .map(|i| {
                    oriented_tile
                        .iter()
                        .map(|row| row.chars().nth(i).unwrap())
                        .collect::<String>()
                })
                .collect();
        } else {
            // Flip vertically
            oriented_tile = oriented_tile.iter().rev().map(|row| row.clone()).collect();
        }
    }

    loop {
        let mut curr_row: Vec<(i64, i64)> = Vec::new();
        curr_row.push((p_tile, p_or));
        let first_tile = p_tile;
        let bottom = oriented_tile[oriented_tile.len() - 1].to_string();

        loop {
            let right = oriented_tile
                .iter()
                .map(|row| row.chars().last().unwrap())
                .collect::<String>();
            let n_tile = edge_occurrances
                .get(&right)
                .unwrap()
                .iter()
                .filter(|tile| **tile != p_tile)
                .next();
            if n_tile.is_none() {
                break;
            }

            p_tile = *n_tile.unwrap();
            oriented_tile = tiles.get(&p_tile).unwrap().clone();
            for i in 0..8 {
                let left = oriented_tile
                    .iter()
                    .map(|row| row.chars().next().unwrap())
                    .collect::<String>();
                if left == right {
                    p_or = i;
                    break;
                }

                if i % 2 == 0 {
                    // Rotate 90 degrees clockwise
                    oriented_tile = (0..oriented_tile.len())
                        .map(|i| {
                            oriented_tile
                                .iter()
                                .map(|row| row.chars().nth(i).unwrap())
                                .collect::<String>()
                        })
                        .collect();
                } else {
                    // Flip vertically
                    oriented_tile = oriented_tile.iter().rev().map(|row| row.clone()).collect();
                }
            }

            curr_row.push((p_tile, p_or));
        }

        tile_arr.push(curr_row.clone());
        let n_tile = edge_occurrances
            .get(&bottom)
            .unwrap()
            .iter()
            .filter(|tile| **tile != first_tile)
            .next();
        if n_tile.is_none() {
            break;
        }

        p_tile = *n_tile.unwrap();
        oriented_tile = tiles.get(&p_tile).unwrap().clone();
        for i in 0..8 {
            let top = oriented_tile[0].to_string();
            if top == bottom {
                p_or = i;
                break;
            }

            if i % 2 == 0 {
                // Rotate 90 degrees clockwise
                oriented_tile = (0..oriented_tile.len())
                    .map(|i| {
                        oriented_tile
                            .iter()
                            .map(|row| row.chars().nth(i).unwrap())
                            .collect::<String>()
                    })
                    .collect();
            } else {
                // Flip vertically
                oriented_tile = oriented_tile.iter().rev().map(|row| row.clone()).collect();
            }
        }
    }

    let mut image: Vec<String> = Vec::new();
    let lines_per_row = tiles.get(&tile_arr[0][0].0).unwrap().len() - 2;
    for (i, row) in tile_arr.iter().enumerate() {
        image.extend(vec!["".to_string(); lines_per_row]);
        for (t_num, or) in row.iter() {
            let mut oriented_tile = apply_orientation(tiles.get(t_num).unwrap().clone(), *or);

            oriented_tile = oriented_tile
                .iter()
                .skip(1)
                .take(oriented_tile.len() - 2)
                .map(|row| row.chars().skip(1).take(row.len() - 2).collect())
                .collect();
            for (j, line) in oriented_tile.iter().enumerate() {
                image[i * lines_per_row + j] += line;
            }
        }
    }

    let top_regex = Regex::new(r"(?=(.{18}#.{1}))").unwrap();
    let middle_regex = Regex::new(r"(?=(#.{4}##.{4}##.{4}###))").unwrap();
    let bottom_regex = Regex::new(r"(?=(.{1}#.{2}#.{2}#.{2}#.{2}#.{2}#))").unwrap();

    let mut monster_start_indices: Vec<(usize, usize)> = Vec::new();
    for i in 0..8 {
        for j in 0..image.len() - 2 {
            let pos_top: HashSet<usize> = top_regex
                .find_iter(&image[j])
                .map(|m| m.unwrap().start())
                .collect();
            let pos_middle: HashSet<usize> = middle_regex
                .find_iter(&image[j + 1])
                .map(|m| m.unwrap().start())
                .collect();
            let pos_bottom: HashSet<usize> = bottom_regex
                .find_iter(&image[j + 2])
                .map(|m| m.unwrap().start())
                .collect();

            let mut possible_monsters: HashSet<usize> =
                pos_top.intersection(&pos_middle).map(|&x| x).collect();
            possible_monsters = possible_monsters
                .intersection(&pos_bottom)
                .map(|&x| x)
                .collect();

            for start in possible_monsters {
                monster_start_indices.push((j, start));
            }
        }

        if monster_start_indices.len() > 0 {
            break;
        }

        if i % 2 == 0 {
            // Rotate 90 degrees clockwise
            image = (0..image.len())
                .map(|i| {
                    image
                        .iter()
                        .map(|row| row.chars().nth(i).unwrap())
                        .collect::<String>()
                })
                .collect();
        } else {
            // Flip vertically
            image = image.iter().rev().map(|row| row.clone()).collect();
        }
    }

    return (image
        .iter()
        .map(|line| line.chars().filter(|c| *c == '#').count())
        .sum::<usize>()
        - monster_start_indices.len() * 15) as i64;
}

fn apply_orientation(tile: Vec<String>, orientation: i64) -> Vec<String> {
    let mut oriented_tile = tile.clone();
    for i in 0..orientation {
        if i % 2 == 0 {
            // Rotate 90 degrees clockwise
            oriented_tile = (0..oriented_tile.len())
                .map(|i| {
                    oriented_tile
                        .iter()
                        .map(|row| row.chars().nth(i).unwrap())
                        .collect::<String>()
                })
                .collect();
        } else {
            // Flip vertically
            oriented_tile = oriented_tile.iter().rev().map(|row| row.clone()).collect();
        }
    }

    return oriented_tile;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 20899048083289);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 273);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "20".to_string();

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
        "\nPart 1:\nProduct of corner tile ID's: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nNumber of # not part of a monster: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}