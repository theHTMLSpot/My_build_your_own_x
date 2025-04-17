#ifndef SHELL_H  
#define SHELL_H

// Function declarations
void lsh_loop(void);               // Function to run the main loop
char *lsh_read_line(void);         // Function to read a line of input
char **lsh_split_line(char *line); // Function to split the line into arguments
int lsh_execute(char **args);      // Function to execute commands
char *lsh_get_cwd(void);



// Enum for exit codes (avoid reusing names like EXIT_SUCCESS — conflict with stdlib.h)
enum LshStatus {
    LSH_EXIT_SUCCESS = 0,
    LSH_EXIT_ERROR = 1
};

#endif // SHELL_H
