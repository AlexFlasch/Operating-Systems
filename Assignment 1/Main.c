#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "CommandUtils.h"

pid_t pid;
char *command;
char **args;

void runCommand(char *command, char **args);

int main(int argc, char *argv[]) {
    // main input loop
    while(command == NULL || strcmp(command, "quit") != 0) {
        if(command != NULL) {
            free(command);
            free(args);
        }

        char *input = malloc(100);

        printf("\n>> ");
        input = fgets(input, 100, stdin);

        command = getCommand(input);
        args = getArgs(input);

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
    // if(strcmp(command, "quit") == 0) {
    //     printf("here");
    //     exit(0);
    // }

    pid = fork();
    int status;

    if(pid < 0) {
        printf("Forking the child process has failed\n");
        exit(1);
    }

    int isChild = 0;
    if(pid == 0) {
        isChild = 1;
    }

    int wait = 1;
    int toFile = 0;

    if(isBackgroundCommand(args) == 1) {
        wait = 0;
    }

    if(isFileCommand(args) == 1) {
        toFile = 1;
    }

    if(isChild == 1) {
        args = stripExtraneousArguments(args);

        if(toFile == 1) {
            char *filename = getFilename(args);
            printf("filename: %s\n", filename);

            int fileStream = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            dup2(fileStream, 1);
            close(fileStream);
        }

        execvp(command, args);
        waitpid(pid, &status, 0);
    }
    if(isChild != 1 && wait == 1) {
        waitpid(pid, &status, 0);
    }
}
