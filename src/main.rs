use std::collections::HashMap;
use std::detect::__is_feature_detected::xsave;
use std::fs::File;
use std::io::{self, BufRead};

// coordinates around point x,y ( N,NW, NE, S, SE, SW, W, NW, SW, E, NE, SE)
const ELF_SURROUNDINGS: [[[i32; 2]; 3]; 4] = [
    [[0, -1], [-1, -1], [1, -1]],
    [[0, 1], [-1, 1], [1, 1]],
    [[-1, 0], [-1, -1], [-1, 1]],
    [[1, 0], [1, -1], [1, 1]],
];

#[derive(Copy, Clone, PartialEq)]
enum Dir {
    N = 0,
    S,
    W,
    E,
}


impl Dir {
    fn next_dir(&self) -> Dir {
        match self {
            Dir::N => Dir::S,
            Dir::E => Dir::N,
            Dir::S => Dir::W,
            Dir::W => Dir::E,
        }
    }
}

struct Elf {
    pos: (i32, i32),
    next_pos: (i32, i32),
    next_dir: Dir,
    go_next: bool,
}

impl Elf {
    fn next_pos(&self, elves: &Vec<Elf>) -> Option<((i32, i32), Dir)> {
        if self.are_surroundings_empty(&elves) {
            return None;
        }
        let mut next_dir = self.next_dir;
        loop {
            if self.is_surrounding_empty(&elves, ELF_SURROUNDINGS[next_dir as usize]) {
                return Some(match next_dir {
                    Dir::N => ((self.pos.0, self.pos.1 - 1), next_dir),
                    Dir::S => ((self.pos.0, self.pos.1 + 1), next_dir),
                    Dir::W => ((self.pos.0 - 1, self.pos.1), next_dir),
                    Dir::E => ((self.pos.0 + 1, self.pos.1), next_dir),
                });
            }
            next_dir = next_dir.next_dir();
            if (next_dir == self.next_dir) {
                return None;
            }
        }
    }

    fn is_surrounding_empty(&self, elves: &Vec<Elf>, arr: [[i32; 2]; 3]) -> bool {
        for sur_pos in arr {
            let sur_pos = (self.pos.0 + sur_pos[0], self.pos.1 + sur_pos[1]);
            for elf in elves {
                if elf.pos == sur_pos {
                    return false;
                }
            }
        }
        return true;
    }

    fn are_surroundings_empty(&self, elves: &Vec<Elf>) -> bool {
        for sur in ELF_SURROUNDINGS {
            if !self.is_surrounding_empty(&elves, sur) {
                return false;
            }
        }
        return true;
    }
}

fn main() {
    let mut elves: Vec<Elf> = Vec::new();
    let mut elves_ht: HashMap<(i32, i32), bool> = HashMap::new();
    let f = File::open("/home/tereza/CLionProjects/aoc2022/input.txt").expect("Failed to open file input.txt");
    for (l, line) in io::BufReader::new(f).lines().enumerate() {
        for (i, c) in line.expect("Failed to read line").chars().enumerate() {
            if c == '#' {
                elves.push(Elf { pos: (i as i32, l as i32), next_pos: (0, 0), next_dir: Dir::N, go_next: false });
            }
        }
    }
       for i in -5..15 {
            for j in -5..15 {
                let m = match elves.iter().find(|e| e.pos == (j, i)) {
                    Some(_) => "#",
                    None => ".",
                };
                print!("{}", m);
            }
            println!();
        }
        println!();
    for c in 0..10 {
        for i in 0..elves.len() {
            match elves[i].next_pos(&elves) {
                Some(pos) => {
                    elves[i].next_pos = pos.0;
                    elves[i].go_next = true;
                }
                None => elves[i].go_next = false,
            }
            elves[i].next_dir = elves[i].next_dir.next_dir();
        }
        'next_elf: for i in 0..elves.len() {
            if !elves[i].go_next {
                continue;
            }
            for j in i + 1..elves.len() {
                if elves[i].next_pos == elves[j].next_pos {
                    elves[i].go_next = false;
                    elves[j].go_next = false;
                    continue 'next_elf;
                }
            }
            elves[i].pos = elves[i].next_pos;
        }

        for i in -5..15 {
            for j in -5..15 {
                let m = match elves.iter().find(|e| e.pos == (j, i)) {
                    Some(_) => "#",
                    None => ".",
                };
                print!("{}", m);
            }
            println!();
        }
        println!();
    }

    let mut x_min = i32::MAX;
    let mut y_min = i32::MAX;
    let mut x_max = 0;
    let mut y_max = 0;
    for elf in elves.iter() {
        if elf.pos.0 < x_min {
            x_min = elf.pos.0;
        }
        if elf.pos.0 > x_max {
            x_max = elf.pos.0;
        }
        if elf.pos.1 < y_min {
            y_min = elf.pos.1;
        }
        if elf.pos.1 > y_max {
            y_max = elf.pos.1;
        }
    }
     for i in y_min..=y_max {
            for j in x_min..=x_max {
                let m = match elves.iter().find(|e| e.pos == (j, i)) {
                    Some(_) => "#",
                    None => ".",
                };
                print!("{}", m);
            }
            println!();
        }
        println!();

    let area = (x_max - x_min + 1) * (y_max - y_min + 1);
    println!("Area: {} {} {} {} {} {} {}", area, elves.len(), area-elves.len() as i32, x_min, x_max, y_min, y_max);
}