#include <stdio.h>
#include <stdlib.h>

#include "main.h"

#define BUFFER_BLOCK_SIZE 1024
#define ARG_LENGTH 64
#define EXIT_ERROR 1

int main(int argc, char** argv){
    shell_loop();
}

void shell_loop(void){
    char* line;
    char **args;
    //int status;

    do {
        printf("> ");
        line = read_line();
        args = split_line(line);
        //status = shell_excecute();

        // Free args allocated in spli_line
        free(line);
        // Free args allocated in split_line
        free(args);
    } while (1);
}

char* read_line(void){
    // Tmp variables
    int buffer_size = BUFFER_BLOCK_SIZE;
    char* buffer = (char*) malloc(sizeof(char) * BUFFER_BLOCK_SIZE);
    int current_char;

    int current_position = 0;

    // Check for allocation error
    if (!buffer){
        perror("[Read Line]: Memory allocation failure\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        current_char = getchar();
        // If we should return
        if (current_char == EOF || current_char == '\n'){
            // Add null char
            buffer[current_position] = '\0';
            return buffer;
        }

        buffer[current_position] = current_char;
        current_position++;

        // If we should resize
        if (current_position >= buffer_size){
            buffer_size += BUFFER_BLOCK_SIZE;
            buffer = (char*) realloc(buffer, sizeof(char) * buffer_size);
            
            if (!buffer){
                perror("[Read Line]: Memory allocation failure\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    return (char*) 1;
}

char** split_line(char* line){
    return (char**) 1;

}