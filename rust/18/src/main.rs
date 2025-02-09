use relative_path::RelativePath;
use std::env;
use std::fs;
use std::time::Instant;

fn part1(contents: String) -> i64 {
    let mut sum: i64 = 0;
    for line in contents.lines() {
        let expr = Expr::new(line.to_string());
        sum += expr.eval();
    }

    return sum;
}

fn part2(contents: String) -> i64 {
    let mut sum: i64 = 0;
    for line in contents.lines() {
        let mut expr = Expr::new(line.to_string());
        sum += expr.eval_ordered();
    }

    return sum;
}

#[derive(Debug, Clone)]
struct Expr {
    sub_exprs: Vec<Expr>,
    op: Vec<char>,
    val: i64,
}

impl Expr {
    fn new(expr_string: String) -> Expr {
        if expr_string.len() == 1 {

            return Expr {
                sub_exprs: vec![],
                op: vec![],
                val: expr_string.parse::<i64>().unwrap(),
            };
        }

        let mut new_expr = Expr {
            sub_exprs: vec![],
            op: vec![],
            val: 0,
        };

        let mut sub_expr_string: String = String::from("");
        let mut opened_parentheses = 0;
        for c in expr_string.chars() {
            match c {
                ' ' => {
                    if opened_parentheses != 0 {
                        sub_expr_string.push(c);
                    }
                }
                '(' => {
                    if opened_parentheses == 0 {
                        sub_expr_string = "".to_string();
                    } else {
                        sub_expr_string.push(c);
                    }

                    opened_parentheses += 1;
                }
                ')' => {
                    opened_parentheses -= 1;

                    if opened_parentheses == 0 {
                        new_expr.sub_exprs.push(Expr::new(sub_expr_string.clone()));
                    } else {
                        sub_expr_string.push(c);
                    }
                }
                '+' | '*' => {
                    if opened_parentheses == 0 {
                        new_expr.op.push(c);
                    } else {
                        sub_expr_string.push(c);
                    }
                }
                _ => {
                    if opened_parentheses == 0 {
                        new_expr.sub_exprs.push(Expr::new(c.to_string()));
                    } else {
                        sub_expr_string.push(c);
                    }
                }
            }
        }

        if sub_expr_string.len() > 0 {
            new_expr.sub_exprs.push(Expr::new(sub_expr_string.clone()));
        }

        return new_expr;
    }

    fn eval(&self) -> i64 {
        if self.sub_exprs.len() == 0 {
            return self.val;
        }

        let mut res = self.sub_exprs[0].eval();
        for i in 0..self.op.len() {
            match self.op[i] {
                '+' => res += self.sub_exprs[i + 1].eval(),
                '*' => res *= self.sub_exprs[i + 1].eval(),
                _ => panic!("Invalid operator"),
            }
        }

        return res;
    }

    fn eval_ordered(&mut self) -> i64 {
        if self.sub_exprs.len() == 0 {
            return self.val;
        }

        let mut i = 0;
        while i < self.op.len() {
            if self.op[i] == '+' {
                let new_sub_expr = Expr {
                    sub_exprs: vec![],
                    op: vec![],
                    val: self.sub_exprs[i].eval_ordered() + self.sub_exprs[i + 1].eval_ordered(),
                };

                self.sub_exprs.remove(i);
                self.sub_exprs.remove(i);
                self.sub_exprs.insert(i, new_sub_expr);
                self.op.remove(i);
            } else {
                i += 1;
            }
        }

        let mut res = self.sub_exprs[0].eval_ordered();
        for i in 0..self.op.len() {
            match self.op[i] {
                '*' => res *= self.sub_exprs[i + 1].eval_ordered(),
                _ => panic!("Invalid operator"),
            }
        }

        return res;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn p1_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part1(contents), 26386);
    }

    #[test]
    fn p2_test() {
        let contents =
            fs::read_to_string("example.txt").expect("Should have been able to read the file");

        assert_eq!(part2(contents), 693942);
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let year = "2020".to_string();
    let day = "18".to_string();

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
        "\nPart 1:\nSum of lines: {}\nRan in {:.5?}",
        part1(contents.clone()),
        part1_timer.elapsed()
    );

    let part2_timer = Instant::now();
    println!(
        "\nPart 2:\nSum of lines with operator precedence: {}\nRan in {:.5?}",
        part2(contents.clone()),
        part2_timer.elapsed()
    );
}