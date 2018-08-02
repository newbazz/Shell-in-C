#include "global.h"
#include "kjobs.h"
#include "setenv.h"
#include "unsetenv.h"
#include "cd-command.h"
#include "pwd-command.h"
#include "echo-command.h"
#include "ls-command.h"
#include "pinfo-command.h"
#include "dirty-command.h"
#include "interrupts-command.h"
#include "pipfunction.h"
#include "overkill.h"
#include "jobs.h"
int check(int id){
	char dir[100000]={0};
	dir[0]='/',dir[1]='p',dir[2]='r',dir[3]='o',dir[4]='c',dir[5]='/';
	char p[10000]={0};
	sprintf(p,"%d",id);
	strcat(dir,p);
	DIR* direct = opendir(dir);
	if(dir)return 1;
	return 0;
}
void sigintHandler(int sig_num)
{
    /* Reset handler to catch SIGINT next time.
       Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGINT, sigintHandler);
    printf("\n Cannot be terminated using Ctrl+C \n");
    fflush(stdout);
}
int main(){
	IsPipe=false;
	procCount=0;
	globalVariablesIndex = 0;
	count=0;
	cdCommand = "cd";
	pwdCommand = "pwd";
	echoCommand = "echo";
	lsCommand = "ls";
	dirtyCommand = "dirty";
	interruptsCommand = "interrupts";
	pinfoCommand = "pinfo";
	int countbp=1;
	previousDirSet = false;
	getcwd(homeDir, 200);
	int homeDirLength = strlen(homeDir);
	while(1){
		signal(SIGINT, sigintHandler);
		int hh=0;
			for(hh=0;hh<procCount;hh++){
				printf("hello\n");
				int yyy=check(bprocess[hh][0]);
				if(!yyy)bprocess[hh][1]=0;
			}
		countbp = 1;
		char systemName[40];
		char* userName = getlogin();
		gethostname(systemName, 40);

		char currentDir[200];
		getcwd(currentDir, 200);      
		int currentDirLength = strlen(currentDir);

		char homeCurrentDir[200];
		if(homeDirLength<=currentDirLength){
			homeCurrentDir[0] = '~';
			for(i=homeDirLength,j=1;i<currentDirLength;i++,j++){
				homeCurrentDir[j]=currentDir[i];
			}
			homeCurrentDir[j]='\0'; 
		}
		else strcpy(homeCurrentDir, currentDir);
		char* command = (char*)malloc(10000);
		char* commandee = (char*)malloc(10000);
		printf("<%s@%s:%s> ", userName,systemName,homeCurrentDir);
		fgets (command, 10000, stdin);
		strcpy(commandee,command);
		// printf("%s\n",commandee);
		char* allcommands[10000];
		int numberOfCommands=0,allcommandsIndex=0;
		char *token, *pos;
		if ((pos=strchr(command, '\n')) != NULL) {
					*pos = '\0';
		}
		token=strtok(command,";");
		while(token!=NULL){
			allcommands[allcommandsIndex++]=token;
			token=strtok(NULL, ";");
		}
		numberOfCommands = allcommandsIndex;
		if(!IsPipe){
			int commandIndex = 0, length, start, end, k;
			bool lastOneBg,isBackgroundProcess;
			char* cmdProcesses[100];

			while(commandIndex<numberOfCommands)
			{	
				pipehelper(allcommands[commandIndex]);
				if(!IsPipe){
					countbp = 1;
					length = strlen(allcommands[commandIndex]);
					k = 0;
					for (k = (length-1); k>=0 && isspace(allcommands[commandIndex][k]); k--)
					{
					}
					end = k;
					for (k = 0; k<length && isspace(allcommands[commandIndex][k]); k++)
					{
					}
					start = k;
					
					if (commandIndex==(numberOfCommands-1)){
						lastOneBg = false;
						if(allcommands[commandIndex][end]=='&'){
							lastOneBg = true;
						}
					}
					
					isBackgroundProcess = false;
					for (k=start; k<=end; k++){
						if(allcommands[commandIndex][k]=='&'){
							isBackgroundProcess = true;
							break;
						}
					}
					char typeOfCmd[17] = "";
					strcat(typeOfCmd, strstr(allcommands[commandIndex],"<")?"1":"0");
					strcat(typeOfCmd, strstr(allcommands[commandIndex],">")?"1":"0");
					strcat(typeOfCmd, strstr(allcommands[commandIndex],">>")?"1":"0");
					strcat(typeOfCmd, strstr(allcommands[commandIndex],"|")?"1":"0");
					strcat(typeOfCmd, "\0");
					char batchString[10000];
					int batchStringIndex = 0;
					for(k=start; k<=end; k++)
						batchString[batchStringIndex++] = allcommands[commandIndex][k];
					batchString[batchStringIndex] = '\0';

					strcpy(command,batchString);
					char commandCopy[10000];
					strcpy(commandCopy, command);
					char *pos3;
					if ((pos3=strchr(commandCopy, '\n')) != NULL) {
						*pos3 = '\0';
					}
					int justCd = strcmp(cdCommand, commandCopy);
					char* token = strtok(commandCopy, " ");
					char* commandTokens[10];
					commandTokens[0] = token;
					int commandTokensCounter=1;
					if(justCd!=0){
						while( token != NULL ) {
							token = strtok(NULL, " ");
							commandTokens[commandTokensCounter++] = token;
						}
					}
					bool setVariableFlag = false;
					for (j=0; j<strlen(command); j++){
						if((command[j]=='=') && (j!=0) && j!=(strlen(command)-1)){
							if((isalnum(command[j-1])||command[j-1]=='_') && (!isspace(command[j+1]))){
								if(j==1 && command[j-1]=='_'){}
								else
								{
									setVariableFlag = true;
									break;								
								}
							}
						}
					}
					int cmdProcessNo=1;
					if(isBackgroundProcess){
						token=strtok(batchString,"&");
						cmdProcesses[0]=token;
						cmdProcessNo = 1;
						token=strtok(NULL, "&");
						while(token!=NULL){
							cmdProcesses[cmdProcessNo++]=token;
							token=strtok(NULL, "&");
						}
					}
					int j=0, count = 0, args=0;
					for(j=0; j<cmdProcessNo; j++){
						int ret = -1;
						char* spaceSepCommands[100];
						char wholeCommand[1000];
						bool executed=false;
						if((j==(cmdProcessNo-1) && commandIndex!=(numberOfCommands-1))||(!isBackgroundProcess)||(commandIndex==(numberOfCommands-1) && !lastOneBg)){
						}
						else{
							strcpy(wholeCommand,cmdProcesses[j]);
							token=strtok(cmdProcesses[j]," ");
							args=0;
							while(token){
								spaceSepCommands[args]=token;
								args++;
								token=strtok(NULL, " ");
						 	}
							ret=fork();
							if(ret){
								bprocess[procCount][0]=(int)ret;
								bprocess[procCount++][1]=1;
							}
						}
						if(strcmp(commandTokens[0], "quit")==0)return 0;
						if(strcmp(commandTokens[0], cdCommand)==0){
							if(ret<0){
								CD(commandTokensCounter,commandTokens,typeOfCmd);
							}
							else{
								if(ret==0){
									CD(args,spaceSepCommands,typeOfCmd);
									exit(0);
								}
								else{
									wait(NULL);
									kill(ret, SIGTERM);
									printf("[%d] %d\n", countbp, ret);
									countbp++;
								}
							}
							executed=true;
						}
						else if(strcmp(commandTokens[0],pwdCommand)==0){
							if(ret<0)
								pwd(commandTokensCounter, commandTokens, typeOfCmd);
							else{
								if(ret==0){
									pwd(args,spaceSepCommands,typeOfCmd);
									exit(0);
								}
								else{
									wait(NULL);
									kill(ret, SIGTERM);
									printf("[%d] %d\n", countbp, ret);
									countbp++;
								}
							}
							executed=true;
						}
						else if(strcmp(commandTokens[0],pinfoCommand)==0){
							if(ret<0)
								pinfo(commandTokensCounter, commandTokens, typeOfCmd);
							else{
								if(ret==0){
									pinfo(args, spaceSepCommands, typeOfCmd);
									exit(0);
								}
								else{
									wait(NULL);
									kill(ret, SIGTERM);
									printf("[%d] %d\n", countbp, ret);
									countbp++;
								}
							}
							executed=true;
						}
						else if(strcmp(commandTokens[0],"kjobs")==0){
							kjob(commandTokens);
							executed=true;
						}
						else if(strcmp(commandTokens[0],"fg")==0){
							// kjob(commandTokens);
							printf("in fg\n");
							// 	
							executed=true;
						}
						else if(strcmp(commandTokens[0],"jobs")==0){
							printf("in job\n");
							job(commandTokens);
							executed=true;

						}
						else if(strcmp(commandTokens[0],"overkill")==0){
							ok(commandTokens);
							executed=true;
						}	
						else if(strcmp(commandTokens[0],"setenv")==0){
							set_env(commandTokens);
							executed=true;
						}
						else if(strcmp(commandTokens[0],"unsetenv")==0){
							unset_env(commandTokens);
							executed=true;
						}
						else if(strcmp(commandTokens[0],"bg")==0){
							printf("in bg\n");
							executed=true;
						}
						else if(strcmp(commandTokens[0],lsCommand)==0){
							if(ret<0)
								lsCmd(commandTokens,commandTokensCounter,typeOfCmd);
							else{
								if(ret==0){
									lsCmd(spaceSepCommands,args,typeOfCmd);
									exit(0);
								}
								else{
									wait(NULL);
									kill(ret, SIGTERM);
									printf("[%d] %d\n", countbp, ret);
									countbp++;
								}
							}
							executed=true;
						}
						else if(strcmp(commandTokens[0],echoCommand)==0){
							if(ret<0)
								echo(commandTokensCounter,commandTokens,command,typeOfCmd);
							else{
								if(ret==0){
									echo(args,spaceSepCommands,wholeCommand,typeOfCmd);
									exit(0);
								}
								else{
									wait(NULL);
									kill(ret, SIGTERM);
									printf("[%d] %d\n", countbp, ret);
									countbp++;
								}
							}
							executed=true;
						}
						else if(setVariableFlag){
							if(ret<=0){
								char *pos;
								if ((pos=strchr(command, '\n')) != NULL){
									*pos = '\0';
								}
								Var newVariable;
								newVariable.isString = false;
								newVariable.isInt = false;
								char variableNm[50];
								for (j=0; j<strlen(command); j++){
									variableNm[j] = command[j];
									if(command[j]=='='){
										break;
									}
								}
								variableNm[j] = '\0';
								j++;
								if((command[j] >= '0') && (command[j] <= '9')){
									newVariable.isInt = true;
									char intValue[20];
									int k=0;
									for (k=0; j<strlen(command); j++,k++){
										intValue[k] = command[j];
									}
									intValue[k] = '\0';
									int globalVariablesCounter = 0;
									bool alreadyDefined = false;
									for (globalVariablesCounter=0; globalVariablesCounter<globalVariablesIndex;globalVariablesCounter++){
										if(strcmp(globalVariables[globalVariablesCounter].variableName,variableNm)==0){
											alreadyDefined = true;
											globalVariables[globalVariablesCounter].IntegerValue = atoi(intValue);
											break;
										}
									}
									if(!alreadyDefined){
										strcpy(newVariable.variableName, variableNm);
										newVariable.IntegerValue = atoi(intValue);
										globalVariables[globalVariablesIndex++] = newVariable;
									}
								}
								else{
									int endpoint;
									if((command[j] == 34) || (command[j] == 39)){
										endpoint = strlen(command)-1;
										j++;
									}
									else
										endpoint = strlen(command);
									newVariable.isString = true;
									char stringValue[50];
									int k=0;
									for (k=0; j<endpoint; j++,k++){
										stringValue[k] = command[j];
									}
									stringValue[k] = '\0';
									int globalVariablesCounter = 0;
									bool alreadyDefined = false;
									for (globalVariablesCounter=0; globalVariablesCounter<globalVariablesIndex;globalVariablesCounter++){
										if(strcmp(globalVariables[globalVariablesCounter].variableName,variableNm)==0){
											alreadyDefined = true;
											strcpy(globalVariables[globalVariablesCounter].StringValue,stringValue);
											break;
										}
									}
									if(!alreadyDefined){
										strcpy(newVariable.variableName, variableNm);
										strcpy(newVariable.StringValue, stringValue);
										globalVariables[globalVariablesIndex++] = newVariable;						
									}
								}
								if (ret==0)
									exit(0);
							}
							else{
								wait(NULL);
								kill(ret,SIGTERM);
								printf("[%d] %d\n", countbp, ret);
								countbp++;
							}
							executed=true;
						}
						else if((commandTokensCounter>=4)||(args>=4)){
							if((strcmp(commandTokens[0],"nightswatch")|strcmp(commandTokens[1],"-n")|strcmp(commandTokens[3],"dirty"))==0){
								if(ret<0){
									dirty(commandTokens[2]);
								}
								else{
									if(ret==0){
										dirty(spaceSepCommands[2]);
										exit(0);
									}
									else{
										wait(NULL);
										kill(ret, SIGTERM);
										printf("[%d] %d\n", countbp, ret);
										countbp++;
									}
								}
								executed=true;
							}
							else if((strcmp(commandTokens[0],"nightswatch")|strcmp(commandTokens[1],"-n")|strcmp(commandTokens[3],"interrupts"))==0){
								if(ret<0){
									watch(commandTokens[2]);
								}
								else{
									if(ret==0){
										watch(spaceSepCommands[2]);
										exit(0);
									}
									else{
										wait(NULL);
										kill(ret, SIGTERM);
										printf("[%d] %d\n", countbp, ret);
										countbp++;
									}
								}
							}
							executed=true;
						}
						if(!executed){
							if(ret<=0){
								char* commandArgs[100];
								int commandArgsIndex = 0;
								for (commandArgsIndex = 0; commandArgsIndex < args; ++commandArgsIndex){
									commandArgs[commandArgsIndex] = spaceSepCommands[commandArgsIndex];
								}
								commandArgs[commandArgsIndex] = NULL;
								if(execvp(commandArgs[0], commandArgs)==-1){
									printf("%s: command not found\n",commandArgs[0]);
									break;
								}	
								if (ret==0)
									exit(0);
							}
							else{
								wait(NULL);
								kill(ret,SIGTERM);
								printf("[%d] %d\n", countbp, ret);
								int hh=0;
								for(hh=0;hh<procCount;hh++){
									if(bprocess[hh][0]==ret){
									int yyy=check(ret);
									if(!yyy)bprocess[hh][1]=0;
										break;
									}
								}
								countbp++;
							}
						}
					}
				}
				else
					IsPipe=false;
				commandIndex++;
			}
		}		
		else
			IsPipe=false;
	}
	return 0;
}

