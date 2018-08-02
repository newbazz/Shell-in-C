#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <pwd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <stddef.h>
#include <sys/param.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#define BUFFERSIZE 2000
#define FALSE 0
#define TRUE !FALSE
extern  int alphasort();
char pathname[1000000];
bool IsPipe;
int globalVariablesIndex;
int i,j,count;
const char* cdCommand;
const char* pwdCommand;
const char* echoCommand;
const char* pinfoCommand;
const char* dirtyCommand;
const char* interruptsCommand;
const char* lsCommand;
bool previousDirSet;
char currentDir[200];
char homeDir[200], previousDir[200];
struct dirent *files;
int bprocess[100000][2];
int procCount;
