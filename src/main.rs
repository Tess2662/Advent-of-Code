use std::detect::__is_feature_detected::xsave;
use std::fs::File;
use std::io::{BufRead, BufReader};
fn main() {
    // defines 2D dynamic array
    let mut grid: Vec<Vec<char>> = Vec::new();
    // let mut grid: [[char; 150]; 200] = [[' '; 150]; 200];
    let mut moves: String = String::new();
    // Open the file in read-only mode
    let file =
        File::open("/home/tereza/CLionProjects/aoc2022/input.txt").expect("Failed to open file");

    // Create a BufReader from the file
    let mut reader = BufReader::new(file);

    let mut column: i32 = 0;
    let mut row: i32 = 0;
    let mut grid_loaded = false;
    // Read the file line by line
    for (size, line) in reader.lines().enumerate() {
        let line = line.expect("Failed to read line");
        if size == 0 {
            column = line.find(".").expect("Failed to find .") as i32;
        }

        if (grid_loaded) {
            moves.extend(line.chars());
        } else {
            if line.is_empty() {
                grid_loaded = true;
                continue;
            }
            grid.push(line.chars().collect());
        }
    }
    let mut moves_index = 0;
    let mut step_size = 0;
    // right down left up
    let mut facing: i32 = 0;
    while moves_index < moves.len() {
        let n_slice = moves[moves_index..]
            .chars()
            .take_while(|ch| ch.is_numeric())
            .collect::<String>();
        moves_index += n_slice.len();
        step_size = n_slice.parse().expect("Failed to parse integer");

        while step_size > 0 {
            let sx = (1 - facing) % 2;
            let sy = (2 - facing) % 2;
            let mut y = ((grid.len() as i32) + row + sy) % (grid.len() as i32);
            let mut x =
                ((grid[y as usize].len() as i32) + column + sx) % (grid[y as usize].len() as i32);

            while grid[y as usize][x as usize] == ' ' {
                y = ((grid.len() as i32) + y + sy) % (grid.len() as i32);
                x = ((grid[y as usize].len() as i32) + x + sx) % (grid[y as usize].len() as i32);
            }

            if grid[y as usize][x as usize] == '#' {
                println!("breaking");
                break;
            }
            row = y;
            column = x;

            grid[y as usize][x as usize] = match facing {
                0 => '>',
                1 => 'v',
                2 => '<',
                3 => '^',
                _ => panic!("Unexpected facing {}", facing),
            };
            step_size -= 1;
        }

        if moves_index < moves.len() {
            let ch = moves[moves_index..=moves_index].chars().next();
            match moves[moves_index..=moves_index].chars().next() {
                Some('R') => facing = (facing + 1) % 4,
                Some('L') => facing = (4 + facing - 1) % 4,
                _ => panic!("Invalid facing"),
            }
        }
        moves_index += 1;
    }
    for x in grid {
        println!("{:?}", String::from_iter(x));
    }
    println!("{} {} {}", facing, row, column);
    println!("help {}", 1000 * (row + 1) + 4 * (column + 1) + facing);
}
