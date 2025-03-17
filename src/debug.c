#include <stdio.h>
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