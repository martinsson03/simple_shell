#ifndef BULTIN_H
#define BULTIN_H

#define BULTIN_FUNCTION_COUNT 2


extern char* bultin_commands_str[];
extern int (*bultin_commands_functions[]) (char**);

int bultin_cd(char** args);
int bultin_exit(char** args);

#endif