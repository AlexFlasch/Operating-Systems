#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

////////// PRIVATE //////////

/*
 * Returns an array of strings that will be
 * [0]: name of command
 * [1 ... n]: args of command
 */
char** splitString(char *string) {
    const int MAX_ARGS_LENGTH = 20;

    char *s = strdup(string);

    char *token = strtok(s, " ");
    char **arr = malloc(sizeof(token) * MAX_ARGS_LENGTH);

    int i = 0;
    while(token != NULL) {
        if(arr == NULL) {
            printf("Memory allocation has failed.\n");
            exit(1);
        }

        char *temp;
        if ((temp = strchr(token, '\n')) != NULL) {
            *temp = '\0';
        }

        arr[i] = token;
        token = strtok(NULL, " ");

        // printf("arr[%d]: %s\n", i, arr[i]);
        ++i;
    }

    // printf("args: ");

    // int j = 0;
    // while(arr[j] != NULL) {
    //     printf("%s, ", arr[j]);
    //     ++j;
    // }
    // printf("\n");

    return arr;
}

////////// PUBLIC //////////

/*
 * Returns the char* that represents the name of the file that
 * execvp's output should be redirected to.
 */
char* getFilename(char **args) {
    char *filename;

    int i = 0;
    while(args[i] != NULL) {
        if(strcmp(args[i], ">") == 0) {
            filename = strdup(args[i + 1]);
            break;
        }
        ++i;
    }

    return filename;
}

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
        // printf("args[%d]: %s\n", i, args[i]);
        // printf("strcmp(args[%d]): %d\n\n", i, strcmp(args[i], ">"));
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
        // printf("args[%d]: %s\n", i, args[i]);
        args[i] = strtok(args[i], "\n");
        ++i;
    }

    return args;
}

/*
 * Strips out arguments that will confuse execvp after they're not needed
 * For example: & to run in background, or > [filename] to redirect to a file.
 */
char** stripExtraneousArguments(char **args) {
    int i = 0;
    while(args[i] != NULL) {
        if(strcmp(args[i], "&") == 0) {
            args[i] = args[i + 1]; // & should be at the end, so args[i] /should/ be null after this.
            return args;
        }
        if(strcmp(args[i], ">") == 0) {
            args[i] = args[i + 2]; // skip past both > and filename
        }

        ++i;
    }

    return args;
}
