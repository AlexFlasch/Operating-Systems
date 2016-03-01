#include <stdio.h>

#include "CommandUtils.h"

int main(int argc, char *argv[]) {
    int quit = 0;

    // main input loop
    while(!quit) {
        char *input;

        printf(">> ");
        input = gets(input);

        char *command = getCommand(input);
        char **args = getArgs(input);
    }
}
