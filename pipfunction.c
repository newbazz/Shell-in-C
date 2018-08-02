#include "pipfunction.h"
void pipehelp(char * args[]){
	int filedes[2],filedes2[2],num_cmds=0,err=-1,end=0,i,j,k,l;
	i=j=k=l=0;
	char *command[256];
	pid_t pid;
	int in_red = 0,out_red = 0;
	while(args[l]!=NULL){
		if(strcmp(args[l],"|")==0){
			num_cmds++;
		} 
		l++;
	}
	if(num_cmds)IsPipe=true;
	else return;
	num_cmds++;         
	l=0;
	while(args[l]!=NULL){
		if (strcmp(args[l],"<")==0)in_red = l+1;
		else if(strcmp(args[l],">")==0)out_red = l+1;
		l++;
	}
	while(args[j]!=NULL&&end!=1){
		k=0;
		while(!(strcmp(args[j],"|")==0||strcmp(args[j],"<")==0||strcmp(args[j],">")==0)){
			command[k]=args[j++];	
			if(args[j]==NULL){
				end=1;
				k++;
				break;
			}
			k++;
		}
		command[k]=NULL;
		j++;
		if(i%2!=0)pipe(filedes); 
		else pipe(filedes2);
		if(i==0&&in_red &&strcmp(args[j-1],"<")==0)j+=2;

		if(i==num_cmds-1 && out_red && strcmp(args[j-1],">") == 0)
			j+=2;
		pid=fork();
		if(pid==-1){			
			if(i!=num_cmds-1){
				if(i%2!=0)close(filedes[1]); 
				else close(filedes2[1]);
			}			
			printf("Child process could not be created\n");
			return;
		}
		if(pid==0){
			if (i == 0){
				if(in_red){
					int in=open(args[in_red],O_RDONLY);
					dup2(in,STDIN_FILENO);
					close(in);
				}
				dup2(filedes2[1], STDOUT_FILENO);
			}
			else if(i==num_cmds-1){
				if(out_red){
					int out=open(args[out_red],O_WRONLY|O_CREAT,0644);
					dup2(out, STDOUT_FILENO);
					close(out);
				}
				if(num_cmds%2!=0)dup2(filedes[0],STDIN_FILENO);
				else dup2(filedes2[0],STDIN_FILENO);
			}
			else{
				if(i%2!=0){
					dup2(filedes2[0],STDIN_FILENO); 
					dup2(filedes[1],STDOUT_FILENO);
				}
				else{ 
					// for even i
					dup2(filedes[0],STDIN_FILENO); 
					dup2(filedes2[1],STDOUT_FILENO);					
				} 
			}
			if (execvp(command[0],command)==err){
				kill(getpid(),SIGTERM);
				}	
		}
		if(i==0){
			close(filedes2[1]);
		}
		else if(i==num_cmds-1){
			if (num_cmds % 2 != 0)close(filedes[0]);
			else close(filedes2[0]);
		}
		else{
			if(i%2!=0){					
				close(filedes2[0]);
				close(filedes[1]);
			}
			else{					
				close(filedes[0]);
				close(filedes2[1]);
			}
		}

		waitpid(pid,NULL,0);      
		i++;	
	}
}
int  parse(char *line, char **argv){
	int count=0;
	while (*line != '\0') {      
		while (*line == ' ' || *line == '\t' || *line == '\n')
			*line++ = '\0';     

		*argv++ = line;              
		count++;

		while (*line != '\0' && *line != ' ' &&  
				*line != '\t' && *line != '\n') 
			line++;             
	}
	*argv = '\0';                  
	return count;
}
void spacemaker(char buffer[],char mainbuf[]){
	int k=0,i=0;
	while(buffer[i]!='\n' && buffer[i]!='\0'){
		if(buffer[i]!='|' && buffer[i]!='>' && buffer[i]!='<')mainbuf[k]=buffer[i];
		else {mainbuf[k]=' '; k++; mainbuf[k]=buffer[i]; k++; mainbuf[k]=' ';}
		k++;
		i++;
	}
	return;
}
void pipehelper(char* buffer){
	char mainbuf[BUFFERSIZE]={0};
	char *argv[100];
	spacemaker(buffer,mainbuf);
	int num=parse(mainbuf,argv);
	argv[num]=NULL;
	int i=0;
	pipehelp(argv);
	return;
}
