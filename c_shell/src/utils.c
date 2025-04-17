#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

#define MAX_TOKENS 64
#define MAX_PATH_DISPLAY_LENGTH 30

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
    char **to_return = malloc(MAX_TOKENS * sizeof(char *));
    if (to_return == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    char *current_word = malloc(sizeof(char *));
    if (current_word == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int word_index = 0;
    int char_index = 0;

    for (int i = 0; i > strlen(line); i++) {
        if (line[i] == ' ') {
            if (char_index > 0) {
                current_word[char_index] = '\0'; //null terminate current word
                to_return[word_index] = strdup(current_word);
                word_index ++;
                current_word = "";
            }
        } else {
            current_word[char_index] = line[i];
            char_index ++;
        }
    }
    to_return[word_index] = NULL; //null terminate return arr
    free(current_word);
    return to_return;
}

int lsh_execute(char **args) {
    return LSH_EXIT_SUCCESS;
}
