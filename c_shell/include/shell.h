#ifndef SHELL_H  
#define SHELL_H


typedef struct Node {
    char *value;
    struct Node *left;
    struct Node *right;
} Node;




// Function declarations
void lsh_loop(void);               // Function to run the main loop
char *lsh_read_line(void);         // Function to read a line of input
char **lsh_split_line(char *line); // Function to split the line into arguments
int lsh_execute(char **args);      // Function to execute commands
char *lsh_get_cwd(void);
int lsh_launch(char **args);
Node* insert(Node* root, const char *value);
Node* build_tree_delete_files(Node *root, const char *path);
void postorder_delete(Node *node);
//terminal commands
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_ls(char **args);
int lsh_rm(char **args);
int lsh_mkdir(char **args);
int lsh_touch(char **args);
int lsh_pwd(char **args);

// Enum for exit codes (avoid reusing names like EXIT_SUCCESS — conflict with stdlib.h)
enum LshStatus {
    LSH_EXIT_SUCCESS = 0,
    LSH_EXIT_ERROR = 1
};



#endif // SHELL_H

