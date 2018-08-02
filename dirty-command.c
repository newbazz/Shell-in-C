#include "dirty-command.h"
void dirty(char* cmd){
	char temp[100];
	time_t lasttime, thistime;
	lasttime = time(NULL);
	int numsec = atoi(cmd);
	while(1){
		while(1){
			thistime = time(NULL);
			if(thistime - lasttime >= numsec)
				break;
			if(thistime - lasttime >= 2)
				sleep(thistime - lasttime - 1);
		}
		FILE * fp = fopen("/proc/meminfo","r");
		while(!feof(fp)){
			fgets(temp,100,fp);
			char* pos;
			if ((pos=strchr(temp, '\n')) != NULL) *pos = '\0';
			if(strstr(temp,"Dirty:")){
				int i,j=0,flag=0;
				char mem[50];
				for (i=0,j=0; i < strlen(temp); ++i)
				{
					if(isdigit(temp[i]))
						flag=1;
					if (flag==1)
						mem[j++] = temp[i];
				}
				mem[j]='\0';
				printf("%s\n", mem);
			}
		}	
		lasttime += numsec;
	}

}
