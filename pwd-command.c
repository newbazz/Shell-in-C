#include "pwd-command.h"

void pwd(int i, char** commandTokens, char typeBitString[]){
	bool redirect = false;
	if(strcmp(typeBitString,"0100")==0||strcmp(typeBitString,"0010")==0){
		char* file = NULL;
		int cnt=0,fd;
		for(;cnt<(i-1);cnt++){
			if(strcmp(commandTokens[cnt],">")==0||strcmp(commandTokens[cnt],">>")==0){
				if(cnt==(i-2))
					printf("Output file not specified\n");
				else{
					file = commandTokens[cnt+1];
					break;
				}
			}
		}
		if(file){
			if(strcmp(typeBitString,"0100")==0)
				fd=open(file, O_CREAT|O_WRONLY|O_TRUNC, 0777 );
			else
				fd=open(file, O_APPEND|O_WRONLY|O_CREAT, 0777 );
			int pid = fork();
			redirect = true;
			if (pid==0){
				dup2(fd,1);
				getcwd(currentDir, 200);
				printf("%s\n", currentDir);
				close(1);
			}

		}
	}
	if(!redirect){
		getcwd(currentDir, 200);
		printf("%s\n", currentDir);	
	}
	return;
}
