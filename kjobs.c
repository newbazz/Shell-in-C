#include "kjobs.h"
#include "signal.h"
void kjob(char* args[1000]){
	int o=atoi(args[1]);
	int m=atoi(args[2]);
	printf("%d %d\n",bprocess[o][0],m);
	bprocess[o][1]=0;
	kill(bprocess[o][0],m);
	// signal(SIGCHLD,siginthandler);
}
