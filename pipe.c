#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <termios.h>
#include <stdio.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/time.h>

#define BUFFERSIZE 2000

void pipehelp(char * args[]){
	// 2 set of file descriptors
	/* filedes2 for even i and filedes for odd i
		command 1: ls -l        i=0       filedes[0] has output; [1] must be closed
		command 2: grep .c      i=1	  filedes[1] is the input; filedes2[0] has the output
		command 3: wc -l        i=2       filedes2[1] is the input
	*/   
	int filedes[2],filedes2[2],num_cmds=0,err=-1,end=0,i,j,k,l;
	i=j=k=l=0;
	char *command[256];
	pid_t pid;
	int in_red = 0,out_red = 0; //diff k n > l | wc 
	while(args[l]!=NULL){		// to find the total number of arguments which is (number of pipes + 1)
		if(strcmp(args[l],"|")==0){
			num_cmds++;
		} 
		l++;
	}
	num_cmds++;            // num_cmd has command length (total number of commands) exchanging between pipes
	l=0;
	while(args[l]!=NULL){
		if (strcmp(args[l],"<")==0){
			in_red = l+1;
		}
		else if(strcmp(args[l],">")==0){
			out_red = l+1;
		}
		l++;
	}
	// main loop
	while(args[j]!=NULL&&end!=1){
		k=0;
		while(!(strcmp(args[j],"|")==0||strcmp(args[j],"<")==0||strcmp(args[j],">")==0)){
			command[k]=args[j++];	
			if(args[j]==NULL){
				// again in the loop when no more arguments are found
				end=1;
				k++;
				break;
			}
			k++;
		}
		// execvp needs last arg to be NULL
		command[k]=NULL;
		j++;
		// old fd remains as it is, new one is created
		if(i%2!=0)
			pipe(filedes); // for odd i
		else
			pipe(filedes2); // for even i

		if(i==0&&in_red &&strcmp(args[j-1],"<")==0)
			j+=2;

		if(i==num_cmds-1 && out_red && strcmp(args[j-1],">") == 0)
			j+=2;
		pid=fork();
		if(pid==-1){			
			if(i!=num_cmds-1){
				// good to close before exiting the function
				if (i % 2 != 0)
					close(filedes[1]); // for odd i
				else
					close(filedes2[1]); // for even i
			}			
			printf("Child process could not be created\n");
			return;
		}
		if(pid==0){
			// If we are in the first command
			if (i == 0){
				if(in_red){
					int in=open(args[in_red],O_RDONLY);
					dup2(in,STDIN_FILENO);
					close(in);
				}
				dup2(filedes2[1], STDOUT_FILENO);
			}
			// last command; nothing to be done to stdout
			else if(i==num_cmds-1){
				if(out_red){
					int out=open(args[out_red],O_WRONLY|O_CREAT,0644);
					dup2(out, STDOUT_FILENO);
					close(out);
				}
				if(num_cmds%2!=0){
					// for odd number of commands
					dup2(filedes[0],STDIN_FILENO);
				}
				else{ 
					// for even number of commands
					dup2(filedes2[0],STDIN_FILENO);
				}
			// write end[1] of a descriptor in previous command becomes read end[0] in the present one	
			}
			else{ // for odd i
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
		// closing all the opened(used) descriptors
		if(i==0){
			close(filedes2[1]);
		}
		else if(i==num_cmds-1){
			if (num_cmds % 2 != 0){					
				close(filedes[0]);
			}
			else{					
				close(filedes2[0]);
			}
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

		waitpid(pid,NULL,0);      // waiting for the pid to get over
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
	// char buffer[BUFFERSIZE]={0};
	char mainbuf[BUFFERSIZE]={0};
	bzero(buffer, BUFFERSIZE);  // eg. ls | grep .c
	// fgets(buffer, BUFFERSIZE, stdin);
	char *argv[100];
	spacemaker(buffer,mainbuf);
	int num=parse(mainbuf,argv);
	argv[num]=NULL;
	int i=0;
	pipehelp(argv);
	printf("DONE\n");
	return (0);
}
