#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "wish_utils.h"
#include "wish_utils.c"

#define MAX_SIZE 100
#define BUFFER_SIZE 1024

char *mypath[] = { "bli","bla","/bin/", ""};

void error();
int check_arguments(char *arguments);
void lineParse(char line[], char *array[30], int flagsNumber);
char** separate_commands(char *line);
int concurrency = 0;

int main(int argc, char *argv[]){
	
	char str[MAX_SIZE];
	char *command_string;
	char *s;
	int fd;
	if(argc == 1){
		char *command;
		char **commands = malloc(sizeof(char*)* MAX_SIZE);
		do{	
			int count=0;
	     		printf("wish> ");
	     		fgets(str, MAX_SIZE, stdin);
			s = str;
		        while(*s != '\n') {
		        	++s;
		        }
		        *s = '\0';
			s = str;
			/*char *arg;
			int i = 0;
			
			// Crear un arreglo para guardar los argumentos
			char *args[20];
			
			arg = strtok(s, " ");
			while (arg != NULL && i < 20) { //contar los argumentos 

				args[i] = (char *) malloc(strlen(arg) + 1);
				strcpy(args[i], arg);
				arg = strtok(NULL, " ");
				i++;
			}
			
			if (i > 1 && strcmp(args[1], ">") == 0) { //
				if (args[2] == NULL) {
				    error();

				} else if (i > 3 || (i > 2 && args[3] != NULL)) {
				    error();				    
				} 
				continue;
			}*/

		     
                	command_string = strtok_r(s, " ", &s);

			if(strcmp(command_string, "exit") == 0){
					if(strcmp(s, "")==0){
						execute_exit(0);
		  			}else{
						error();
					}
				
			}else if(strcmp(command_string, "cd") == 0){ 
				
				/*if(i>2){
					error();
				}else{*/

				execute_cd(s);
				//}
				
			}else if(strcmp(command_string, "path") == 0){
				execute_path(mypath,s);
			}else if(strcmp(command_string, "&") == 0){   //Test 16
				
			}else{
				
				fd = -1;
				char **mp = mypath;
				char specificpath[MAX_SIZE];
				while( (strcmp(*mp, "") != 0)  && fd != 0){
					
					strcpy(specificpath, *mp++);
					strncat(specificpath, command_string, strlen(command_string));
					fd = access(specificpath, X_OK);

				}
				
				if(fd==0){
					int subprocess = fork();
					if(subprocess < 0){
						printf("Error launching the subprocess");
					}else if(subprocess == 0){
						
						char *myargs[3];
						
		  				myargs[0] = strdup(specificpath);
		  				if(strcmp(s, "")==0){
							myargs[1]=NULL;
		  				}else if(strcmp(s, " > ")==0)
		  				{
		  					myargs[1]=strdup(s);
		  				}

		  				myargs[2] = NULL;
		  				execvp(myargs[0], myargs);
		  				
					}else{
						wait(NULL);
					}
					
				}else{
					error();
				}
			}
	   	}while(1);
	}
	else if(argc == 2){ //Batch

		char line[BUFFER_SIZE];

		    FILE *file = NULL;
		    char *batchFile = strdup(argv[1]);
		    file = fopen(batchFile, "r");

		    if (!file)
		    {
			exit(1);
		    }
		    
		    while(fgets(line, BUFFER_SIZE, file)){
		    	char *cmd[sizeof(line)];
		    	
		    	lineParse(line, cmd, 20);
		    	printf("Comando: %s\n", cmd[0]);
		    	command_string = strtok_r(line, " ", &line);
		    	if(strcmp(command_string, "exit") == 0){
					if(strcmp(s, "")==0){
						execute_exit(0);
		  			}else{
						error();
					}
				
			}else if(strcmp(command_string, "cd") == 0){
				
				execute_cd(s);
				
			}else if(strcmp(command_string, "path") == 0){
				execute_path(mypath,s);
			}else if(strcmp(command_string, "&") == 0){   //Test 16
				
			}else{
				
				fd = -1;
				char **mp = mypath;
				char specificpath[MAX_SIZE];
				while( (strcmp(*mp, "") != 0)  && fd != 0){
					
					strcpy(specificpath, *mp++);
					strncat(specificpath, command_string, strlen(command_string));
					fd = access(specificpath, X_OK);

				}
				
				if(fd==0){
					int subprocess = fork();
					if(subprocess < 0){
						printf("Error launching the subprocess");
					}else if(subprocess == 0){
						
						char *myargs[3];
						
		  				myargs[0] = strdup(specificpath);
		  				if(strcmp(s, "")==0){
							myargs[1]=NULL;
		  				}else if(strcmp(s, " > ")==0)
		  				{
		  					printf("Hola ");
		  				}else
		  				{ 
		  					
		  					myargs[1]=strdup(s);	
		  				}

		  				myargs[2] = NULL;
		  				execvp(myargs[0], myargs);
		  				
					}else{
						wait(NULL);
					}
					
				}else{
					//error();
				}
		    		
		    	}
		    
		   

		}
	}
	
}

void error(){
	char error_message[28] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message)); 
}

void lineParse(char line[], char *array[30], int flagNumber){
	if(line[0] != '\0' && line [0] != '\n'){
		char *command[30];
		command[0] = strtok(line, "\t\n");
		int p = 0;
		while(command[p] != NULL){
			p++;
			command[p] = strtok(NULL, " \n\t");
		}
		command[p + 1] = NULL;
		
		for(int loop = 0; loop < flagNumber; loop++){
			array[loop] = command[loop];
		}
		
	}
}

int check_arguments(char *arguments){
	char *arguments_copy = strdup(arguments);
	int count = 0;
	char *argument;
	while((argument = strtok_r(arguments_copy, " ", &arguments_copy))){
		if(strcmp(argument, ">") == 0){
			break;
			return 1;
		}else {return 0;}
		count++;		
	}
	
}

char** separate_commands(char *line){
	int commands_count = 0;
	char *command;
	char **commands = malloc(sizeof(char*) * MAX_SIZE);
	while((command = strtok_r(line, "&", &line))){
		commands[commands_count] = malloc(strlen(command) + 1);
		strcpy(commands[commands_count], command);
		commands_count++;
	}

	commands[commands_count] = "";
	if(commands_count > 1){
		concurrency = 1;
	}
	else{
		concurrency = 0;
	}
	return commands;
}

