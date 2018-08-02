#include "jobs.h"
void job(char* args[1000]){
	int z=0;
	if(!procCount)printf("No process in background\n");
	for(int z=0;z<procCount;z++){
		printf("%d ",bprocess[z][0]);
		if(bprocess[z][1])printf("Running\n");
		else printf("Stopped\n");
	}
	return;
}