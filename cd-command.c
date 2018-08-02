#include "cd-command.h"
#include "trim.h"

void CD(int i,char** commandTokens,char typeBitString[]){
	if(strcmp(typeBitString,"0100")==0||strcmp(typeBitString,"0010")==0){
		char* file = NULL;
		int cnt=0,fd;
		for(;cnt<(i-1);cnt++){
			if(strcmp(commandTokens[cnt],">")==0||strcmp(commandTokens[cnt],">>")==0){
				if(cnt==(i-2)){
					printf("Output file not specified\n");
				}
				else{
					file = commandTokens[cnt+1];
					break;
				}
			}
		}
		if(file){
			int pid = fork();
			char* argv[] = {"touch", file, NULL};
			if(pid==0)
				execvp(argv[0],argv);
		}
	}
	if(i==1){
		getcwd(currentDir, 200);
		chdir(homeDir);
		// return;
	}
	else if(strcmp(commandTokens[1],"~")==0){
		getcwd(currentDir, 200);
		chdir(homeDir);
	}
	else if(strcmp(commandTokens[1],"-")==0){
		if(!previousDirSet){
			printf("OLDPWD not set\n");		
		}
		else{
			printf("%s\n", previousDir);
			getcwd(currentDir, 200);
			chdir(previousDir);
			strcpy(previousDir, currentDir);
		}
	}

	else{
		getcwd(currentDir, 200);
		int returnValue = chdir(commandTokens[1]);
		if(returnValue<0){
			printf("cd: %s: No such file or directory\n", commandTokens[1]);
			return;
		}
	}
	strcpy(previousDir, currentDir);

	previousDirSet = true;
	return;
}
