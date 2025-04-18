#include <stdlib.h>
#include <stdio.h>
#include "shell.h"

void lsh_loop(void)
{

  char *line;
  char **args;
  int status;

  do {
    printf("%s %% ", lsh_get_cwd());
    //read input
    line = lsh_read_line();
    //split into tokens
    args = lsh_split_line(line);
    //execute program
    status = lsh_execute(args);
  

    free(line);
    free(args);
  } while (status == LSH_EXIT_SUCCESS || status == LSH_EXIT_ERROR);
}
