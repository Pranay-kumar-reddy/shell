#include<stdio.h>
#ifndef main_file
#define main_file
#include "dirent.h"
#include "time.h"
#include "pwd.h"
#include "grp.h"
#include "string.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void processing();
char *reading_input();
char **parsing(char *read_input, char *delimitor);
void change_cur_dir(char cur_dir[]);
int command_execute(char **args, int background);
int command_ls(char ** get_space_seperated);
int command_pwd();
int command_cd(char **args);
int command_echo(char **args); 
int command(char **args, int background_check);
void command_pinfo(char **args);
int check_append(char *read_input);
int command_setenv(char **args);
int command_unsetenv(char **args);
void command_ctrl_c(int sig);
typedef struct node{
    char name[500];
    pid_t pid;
    int stat;
    struct node *next;
}qjob;
void insert_into_list(char [], pid_t, int);
void print_jobs();
void remove_jobs(pid_t);
int killLL(pid_t , int);
void printDoneJobs( );
void print_jobssize();
void kill_all(char ** args);
void change_state(pid_t , int);
void update_jobs();
int command_bg(char **args);
int command_fg(char **args);
int command_kjob(char **args);


qjob* head;
qjob* retrive_job(int );
char home_dir[300],cur_dir[300], hostname[300], show_cur_dir[300];
int background_c;
pid_t mainShellPID;
pid_t childPID;
char presentProcess[1000];
#define red_colour   "\x1b[31m"
#define reset_colour "\x1b[0m"
#define blue_colour  "\x1b[34m"

#endif