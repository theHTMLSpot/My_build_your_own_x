#include <stdlib.h>
#include <stdio.h>
#include "shell.h"

int main(int argc, char **argv) {
    // loop
    lsh_loop();

    // return success
    return LSH_EXIT_SUCCESS;
}
