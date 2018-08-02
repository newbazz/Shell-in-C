#include "interrupts-command.h"
void watch(char* count){
	FILE *fd;
	int i,n=0;
	n=atoi(count);
	while(1){
		int count=0;
		char line[1000];
		fd=fopen("/proc/interrupts","r");
		while (fgets(line,sizeof line,fd))
		{
			if(strstr(line,"i8042") || strstr(line,"IWI") || count==0)
				printf ("%s", line);
			count++;
		}
		fclose(fd);
		sleep(n);
	}
	fclose(fd);
}
