#include "shell.h"
qjob* retrive_job(int cur_pid)
{int c = 0;
	qjob* cur = head;
	qjob* present;
	while(cur)
	{
		present = cur->next;
		c += 1;
		if(present != NULL)
		{
			if(c == cur_pid)
			{
				return present;
			}
		}
		cur = present;
	}
}

void print_jobs()
{
	qjob* cur = head->next;
	int c  = 0;
	while(cur)
	{
		c += 1;
		printf("[%d] \t %s \t %s \t [%d]\n",c, (cur->stat == 0)?"Stopped":"Running", cur->name, cur->pid);
		cur = cur->next;
	}
}

void remove_jobs(pid_t pid)
{
	qjob* cur = head;
	qjob* present;
	while(cur)
	{
		present = cur->next;
		if(present != NULL)
		{
			if(present->pid == pid)
			{
				cur-> next = present->next;
				free(present);
				break;
			}
		}
		cur = present;
	}
}

int killLL(pid_t pid, int sig)
{
	qjob* cur = head;
	qjob* present;
	while(cur)
	{
		present = cur->next;
		if(present != NULL)
		{
			if(present->pid == pid)
			{
				cur-> next = present->next;
				kill(pid, 17);
				kill(pid, sig);
				fprintf(stderr,"Killed %d - %s\n" , pid, present->name);
				free(present);
				return 1;
				break;
			}
		}
		cur = present;
	}
	return 0;
}

void kill_all(char **args)
{
	qjob* cur = head->next;
	int copy;
	while(cur)
	{	
		copy = cur->pid;
		cur = cur->next;
		killLL(copy, SIGINT);
	}
}

void update_jobs()
{   int temp;
	qjob* cur = head;
	qjob* present;
	while(cur)
	{
		present = cur->next;
		if(present != NULL)
		{
			pid_t return_pid = waitpid(present->pid,&temp, WNOHANG);
			if(return_pid >0)
			{
				if(WIFSTOPPED(temp)) present->stat=0;
                else if(WIFCONTINUED(temp)) present->stat=1;
                else if(WIFEXITED(temp)||WIFSIGNALED(temp)) remove_jobs(present->pid);
            }
            else if(return_pid<0){
                remove_jobs(present->pid);
            }
		}
		cur = cur->next;
	}
}


int command_kjob(char **args)
{
	int c = 0, i;
	for ( i = 0; args[i] != NULL; ++i)
		c = c + 1;
	if(c >3)
			fprintf(stderr,"kjob: Too many arguments\n");
	else if(c < 3)
			fprintf(stderr,"kjob: Too few arguments\n");
	else
	{
		int x = atoi(args[1]);
		int sig = atoi(args[2]);
		qjob* job_node = retrive_job(x);
		if(job_node !=NULL)
		{
			int pid = job_node -> pid;
			kill(pid,sig);
		}
		else
		{
			fprintf(stderr,  "kjob: No such pid exists\n" );
		}

	}

	return 1;
}

int command_fg(char **args)
{
	int c = 0, i;
	for ( i = 0; args[i] != NULL; ++i)
		c = c + 1;
	if(c >2)
			fprintf(stderr, "fg: Too many arguments\n" );
	else if(c <2)
			fprintf(stderr, "fg: Too few arguments\n" );
	else
	{

		int x = atoi(args[1]);
		qjob* job_node = retrive_job(x);
		if(job_node !=NULL)
		{
			int pid = job_node -> pid;
			childPID = pid;
			kill(pid, SIGCONT);
			strcpy(presentProcess, job_node->name);
			remove_jobs( pid);
			waitpid(-1,NULL,WUNTRACED);
		}
		else
		{
			fprintf(stderr, "fg: No such pid exists\n" );
		}
	}

	return 1;
}

int command_bg(char **args)
{
	int c = 0, i;
	for ( i = 0; args[i] != NULL; ++i)
		c = c + 1;
	if(c >2)
			fprintf(stderr, "bg: Too many arguments\n" );
	else if(c < 2)
			fprintf(stderr, "bg: Too few arguments\n" );
	else
	{
		int inC = atoi(args[1]);
		qjob* job_node = retrive_job( inC);
		if(job_node !=NULL)
		{
			int pid = job_node -> pid;
			kill(pid, SIGTTIN);
			kill(pid, SIGCONT);
			change_state(pid,1);
		}
		else
		{
			fprintf(stderr, "bg: No such pid exists\n" );
		}
	}
	return 1;
}

void insert_into_list(char name[], pid_t pid , int stat)
{
	qjob* cur = head;
	qjob* present;
	while(cur)
	{
		present = cur->next;
		if(present == NULL)
		{
			present  = (qjob*)malloc(sizeof(qjob));
			strcpy(present->name, name);
			present->next = NULL;
			present->stat = stat;
			present->pid = pid;
			cur->next = present;
			break;
		}
		cur = present;
	}
}
void change_state(pid_t pid, int stat)
{
	qjob* cur = head;
	qjob* present;
	int c = 0;
	while(cur)
	{
		present = cur->next;
		c += 1;
		if(present != NULL)
		{
			if(present->pid == pid)
			{
				present->stat = stat;
				break;
			}
		}
		cur = present;
	}
}

