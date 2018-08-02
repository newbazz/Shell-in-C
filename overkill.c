#include "overkill.h"
#include "signal.h"

void ok(char* args[1000]){
	int z=0;
	int retttt=fork();
	if(retttt==0){
		for(z=0;z<procCount;z++){
			if(bprocess[z][1]==1){
				bprocess[z][1]=0;
				printf("%d\n",bprocess[z][0]);
				kill(bprocess[z][0],9);
				// signal(SIGCHLD,siginthandler);
			}
		}
	}
}