#include "setenv.h"
void set_env(char* args[1000]){
	// printf("%s %s %sl",args[0],args[1],args[2]);
	if(getenv(args[1])==NULL)setenv(args[1],"",1);
	if(strlen(args)<=2)setenv(args[1],"",1);
	else setenv(args[1],args[2],1);
}
