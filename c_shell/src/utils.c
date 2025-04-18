#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>   // For DIR, readdir, etc.
#include <sys/types.h>
#include <sys/stat.h> // For struct stat and S_ISDIR
#include "shell.h"

#define LSH_TOK_DELIM " \t\r\n\a"  // Define token delimiters

#define LSH_TOK_BUFSIZE 64  // Define a default buffer size for tokens

#define MAX_TOKENS 64
#define MAX_PATH_DISPLAY_LENGTH 40





Node* create_node(const char *value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->value = strdup(value);
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

Node* insert(Node* root, const char *value) {
    if (root == NULL) return create_node(value);
    if (root->left == NULL)
        root->left = create_node(value);
    else if (root->right == NULL)
        root->right = create_node(value);
    else
        insert(root->left, value); // continue left bias
    return root;
}

void postorder_delete(Node *node) {
    if (node == NULL) return;

    postorder_delete(node->left);
    postorder_delete(node->right);

    struct stat st;
    stat(node->value, &st);

    if (S_ISDIR(st.st_mode)) {
        if (rmdir(node->value) == 0) {
            printf("Deleted directory: %s\n", node->value);
        } else {
            perror(node->value);
        }
    } else {
        if (remove(node->value) == 0) {
            printf("Deleted file: %s\n", node->value);
        } else {
            perror(node->value);
        }
    }

    free(node->value);
    free(node);
}

Node* build_tree_delete_files(Node *root, const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            root = insert(root, strdup(full_path));  // Save full path

            // Recurse
            root = build_tree_delete_files(root, full_path);
        } else if (entry->d_type == DT_REG) {
            if (remove(full_path) == 0) {
                printf("Deleted file: %s\n", full_path);
            } else {
                perror(full_path);
            }
        }
    }

    closedir(dir);
    return root;
}



char *lsh_get_cwd(void) {
    char *buffer = malloc(1024);
    if (buffer == NULL) {
        perror("malloc");
        return NULL;
    }

    if (getcwd(buffer, 1024) != NULL) {
        size_t len = strlen(buffer);

        if (len > MAX_PATH_DISPLAY_LENGTH) {
            // Allocate space for shortened path
            char *shortened = malloc(MAX_PATH_DISPLAY_LENGTH + 4); // extra for "..." and null terminator
            if (shortened == NULL) {
                perror("malloc");
                free(buffer);
                return NULL;
            }

            // Copy start and end of the path, add ellipsis in the middle
            strncpy(shortened, buffer, MAX_PATH_DISPLAY_LENGTH / 2 - 1);
            shortened[MAX_PATH_DISPLAY_LENGTH / 2 - 1] = '\0';
            strcat(shortened, "...");
            strcat(shortened, buffer + len - (MAX_PATH_DISPLAY_LENGTH / 2));

            free(buffer); // free the full path buffer
            return shortened;
        }

        return buffer;  // return full path
    } else {
        perror("getcwd");
        free(buffer);
        return NULL;
    }
}

char *lsh_read_line(void) {
    char *line = malloc(1024 * sizeof(char));  // Allocate memory for the line
    if (line == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);  // Exit if malloc fails
    }

    // Read a line of input
    if (fgets(line, 1024, stdin) == NULL) {
        free(line);  // Free memory if fgets fails
        return NULL;  // Return NULL if no input was read
    }

    // Remove the newline character if it was read
    size_t length = strlen(line);
    if (length > 0 && line[length - 1] == '\n') {
        line[length - 1] = '\0';
    }

    return line;
}

char **lsh_split_line(char *line){
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;
  
    if (!tokens) {
      fprintf(stderr, "lsh: allocation error\n");
      exit(EXIT_FAILURE);
    }
  
    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
      tokens[position] = token;
      position++;
  
      if (position >= bufsize) {
        bufsize += LSH_TOK_BUFSIZE;
        tokens = realloc(tokens, bufsize * sizeof(char*));
        if (!tokens) {
          fprintf(stderr, "lsh: allocation error\n");
          exit(EXIT_FAILURE);
        }
      }
  
      token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}


