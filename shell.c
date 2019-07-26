#include "shell.h"
int main(int argc, char const *argv[]) 
{ 
  childPID = -1;
  head = (qjob*)malloc(sizeof(qjob));
  mainShellPID = getpid();
  signal(SIGINT, command_ctrl_c);
  processing();
  return 0;
} 

int command_execute(char **args, int background_check)
    {
        if (args[0] == NULL) return 1;
        if(strcmp(args[0], "ls")==0 || strcmp(args[0], "l")==0) command_ls(args);
        else if (strcmp(args[0], "cd")==0) 
            {
                command_cd(args);
            } 
        else if (strcmp(args[0], "pwd")==0)
            {
                command_pwd();
            } 
        
        else if (strcmp(args[0], "echo")==0) 
            {
                command_echo(args);
            }
        else if (strcmp(args[0], "quit")==0) 
            {
                exit(0);
            }
        else if(strcmp(args[0], "pinfo")==0) 
            {
                command_pinfo(args);
            }
        else if(strcmp(args[0], "setenv")==0)
        {
             command_setenv(args);
        }  
        else if(strcmp(args[0], "unsetenv")==0)
        {
             command_unsetenv(args);
        }  
        else if(strcmp(args[0], "jobs")==0)
        {
             update_jobs(head);
             print_jobs(head);
        }  
        else if(strcmp(args[0], "kjob")==0)
        {
             command_kjob(args);
        }

        else if(strcmp(args[0], "fg")==0)
        {
             command_fg(args);
        } 
        else if(strcmp(args[0], "bg")==0)
        {
             command_bg(args);
        }
        else if(strcmp(args[0], "overkill")==0)
        {
             kill_all(args);
        }         

        else {
                return command(args, background_check);
            }    
        return 1;
    }