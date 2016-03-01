#include <stdio.h>
#include <stdlib.h>

#include "CommandUtils.h"

int main(int argc, char *argv[]) {
    // main input loop
    for(;;) {
        char *input = malloc(100);

        printf("\n>> ");
        input = fgets(input, 100, stdin);

        char *command = getCommand(input);
        char **args = getArgs(input);

        runCommand(command, args);
    }
}
