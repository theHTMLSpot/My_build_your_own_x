use std::io::{self, Write};
use crate::execute::execute;
use crate::utils;
use crate::execute;



pub fn rssh_loop() {
    loop {
        
        let cwd = utils::get_cw(); // Ensure utils module has the get_cw function
        // Print the prompt
        print!("{} % ", cwd);
        io::stdout().flush().unwrap();  // Ensure the prompt is displayed immediately

        // Read user input
        let mut input = String::new();
        io::stdin().read_line(&mut input).expect("Failed to read line");

        let input = input.trim();  // Remove any leading/trailing whitespace

        if input == "exit" {
            println!("Exiting shell...");
            break;  // Exit the loop when user types "exit"
        }

        let _args: Vec<&str> = input.split(" ").collect();
        let status: i8 = execute(_args);


    }
}
