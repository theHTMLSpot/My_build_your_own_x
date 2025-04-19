use std::env;
use std::path::PathBuf;

pub fn get_cw() -> String {
    const MAX_SEGMENTS: usize = 5;

    // Step 1: Get current directory
    let cwd = match env::current_dir() {
        Ok(path) => path,
        Err(_) => return String::from("unknown"),
    };

    // Step 2: Get user home directory
    let home_dir = dirs::home_dir(); // uses the `dirs` crate

    let mut display_path = cwd.clone();
    if let Some(home) = home_dir {
        if cwd.starts_with(&home) {
            // Replace the home directory prefix with ~
            let components = cwd.strip_prefix(&home).unwrap_or(&cwd).components();
            let mut new_path = PathBuf::from("~");
            new_path.extend(components);
            display_path = new_path;
        }
    }

    // Step 3: Split into segments
    let path_str = display_path.display().to_string();
    let segments: Vec<&str> = path_str.split('/').collect();

    // Step 4: Truncate if too long
    if segments.len() > MAX_SEGMENTS {
        let start = &segments[..MAX_SEGMENTS / 2];
        let end = &segments[segments.len() - (MAX_SEGMENTS / 2)..];
        let mut shortened = start.to_vec();
        shortened.push("...");
        shortened.extend_from_slice(end);
        return shortened.join("/");
    }

    path_str
}
