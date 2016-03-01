#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

////////// PRIVATE //////////

/*
 * Returns 1 if the command has a & argument
 * Returns 0 if the shell should wait for completion
 */
int isBackgroundCommand(char **args) {
    int isBackground = 0;

    size_t i;
    for(i = 0; i < sizeof(args); i++) {
        if(strcmp(args[i], "&") == 0) {
            isBackground = 1;
        }
    }

    return isBackground;
}

/*
 * Returns 1 if the command's output should be piped to a file
 * Returns 0 if the command's output should be printed to the shell
 */
int isFileCommand(char **args) {
    int isFile = 0;

    size_t i;
    for(i = 0; i < sizeof(args); i++) {
        if(strcmp(args[i], ">") == 0) {
            isFile = 1;
        }
    }

    return isFile;
}

char* getFilename(char **args) {
    char *filename;

    size_t i;
    for(i = 0; i < sizeof(args); i++) {
        if(strcmp(args[i], ">")) {
            filename = strdup(args[++i]);
        }
    }

    return filename;
}

/*
 * Returns an array of strings that will be
 * [0]: name of command
 * [1 ... n]: args of command
 */
char** splitString(char *string) {
    char *s = strdup(string);

    char *token = strtok(s, " ");
    char **arr = malloc(sizeof(token));

    int i = 0;
    while(token != NULL) {
        arr = realloc(arr, sizeof(token) * i);

        if(arr == NULL) {
            printf("Memory allocation has failed.");
            exit(1);
        }

        arr[i] = token;
        token = strtok(NULL, " ");

        // printf("arr[%d]: %s\n", i, arr[i]);
        ++i;
    }

    // prevent memory leaks
    free(token);

    return arr;
}

////////// PUBLIC //////////

/*
 * Returns the first part of the user's input.
 * Generally the name of the command (ls, clear, cp, rm, etc)
 */
char* getCommand(char *input) {
    return splitString(input)[0];
}

/*
 * Returns an array of strings which are all of the arguments
 * of the command string
 */
char** getArgs(char *input) {
    char **args = &splitString(input)[1];

    // int i = 0;
    // while(args[i] != NULL) {
    //     printf("%s, ", args[i]);
    //     ++i;
    // }

    return args;
}

/*
 * Runs the command, this will:
 *      * fork a new process
 *      * execute the command
 *      * waits for the command to finish
 */
void runCommand(char *command, char **args) {
    printf("here");
    if(strcmp(command, "quit") == 0) {
        exit(0);
    }

    int pid = fork();

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
    // if(isFileCommand(args) == 1) {
    //     toFile = 1;
    // }

    if(isChild == 1) {
        execvp(command, args);
    }
}
