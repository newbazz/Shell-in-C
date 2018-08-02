#include "bg.h"
int bgs(char **args)
{

        if(args[1]==NULL)
        {
                fprintf(stderr, "Usage: fg <jobid>\n");
        }
        pid_t wpid;
        int jbid=atoi(args[1]);
        printf("%s\n",backgrund_process[jbid].command);
        int status;
        backgrund_process[jbid].state=1;
        kill(backgrund_process[jbid].pid,SIGCONT);

        do {
                wpid = waitpid(backgrund_process[jbid].pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        return 1;
}