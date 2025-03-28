#include <stdio.h>
#include <unistd.h>
#include "debug.h"

/**
 * @brief Function do view buffer contents as text
 */
void print_line(char* line){
    printf("Buffer internals: \n");
    int i = 0;
    while(line[i] != '\0'){
        printf("%c", line[i]);
        i++;
    }
    printf("\n");
}

void dump_pipe(int pipefd){
    // Dump pipefd
    char c;
        while(read(pipefd, &c, 1)){
        printf("%c", c);
    }
}