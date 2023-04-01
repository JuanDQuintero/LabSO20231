#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "wish_utils.h"

#define MAX_SIZE 100

char error_message[28] = "An error has occurred\n";
char cwd[1024];


void execute_exit(int value){

	exit(0);
}

void execute_cd(char *newpath){
	char *path = strtok_r(newpath, " ", &newpath);
	
	if(path == NULL){
		write(STDERR_FILENO, error_message, strlen(error_message)); 
	}else{
		char *hola = getcwd(NULL, 0);
		strcat(hola, "/");
		strcat(hola, path);
		chdir(hola);
		printf("%s\n", hola);
	}

}

char* concatenate_path(char* original){
	char *relative = "./";
	char *thepath = strdup(original);
	char *lp = "/";
	char *fp = malloc(sizeof(char*) * MAX_SIZE);
	strcpy(fp, relative);
	strncat(fp, thepath, strlen(thepath));
	strcat(fp, lp);
	free(thepath);
	return fp;
}

void execute_path(char** path, char *newpaths){
	
	memset(path,0,sizeof(path));
	char* newpath;
	int index = 0;
	while((newpath = strtok_r(newpaths, " ", &newpaths))){
		if(strncmp(newpath, ".", 1) != 0 && strncmp(newpath, "/", 1) != 0){
			char *fp = concatenate_path(newpath);
			path[index] = malloc(strlen(fp) + 1);
			strcpy(path[index], fp);
			free(fp);
		}
		else{
			char *actualpath = strdup(newpath);
			strcat(actualpath, "/");
			path[index] = actualpath;
		}
		index++;
	} 
	path[index] = "";
	
	
}


	
	/*

	char *newpath = strtok_r(newpaths, " ", &newpaths);
	char *concat;
	sprintf(concat, "/%s", newpath);
	printf("%s\n", concat);
	strncat(str,concat,strlen(str));
	chdir(str);
	free(concat);
	printf("path executed: %s\n",str);*/
	




