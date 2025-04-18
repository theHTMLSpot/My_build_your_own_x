#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> // Required for DIR and directory operations
#include <stdbool.h> // Required for bool type
#include <sys/types.h>
#include <sys/stat.h>
#include "shell.h"

int lsh_launch(char **args)
{
  printf("running"); 
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return EXIT_FAILURE;
}

char *builtin_str[] = {
    "cd",
    "help",
    "ls",
    "mkdir",
    "rm",
    "touch",
    "pwd",
    "exit"
  };
  
int (*builtin_func[]) (char **) = {
    &lsh_cd,
    &lsh_help,
    &lsh_ls,
    &lsh_mkdir,
    &lsh_rm,
    &lsh_touch,
    &lsh_pwd,
    &lsh_exit
};

int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
  }

int lsh_cd(char **args)
{
   
    if (args[1] == NULL) {
      fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else {
      if (chdir(args[1]) != 0) {
        perror("lsh");
      }
    }
    return EXIT_FAILURE;
}



int lsh_help(char **args)
{
 
  int i;
  printf("Ethan Lagdens LSH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return EXIT_FAILURE;
}

int lsh_exit(char **args)
{
  return 501;
}
int lsh_ls(char **args){
    struct dirent *entry;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return EXIT_FAILURE;
    }
    const char *path = (args[1] != NULL) ? args[1] : cwd;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        return EXIT_SUCCESS;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
    return EXIT_SUCCESS;
}
int lsh_rm(char **args){
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: rm: missing operand\n");
        return EXIT_FAILURE;
    }

    bool recursive = false;
    char *target = NULL;

    for (int i = 1; args[i] != NULL; i++) {
        if (strcmp(args[i], "-r") == 0 || strcmp(args[i], "-rf") == 0)
            recursive = true;
        else
            target = args[i];
    }

    if (!target) {
        fprintf(stderr, "lsh: rm: missing target path\n");
        return EXIT_FAILURE;
    }

    if (recursive) {
        printf("Recursively deleting: %s\n", target);
        Node *root = insert(NULL, strdup(target));
        build_tree_delete_files(root, target);
        postorder_delete(root);
    } else {
        if (remove(target) == 0) {
            printf("Deleted file: %s\n", target);
        } else {
            perror(target);
        }
    }

    return EXIT_SUCCESS;
}

int lsh_mkdir(char **args){
    if (mkdir(args[1], 0755) == 0) {
        printf("Directory created successfully.\n");
    } else {
        perror("Error creating directory");
        return 1;  // Return an error code if directory creation fails
    }

    return 0;

}
int lsh_touch(char **args){
    FILE *file = fopen(args[1], "w");
    return EXIT_SUCCESS;
}
int lsh_pwd(char **args) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return EXIT_FAILURE;
    }
    printf("%s\n", cwd);
    return EXIT_SUCCESS;
}



int lsh_execute(char **args)
{

  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return LSH_EXIT_ERROR;
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return lsh_launch(args);
}