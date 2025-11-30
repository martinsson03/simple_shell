#include <unistd.h> 
#include "bultin_functions.h"
#include <stdio.h>

char* bultin_commands_str[] = {"cd", "exit"};
int (*bultin_commands_functions[]) (char**) = {&bultin_cd, &bultin_exit};

/**
 * @brief Builtin change directory method
 */
int bultin_cd(char** args){
    if(args[1] != NULL){
        chdir(args[1]);
    }
    return 1;
}

int bultin_exit(char** args){
    // Return 0 for exit
    printf("Exiting...\n");
    return 0;
}
