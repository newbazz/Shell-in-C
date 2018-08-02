#include "pinfo-command.h"
void findPinfo(char p[]){
	int UserID=0,i=6,size=0,count=1;
	for(int i=0;i<=strlen(p)-1;++i)UserID=UserID*10+(p[i]-'0');
	char exec_path[2000]={},exec[2000]={},newDiectory[2000]={},procid[20]={};
	 newDiectory[0]='/',	newDiectory[1]='p',newDiectory[2]='r',newDiectory[3]='o',newDiectory[4]='c',newDiectory[5]='/';
	// newDiectory="/proc/"
		while(UserID!=0){
		int ld=UserID%10;
		UserID/=10;
		procid[size++]=ld+48;
	}
	--size;
	while(size>=0)newDiectory[i++]=procid[size--];
	strcpy(exec_path,newDiectory);
	strcat(exec_path,"/exe");
	strcat(newDiectory,"/status");
	FILE *file =fopen(newDiectory,"r");
	if(file==NULL)perror("PID doesn't exist");
	else{
		char line[1000];
		while (fgets(line, sizeof line, file) != NULL){
			if (count==2||count == 3 || count== 6 || count==7)printf("%s", line);
			count++;

		}
		struct rusage usage;
		readlink(exec_path,exec,sizeof(exec));
		getrusage(RUSAGE_SELF,&usage);

		printf("Executable Path: %s\n",exec);
		printf("Memory Usage %ld\n",usage.ru_maxrss);
		fclose(file);   
	}
}
void pinfo(int i, char** commandTokens, char typeBitString[]){
	bool redirect = false, done = false, op=false;
	int fd0;
	if(strcmp(typeBitString,"0100")==0||strcmp(typeBitString,"0010")==0){
		done = true;
		char* file = NULL;
		int cnt=0,fd0;
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
			op = true;
			if(strcmp(typeBitString,"0100")==0)
				fd0=open(file, O_CREAT|O_WRONLY|O_TRUNC, 0777 );
			else
				fd0=open(file, O_APPEND|O_WRONLY|O_CREAT, 0777 );
			int pid = fork();
			redirect = true;
			if (pid==0){
				dup2(fd0,1);
				getcwd(currentDir, 200);
				printf("%s\n", currentDir);
				close(1);
			}

		}
	}
	else if(strcmp(typeBitString,"1100")==0||strcmp(typeBitString,"1010")==0){
		done = true;
		char* inputFile = NULL, outputFile = NULL;
		int cnt=0,fd0,fd1;
		for(;cnt<(i-1);cnt++){
			if(strcmp(commandTokens[cnt],">")==0||strcmp(commandTokens[cnt],">>")==0){
				if(cnt==(i-2))
					printf("Output file not specified\n");
				else{
					outputFile = commandTokens[cnt+1];
					break;
				}
			}
		}
		for(;cnt<(i-1);cnt++){
			if(strcmp(commandTokens[cnt],"<")==0){
				if(cnt==(i-2))
					printf("Input file not specified\n");
				else{

					inputFile = commandTokens[cnt+1];
					break;
				}
			}
		}
		if(outputFile){
			op = true;
			if(strcmp(typeBitString,"1100")==0)
				fd0=open(outputFile, O_CREAT|O_WRONLY|O_TRUNC, 0777 );
			else
				fd0=open(outputFile, O_APPEND|O_WRONLY|O_CREAT, 0777 );
			int pid = fork();
			if(inputFile){
				redirect = true;
				fd1=open(inputFile, O_RDONLY);
				char* buffer = (char*) malloc(sizeof(char)*1024);
				if(read(fd1, buffer, 1024)!=-1){
					close(fd1);
					strcpy(commandTokens[1],buffer);	
				}
				else{
					redirect = false;
					printf("ERROR: Input file does not exist.\n");
					close(fd0);
					return;
				}
			}
			if (pid==0)
				dup2(fd0,1);
		}
	}
	else if(strcmp(typeBitString,"1000")==0){
		done = true;
		char* file = NULL;
		int cnt=0,fd1;
		for(;cnt<(i-1);cnt++){
			if(strcmp(commandTokens[cnt],"<")==0){
				if(cnt==(i-2))
					printf("Input file not specified\n");
				else{
					file = commandTokens[cnt+1];
					break;
				}
			}
		}
		if(file){
			redirect = true;
			fd1=open(file, O_RDONLY);
			char* buffer = (char*) malloc(sizeof(char)*1024);
			if(read(fd1, buffer, 1024)!=-1){
				close(fd1);
				strcpy(commandTokens[1],buffer);	
			}
			else{
				redirect = false;
				printf("ERROR: Input file does not exist.\n");
				return;
			}
		}
	}
	if((i>2&&!done)||redirect){
		char *token;
		char cmd[1000][100];
		int noCommands=0;
		token=strtok(commandTokens[0],"\n");
		while(token){
			strcpy(cmd[noCommands],token);
			token=strtok(NULL, "\n");
			noCommands++;
		}
		token=strtok(cmd[0]," ");
		noCommands=0;
		char allcmd[100][100];
		while(token){
			strcpy(allcmd[noCommands],token);
			token=strtok(NULL, " ");
			noCommands++;
		}
		findPinfo(allcmd[1]);
	}
	else if(strcmp(commandTokens[0],"pinfo")==0){
		char exec_path[2000]={},exec[2000]={},newDiectory[2000]={},procid[20]={};
		newDiectory[0]='/',	newDiectory[1]='p',newDiectory[2]='r',newDiectory[3]='o',newDiectory[4]='c',newDiectory[5]='/';
		int i=6;
		int UserID=getpid();
		int size=0;
		while(UserID!=0){
			int ld=UserID%10;
			UserID/=10;
			procid[size++]=ld+48;
		}
		--size;
		while(size>=0)newDiectory[i++]=procid[size--];
		strcpy(exec_path,newDiectory);
		strcat(exec_path,"/exe");
		strcat(newDiectory,"/status");
		FILE *file =fopen(newDiectory,"r");
		char line[1000];
		while(fgets(line, sizeof line,file)!=NULL){
			if(count==2||count==3||count== 6||count==7)printf("%s", line);
			count++;
		}
		struct rusage usage;
		readlink(exec_path,exec,sizeof(exec));
		getrusage(RUSAGE_SELF,&usage);
		printf("Executable Path: %s\n",exec);
		printf("Memory Usage %ld\n",usage.ru_maxrss);
		fclose(file);   
	}
	if(op){
		close(fd0);
	}

}
