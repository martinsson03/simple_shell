#include "bultin_functions.h"

char* bultin_commands_str[] = {"cd", "exit"};
int (*bultin_commands_functions[]) (char**) = {&bultin_cd, &bultin_exit};

int bultin_cd(char** args){
    return 1;
}

int bultin_exit(char** args){
    return 1;
}