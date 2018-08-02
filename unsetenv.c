#include "unsetenv.h"
void unset_env(char* arg[1000]){
	if(getenv(arg[1])==NULL)printf("No such enviorment variable\n");
	else if(unsetenv(arg[1])!=0)printf("Unidentified error\n");
}