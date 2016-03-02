#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "CommandUtils.h"

void runCommand(char *command, char **args);

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

/*
 * Runs the command, this will:
 *      * fork a new process
 *      * execute the command
 *      * waits for the command to finish
 */
void runCommand(char *command, char **args) {
    if(strcmp(command, "quit") == 0) {
        exit(0);
    }

    pid_t pid = fork();
    int status;

    if(pid < 0) {
        printf("Forking the child process has failed");
        exit(1);
    }

    int isChild = 0;
    if(pid == 0) {
        isChild = 1;
    }

    int wait = 1;
    int toFile = 0;

    // if(isBackgroundCommand(args) == 1) {
    //     wait = 0;
    // }
    printf("toFile: %d", isFileCommand(args));
    if(isFileCommand(args) == 1) {
        toFile = 1;
    }

    printf("args: ");

    int i = 0;
    while(args[i] != NULL) {
        printf("%s, ", args[i]);
        ++i;
    }

    if(isChild == 1) {
        if(toFile == 1) {
            printf("Redirecting to file...");
        }

        execvp(command, args);
    }
    else {
        waitpid(pid, &status, 0);
    }

    // free(command);
    // free(args);
}
