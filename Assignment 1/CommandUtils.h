#ifndef CommandUtils
#define CommandUtils

char* getCommand(char *input);

char** getArgs(char *input);

void runCommand(char *command, char **args);

#endif
