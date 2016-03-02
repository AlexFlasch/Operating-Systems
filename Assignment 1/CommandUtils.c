#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

////////// PRIVATE //////////

char* getFilename(char **args) {
    char *filename;

    int i = 0;
    while(args[i] != NULL) {
        if(strcmp(args[i], ">")) {
            filename = strdup(args[++i]);
            break;
        }
        ++i;
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
 * Returns 1 if the command has a & argument
 * Returns 0 if the shell should wait for completion
 */
int isBackgroundCommand(char **args) {
    int isBackground = 0;

    int i = 0;
    while(args[i] != NULL) {
        if(strcmp(args[i], "&") == 0) {
            isBackground = 1;
            break;
        }
        ++i;
    }

    return isBackground;
}

/*
 * Returns 1 if the command's output should be piped to a file
 * Returns 0 if the command's output should be printed to the shell
 */
int isFileCommand(char **args) {
    int isFile = 0;

    int i = 0;
    while(args[i] != NULL) {
        if(strcmp(args[i], ">") == 0) {
            isFile = 1;
            break;
        }
        ++i;
    }

    return isFile;
}

/*
 * Returns the first part of the user's input.
 * Generally the name of the command (ls, clear, cp, rm, etc)
 */
char* getCommand(char *input) {
    char *command = splitString(input)[0];
    command = strtok(command, "\n");

    return command;
}

/*
 * Returns an array of strings which are all of the arguments
 * of the command string
 */
char** getArgs(char *input) {
    char **args = splitString(input);

    int i = 0;
    while(args[i] != NULL) {
        args[i] = strtok(args[i], "\n");
        ++i;
    }

    return args;
}
