#include "shell.h"
void handler()
{   int status;
    pid_t pid = waitpid(-1, &status, WNOHANG | WUNTRACED);
    // pid = wait(NULL);
    background_c--;
    if(pid!=-1)
        printf("\n[%d] done.\n", pid);

}
void handler2(int sig)
{  
	printf("\n");
	if(getpid() != mainShellPID)
		return;
	if(childPID != -1)
	{
		kill(childPID, SIGINT);
	}
	signal(SIGINT, command_ctrl_c);	
}
int command(char **args, int background_check)
{
  int pid,status;
    // printf("%s %s %s",args[0],args[1],args[2]);
  pid = fork();
  if (pid == 0) 
  { 
    setpgid(0,0);
    if (execvp(args[0], args) == -1)
    {
      perror("shell");
    }
    exit(1);
  }
  else if (pid < 0) 
  {
    perror("shell");
    return 1;
  }
  else if (background_check==0)
  { 
    childPID=pid;
    waitpid(pid, &status, WUNTRACED);
    signal(SIGKILL,handler2);
  }
  else 
  { childPID=pid;
    signal(SIGCHLD, handler);
    // printf("hai");
    background_c++;
    printf("[%d] %d\n", background_c, pid);
    // waitpid(pid, &status, WNOHANG);
    insert_into_list(args[0],pid,1);
  }

return 1;
}
