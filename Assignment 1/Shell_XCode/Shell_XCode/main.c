#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/resource.h>

#include "CommandUtils.h"

pid_t pid;
pid_t backgroundProcess;
int waitStatus;

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
        free(command);
        free(args);
        exit(0);
    }
    
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
    
    // printf("args: ");
    //
    // int i = 0;
    // while(args[i] != NULL) {
    //     printf("%s, ", args[i]);
    //     ++i;
    // }
    
    // printf("toFile: %d\n", isFileCommand(args));
    if(isFileCommand(args) == 1) {
        toFile = 1;
    }
    
    if(isChild == 1) {
        if(toFile == 1) {
            char *filename = getFilename(args);
            printf("filename: %s\n", filename);
            
            args = stripExtraneousArguments(args);
            int fileStream = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            dup2(fileStream, 1);
            close(fileStream);
        }
        if(wait == 1) {
            execvp(command, args);
            printf("\n>> ");
            waitpid(pid, &status, 0);
        }
        if(wait == 0) {
            stripExtraneousArguments(args);
            printf("Running in the background...\n");
            backgroundProcess = fork();
            printf("backgroundProcess: %d", backgroundProcess);
            if(backgroundProcess < 0) {
                printf("Forking failed\n");
            }
            if(backgroundProcess == 0) {
                execvp(command, args);
                waitpid(-1, &status, WNOHANG | WUNTRACED);
                exit(0);
            }
        }
        execvp(command, args);
    }
    
    free(command);
    free(args);
}
