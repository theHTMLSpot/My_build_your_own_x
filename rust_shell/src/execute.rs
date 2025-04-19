use std::fs;
use std::env;
use std::io::Write;
use std::path::{Path, PathBuf};



fn cd(_args: Vec<&str>) -> i8 {
    if _args.len() != 2 {
        println!("invalid number of args");
        return 1;
    }
    if let Err(_) = env::set_current_dir(_args[1]) {
        println!("Path not found: {}", _args[1]);
        return 1;
    }
    0
}

fn help(_args: Vec<&str>) -> i8 {
    let builtins: &[&str] = &[
        "cd",
        "help",
        "ls",
        "pwd",
        "touch",
        "mkdir",
        "rm",
    ];
    println!("Ethan Lagdens rust shell");
    println!("I have implemented the following commands: ");
    for i in 0..builtins.len() {
        println!("{}", builtins[i]);
    }
    0
}

fn ls(args: Vec<&str>) -> i8 {
    let dir_path: PathBuf;

    if args.len() == 2 {
        dir_path = PathBuf::from(args[1]);
    } else {
        dir_path = match env::current_dir() {
            Ok(dir) => dir,
            Err(e) => {
                println!("Failed to get current directory: {}", e);
                return 1;
            }
        };
    }

    let entries = match fs::read_dir(&dir_path) {
        Ok(entries) => entries,
        Err(e) => {
            println!("Failed to read directory {}: {}", dir_path.display(), e);
            return 1;
        }
    };

    for entry in entries {
        if let Ok(entry) = entry {
            if let Ok(file_name) = entry.file_name().into_string() {
                print!("{}  ", file_name);
            }
        }
    }

    println!();
    0
}


fn pwd(_args: Vec<&str>) -> i8 {
    match env::current_dir() {
        Ok(dir) => {
            println!("{}", dir.display());
            0
        },
        Err(e) => {
            println!("Failed to get current directory: {}", e);
            1
        }
    }
}


fn touch(_args: Vec<&str>) -> i8 {
    if let Err(_) = fs::File::create(_args[1]) {
        println!("Path not found: {}", _args[1]);
        return 1;
    }
    0
}

fn mkdir(_args: Vec<&str>) -> i8 {
    if let Err(_) = fs::create_dir(_args[1]) {
        println!("Path not found: {}", _args[1]);
        return 1;
    }
    0
}

fn rm(args: Vec<&str>) -> i8 {
    if args.len() < 2 {
        println!("Usage: rm [-r|-rf] <path>");
        return 1;
    }

    if args.len() > 2 && (args[1] == "-r" || args[1] == "-rf") {
        let target = args[2];
        if let Err(_) = fs::remove_dir_all(target) {
            println!("Path not found or failed to delete directory: {}", target);
            return 1;
        }
    } else {
        let target = args[1];
        // Try removing as file or directory
        if fs::remove_file(target).is_err() && fs::remove_dir(target).is_err() {
            println!("Path not found or failed to delete: {}", target);
            return 1;
        }
    }

    0
}

const BUILTINS: &[&str] = &[
    "cd",
    "help",
    "ls",
    "pwd",
    "touch",
    "mkdir",
    "rm",
];

const BUILTINFUNCS: &[fn(Vec<&str>) -> i8] = &[
    cd,
    help,
    ls,
    pwd,
    touch,
    mkdir,
    rm,
];

pub fn execute(args: Vec<&str>) -> i8 {
    if args.is_empty() {
        return 1; // No command entered
    }

    for i in 0..BUILTINS.len() {
        if BUILTINS[i] == args[0] {
            return BUILTINFUNCS[i](args);
        }
    }

    println!("Unknown command: {}", args[0]);
    1
}
