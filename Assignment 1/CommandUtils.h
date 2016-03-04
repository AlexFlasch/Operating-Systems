#ifndef CommandUtils
#define CommandUtils

char* getCommand(char *input);

char** getArgs(char *input);

int isFileCommand(char **args);

int isBackgroundCommand(char **args);

char* getFilename(char **args);

char** stripExtraneousArguments(char **args);

#endif
