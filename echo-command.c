#include "echo-command.h"
void echo(int i,char** commandTokens,char* command, char typeBitString[]){
	bool redirect = false, done = false, op=false, inp=false;
	int fd0, childPid= -1;
	char* buffer = (char*) malloc(sizeof(char)*10000);
	if(strcmp(typeBitString,"0100")==0||strcmp(typeBitString,"0110")==0){
		done = true;
		char* file = (char*)malloc(sizeof(char)*1000);
		file = "none\0";
		int cnt=0,fd0;
		for(;cnt<(i-1);cnt++){
			if(strstr(commandTokens[cnt],">")||strstr(commandTokens[cnt],">>")){
				if(cnt==(i-2)&&((strcmp(commandTokens[cnt],">")==0)||(strcmp(commandTokens[cnt],">>")==0)))
					printf("Output file not specified\n");
				else{
					if(strstr(commandTokens[cnt],">>")){
						if(strlen(commandTokens[cnt])>2){
							strtok(commandTokens[cnt],">>");
							file = strtok(NULL,">>");
						}
						else
							file=commandTokens[cnt+1];
					}
					else if(strstr(commandTokens[cnt],">")){
						if(strlen(commandTokens[cnt])>1){
							strtok(commandTokens[cnt],">");
							file = strtok(NULL,">");
						}
						else
							file=commandTokens[cnt+1];
					}
					i = cnt+1;
					break;
				}
			}
		}
		if(strcmp(file,"none")!=0){
			op = true;
			if(strcmp(typeBitString,"0100")==0)
				fd0=open(file, O_CREAT|O_WRONLY|O_TRUNC, 0777 );
			else
				fd0=open(file, O_APPEND|O_WRONLY|O_CREAT, 0777 );
			redirect = true;
			childPid = fork();
			if (childPid==0)
				dup2(fd0,1);
		}
	}
	else if(strcmp(typeBitString,"1100")==0||strcmp(typeBitString,"1110")==0){
		done = true;
		char* inputFile = (char*)malloc(sizeof(char)*1000);
		char* outputFile = (char*)malloc(sizeof(char)*1000);
		outputFile = "none\0";
		inputFile = "none\0";
		int cnt=0,fd0,fd1;
		for(cnt=0;cnt<(i-1);cnt++){
			if(strstr(commandTokens[cnt],">")||strstr(commandTokens[cnt],">>")){
				if(cnt==(i-2)&&((strcmp(commandTokens[cnt],">")==0)||(strcmp(commandTokens[cnt],">>")==0)))
					printf("Output file not specified\n");
				else{
					if(strstr(commandTokens[cnt],">>")){
						if(strlen(commandTokens[cnt])>2){
							strtok(commandTokens[cnt],">>");
							outputFile = strtok(NULL, ">>");
						}
						else
							outputFile=commandTokens[cnt+1];
					}
					else if(strstr(commandTokens[cnt],">")){
						if(strlen(commandTokens[cnt])>1){
							strtok(commandTokens[cnt],">");
							outputFile = strtok(NULL, ">");
						}
						else
							outputFile=commandTokens[cnt+1];
					}
					i = cnt+1;
					break;
				}
			}
		}
		for(cnt=0;cnt<(i-1);cnt++){
			if(strstr(commandTokens[cnt],"<")){
				if(cnt==(i-2)&&(strcmp(commandTokens[cnt],"<")==0))
					printf("Input file not specified\n");
				else{
					if(strlen(commandTokens[cnt])>1){
						inp = true;
						strtok(commandTokens[cnt],"<");
						inputFile = strtok(NULL,"<");
					}
					else
						inputFile=commandTokens[cnt+1];
					i = cnt+1;
					break;
				}
			}
		}
		if((strcmp(outputFile,"none")!=0)||(strcmp(inputFile,"none")!=0)){
			if(strcmp(outputFile,"none")==0){
				printf("Invalid command. Exiting...\n");
				return;
			}
			op = true;
			if(strcmp(typeBitString,"1100")==0)
				fd0=open(outputFile, O_CREAT|O_WRONLY|O_TRUNC, 0777 );
			else
				fd0=open(outputFile, O_APPEND|O_WRONLY|O_CREAT, 0777 );
			if(strcmp(inputFile,"none")!=0){
				redirect = true;
				fd1=open(inputFile, O_RDONLY);
				if(read(fd1, buffer, 1024)!=-1){
					inp = true;
					close(fd1);
					strcpy(commandTokens[i-1],buffer);
					i++;
				}
				else{
					redirect = false;
					printf("ERROR: Input file does not exist.\n");
					close(fd0);
					return;
				}
			}
			childPid = fork();
			if (childPid==0){
				dup2(fd0,1);
				if(inp){
					printf("%s", buffer);
					close(fd0);
					fclose(stdout);
					exit(1);
				}
				else{
					wait(NULL);
					return;
				}
			}
		}
	}
	else if(strcmp(typeBitString,"1000")==0){
		done = true;
		char* file = (char*)malloc(sizeof(char)*1000);
		file = "none\0";
		int cnt=0,fd1;
		for(;cnt<(i-1);cnt++){
			if(strstr(commandTokens[cnt],"<")){
				if(cnt==(i-2)&&(strcmp(commandTokens[cnt],"<")==0))
					printf("Input file not specified\n");
				else{
					if(strlen(commandTokens[cnt])>1){
						strtok(commandTokens[cnt],"<");
						file = strtok(NULL, "<");
					}
					else
						file=commandTokens[cnt+1];
					i = cnt+1;
					break;
				}
			}
		}
		if(strcmp(file,"none")!=0){
			redirect = true;
			fd1=open(file, O_RDONLY);
			if(read(fd1, buffer, 10000)!=-1){
				inp = true;
				close(fd1);
				printf("%s", buffer);
				return;
				strcpy(commandTokens[i-1],buffer);
				i++;
			}
			else{
				redirect = false;
				printf("ERROR: Input file does not exist.\n");
				return;
			}
		}
	}
	if(childPid==-1 || childPid==0){
		if(i==1){
			printf("\n");
		}
		else{
			if(commandTokens[1][0]=='$'){
				int length = strlen(commandTokens[1]),k=0;
				char varName[length];
				for(j=1; j<length; j++){
					varName[j-1] = commandTokens[1][j];
				}
				varName[j-1] = '\0';
				bool isDefined=false;
				for (k=0; k<globalVariablesIndex; k++){
					if(strcmp(globalVariables[k].variableName,varName)==0){
						isDefined=true;
						if(globalVariables[k].isString)
							printf("%s\n", globalVariables[k].StringValue);
						else
							printf("%d\n", globalVariables[k].IntegerValue);
						break;
					}
				}
				if(!isDefined){
					if(strlen(commandTokens[1])==1){
						printf("$\n");
					}
					else{
						printf("\n");
					}
				}
			}
			else{
				// printf("yesiuyfias\n");
				int cnt = 5;
				char *pos2;
				if ((pos2=strchr(command, '\n')) != NULL) {
					*pos2 = '\0';
				}
				char newCommand[1000];
				int cmdIndex, newCmdIndex;
				for (cmdIndex=0; cmdIndex<strlen(command) && isspace(command[cmdIndex]); cmdIndex++){
				}		
				int startingPos = cmdIndex;
				for (cmdIndex=(strlen(command)-1); cmdIndex>=startingPos && isspace(command[cmdIndex]); cmdIndex--){
				}
				int endingPos = cmdIndex;
				for(newCmdIndex=0,cmdIndex=startingPos; cmdIndex<=endingPos; cmdIndex++){
					newCommand[newCmdIndex++] = command[cmdIndex];
				}
				newCommand[newCmdIndex] = "\0";
				int counter = 0;
				int newCommandLength = strlen(newCommand);
				newCmdIndex = newCommandLength;
				for(counter=0; counter<newCmdIndex; counter++){
					if((newCommand[counter]=='>')||(newCommand[counter]=='<')){
						if(done)
							newCommandLength = counter;
						else{
							printf("-bash: syntax error near unexpected token `newline'");
							return;
						}
						break;
					}
				}
				if(newCommand[5]==39){
					cnt = 6;
					for(; cnt<newCommandLength && newCommand[cnt]!=39; cnt++)
						printf("%c", newCommand[cnt]);
					printf("\n");
				}
				else if(newCommand[5]==34){
					cnt = 6;
					for(; cnt<newCommandLength && newCommand[cnt]!=34; cnt++)
						printf("%c", newCommand[cnt]);
					printf("\n");
					// printf("okkk\n");
				}
				else{
					cnt = 5;
					for(; cnt<newCommandLength; cnt++)
						printf("%c", newCommand[cnt]);
					printf("\n");	
				}
			}
		}
	}
	if(op){
		if(childPid==0){
			close(fd0);
			fclose(stdout);
			exit(1);
		}
		else{
			wait(NULL);
		}
	}
	return;
}
