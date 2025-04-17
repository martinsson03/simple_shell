    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/wait.h>
    #include <unistd.h>
    #include <stdbool.h>
    #include <dirent.h>

    #include "main.h"
    #include "bultin_functions.h"
    #include "debug.h"



    #define BUFFER_BLOCK_SIZE 64
    #define BUFFER_ARG_BLOCK_SIZE 8
    #define TOKEN_DELIM " \t\r\n\a"

    #define MAX_PATH_LENGTH 256

    int main(int argc, char** argv){
        shell_loop();
    }

    void shell_loop(void){
        char* line;
        char **args;
        int status;

        do {
            // Print the path to CWD
            print_prompt();
            
            line = read_line();
            args = split_line(line);
            status = excecute(args);

            // Free line allocated in read_line
            free(line);
            // Free args allocated in split_line
            free(args);
        } while (status);
    }

    /**
     * @brief Reads a line from stdin using getchar()
    */
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

            // If tab character is detected
            if(current_char == '\t'){
                DIR *d;
                struct dirent *dir;
                // Open the current directory
                d = opendir(".");


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
        return buffer;
    }

    /**
     * @brief Function which choose what commands to be sent to launch and
     * what commands are bultin
     */
    int excecute(char** args){
        char** args_i = args;
        int pipe_index = -1;

        while (args_i != NULL && *args_i != NULL) {
            // If pipe is detected
            if (strcmp(*args_i, "|") == 0) {
                pipe_index = args_i - args;
                //printf("Pipe index = %i\n", pipe_index);
                // Inserts null
                *args_i = NULL;
                // Make the pointer point to the first argument to the second process
                args_i++;
                // Print the first argument after print
                //printf("First argument after pipe = %s\n", args_i[0]);
                break;
            }
            args_i++;
        }

        // If no pipe
        if (pipe_index == -1){
            //printf("No pipe index detected\n");
            // Checking for all builtin functions
            for (int i = 0; i < BULTIN_FUNCTION_COUNT; i++){
                // If we match with a bultin function
                if(strcmp(bultin_commands_str[i], args[0]) == 0){
                    return bultin_commands_functions[i](args);
                }
            }
            return launch(args, false, 0, 0);
        }
        // If pipe
        else{
            // Create fds for pipe
            int pipefd[2];
            if (pipe(pipefd) == -1){
                perror("[execute]:  pipe failed\n");
                exit(EXIT_FAILURE);
            }
            
            launch(args, true, pipefd, 1);
            close(pipefd[1]);
            int status = launch(args_i, true, pipefd, 2);
            close(pipefd[0]);
            
            return status;
        }
    }


    /**
     * @brief Function to tokanize input
     */
    char** split_line(char* line){
        int buffer_size = BUFFER_ARG_BLOCK_SIZE;
        char** tokens = (char**) malloc(sizeof(char*) * BUFFER_ARG_BLOCK_SIZE);
        char* token;
        int current_position = 0;

        // Check for allocation error
        if (!tokens){
            perror("[Split Line]: Memory allocation failure\n");
            exit(EXIT_FAILURE);
        }

        token = strtok(line, TOKEN_DELIM);
        while(token != NULL){
            tokens[current_position] = token;
            current_position++;

            // If we need to resize
            if(current_position >= buffer_size){
                buffer_size += BUFFER_ARG_BLOCK_SIZE;
                tokens = realloc(tokens, sizeof(char*) * buffer_size);
                if (!tokens){
                    perror("[Split Line]: Memory allocation failure\n");
                    exit(EXIT_FAILURE);
                }
            }
            token = strtok(NULL, TOKEN_DELIM);
        }
        tokens[current_position] = NULL;
        return tokens;
    
    }


    int launch(char** args, bool usePipe, int pipefd[], int processNumber){
        pid_t pid;
        int status;

        // Fork this process
        pid = fork();
        // If 0, returned to the newly created child process
        if (pid < 0){
            perror("[Launch]:   Error when forking\n");
            exit(EXIT_FAILURE);
        }
        // If "we" are the newly created child
        else if(pid == 0){
            // If we should redirect stdin or stdout
            if(usePipe){
                // If we are the "first" process in pipe
                if(processNumber == 1){
                    printf("First process stdout changed to %i\n", pipefd[1]);
                    // Redirect stdout to the in port of the pipe
                    dup2(pipefd[1], 1);
                    close(pipefd[0]);
                    close(pipefd[1]);
                    printf("THIS SHOULD NOT BE PRINTED!!!!\n");
                }
                // If we are the second process
                else if(processNumber == 2){
                    printf("Second process stdin changed to %i\n", pipefd[0]);
                    dup2(pipefd[0], 0);
                    close(pipefd[0]);
                    close(pipefd[1]);
                }
            }
            // Become the new process
            // If error
            if (execvp(args[0], args) == -1){
                perror("[Launch (child)]:   Error when launching\n");
            }
            exit(EXIT_FAILURE);
        }
        // If we are the original process
        else{
            do{
                waitpid(pid, &status, WUNTRACED);
            }
            while(!(WIFEXITED(status)) && !WIFSIGNALED(status)); 
        }
        return 1;
    }


    /**
     * @brief prints the path to cwd to stdout
     */
    void print_prompt(void){
        char cwd[MAX_PATH_LENGTH];

        if(getcwd(cwd, sizeof(cwd)) == NULL){
            perror("[Print Prompt]: Error when getting the path to cwd\n");
            exit(EXIT_FAILURE);
        }

        printf("%s>", cwd);
    }
