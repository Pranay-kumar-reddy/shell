#include "shell.h"
int command_setenv(char **args)
{
	int i=0,c=0;
	while(args[i]!=NULL){
		c=c+1;
        i++;
        }
	if(c <= 1)fprintf(stderr,"setenv: Too few arguments\n");
    else if(c > 3)fprintf(stderr, "setenv: Too many arguments\n");        
	else if(c == 3)
	{
		if(setenv(args[1], args[2], 1) == -1)
		{
			perror("Shell");
		}			
	}
	else if(c == 2)
	{
		if(setenv(args[1],"", 1) == -1)
		{
			perror("Shell");
		}		
	}
	return 1;
}

int command_unsetenv(char **args)
{
	int i=0,c=0;
	while(args[i] != NULL){
		c=c+1;
        i++;}
	if (c >2) fprintf(stderr,"unsetenv: Too many arguments\n");
	else if (c == 1) fprintf(stderr,"unsetenv: Too few arguments\n");
	else
	{
		if(unsetenv(args[1]) == -1)
		{
			perror("Shell");
		}		
	}	
	return 1;
}
void command_ctrl_c(int sig)
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
int command_ls(char ** get_space_seperated)
    {
        char date[100];
        DIR *dirp;
        struct dirent *name;
        struct stat dir;
        int check_a = 0, check_l = 0, dir_c = 0, j = 0,i;
        for (i = 1; get_space_seperated[i] != NULL; i++)
            {
                if(strcmp(get_space_seperated[i], "-a") == 0) {check_a = 1;}
                else if (strcmp(get_space_seperated[i], "-l") == 0) check_l = 1;
                else if (strcmp(get_space_seperated[i], "-al") == 0 || strcmp(get_space_seperated[i], "-la") == 0)
                    {
                        check_a = 1;
                        check_l = 1;
                    }
                else if(strcmp(get_space_seperated[i], " ") != 0) 
                    {
                        dir_c++;
                    }
            }        
                int dir_check = 0;
                if (dir_c == 0) 
                    {
                        dir_c = 1;
                        dir_check = 1;
                    }
                    
                char **dir_names = (char **)malloc(dir_c * 8);
                for(int i = 0; i < dir_c; i++) 
                    {
                        dir_names[i] = (char *)malloc(50);
                    }
                
                for(i = 1; get_space_seperated[i] != NULL; i++) 
                    { 
                        if(strcmp(get_space_seperated[i], "-al") != 0 && strcmp(get_space_seperated[i], "-la") != 0 && strcmp(get_space_seperated[i], "-a") != 0 && strcmp(get_space_seperated[i], "-l") != 0)
                            {
                                dir_names[j] = get_space_seperated[i];
                                if(strcmp(dir_names[j],"~")==0)
                                    {
                                        strcpy(dir_names[j],home_dir);
                                    }
                                j++;                     
                            // if(j>1)
                            // {
                            //     if(dir_names[j-2][strlen(dir_names[j-2])-1]== 92)
                            //     {
                            //         dir_names[j-2][strlen(dir_names[j-2])]=32;
                            //         strcat(dir_names[j-2],dir_names[j-1]);
                            //         j--;
                            //         dir_c-=1;
                            //     }
                            // }    
                                // dir_names[j] = get_space_seperated[i];
                                // j++;                    
                            }
                    }
                if(dir_check==1) dir_names[0] = ".";
                for(i = 0; i < dir_c; i++) 
                    {
                        dirp = opendir(dir_names[i]);
                        if (dirp == NULL) 
                            {
                                printf("%s: No such file or dir\n", dir_names[i]);
                                continue;
                            }
                        if(dir_names[0]!=".")
                        {    
                        printf(red_colour "%s\n" reset_colour, dir_names[i]);
                        }
                        while((name = readdir(dirp)) != NULL)
                            {
                                stat(name->d_name, &dir);                           
                                if(check_a==0 && check_l==0)
                                    {
                                        if(name->d_name[0] != '.')
                                        {
                                            printf("%s\n", name->d_name);
                                        }    
                                    }    
                                else if(check_a==1 && check_l==0) 
                                    {   
                                        printf("%s\n", name->d_name);
                                    }    
                                else if(check_l==1) 
                                    {   
                                        if(check_a==1 || (check_a==0 && name->d_name[0] != '.'))
                                            {
                                                mode_t bit1 = dir.st_mode;
                                                if(S_ISDIR(bit1)!=0) printf("d"); else printf("-");
                                                if((bit1 & S_IRUSR) != 0) printf("r"); else printf("-");
                                                if((bit1 & S_IWUSR) != 0) printf("w"); else printf("-");
                                                if((bit1 & S_IXUSR) != 0) printf("x"); else printf("-");
                                                if((bit1 & S_IRGRP) != 0) printf("r"); else printf("-");
                                                if((bit1 & S_IWGRP) != 0) printf("w"); else printf("-");
                                                if((bit1 & S_IXGRP) != 0) printf("x"); else printf("-");
                                                if((bit1 & S_IROTH) != 0) printf("r"); else printf("-");
                                                if((bit1 & S_IWOTH) != 0) printf("w"); else printf("-");
                                                if((bit1 & S_IXOTH) != 0) printf("x"); else printf("-");
                                                printf(" %d ", (int)dir.st_nlink);
                                                struct group  *gr = getgrgid(dir.st_gid);
                                                struct passwd *pw = getpwuid(dir.st_uid);                                   
                                                printf(" %s", pw->pw_name);
                                                printf(" %s", gr->gr_name);
                                                strftime(date, 100, "%h %d %H:%M", localtime(&(dir.st_ctime)));
                                                printf(" %s", date);
                                                printf("\t%d", (int)dir.st_size);
                                                printf(" \t%s\n", name->d_name);
                                            }                                    
                                    }                                  
                            }
                    }
            closedir(dirp);
            return 1;
    }                    

int command_pwd()
{
  char cwd[200];
  getcwd(cwd, 200);
  printf("%s\n", cwd);
  return 1;
}

int command_cd(char **args) 
{
    if (args[1] == NULL) 
        {    
            args[1]=(char*)malloc(200*sizeof(char));
            strcpy(args[1],home_dir);
        }
    else if(args[1][0]=='~')
        {   
            char *temp = malloc(200 * sizeof(char));
            strcpy(temp,home_dir);
            int i,len=strlen(home_dir),len2=strlen(args[1]);
            for(i=1;i<len2;i++)
            {
                temp[len+i-1]=args[1][i];
            }
            // printf("%s",temp);   
            strcpy(args[1],temp);
            // free(temp);
        }
        int k=0,j=0;
        for(int i=1;args[i+1]!=NULL;i++)
        {
            k++;
        int len_arg=strlen(args[i]);
                                {
                                    if(args[i][len_arg-1]==92)
                                    {   
                                        args[i][len_arg-1]=32;                                                                            
                                        strcat(args[1],args[i+1]);
                                        j++;
                                        // printf("%s",args[1]);
                                    }
                                }

        }
        // printf("%d %d ",j,k);
        if(j!=k)
        {
            fprintf(stderr, "shell: too many arguments\n");
            return 1;
        }
            // printf("%s",home_dir);

        if (chdir(args[1]) < 0) 
        {   
            perror("run");
        }
        else
        change_cur_dir(args[1]);
            //  printf("%s",home_dir);
    return 1;
}

int command_echo(char **args) 
{
  int i,j=0,k=0;
  for(i=1;args[i]!=NULL;i++)
  {
  if(args[1][0] == 34)
  {args[1][0]=127;j++;}   
        if(args[i][strlen(args[i])-1] == 34)
        {
            args[i][strlen(args[i])-1] =127;
            j--;
        }
    }
    for(i=1;args[i]!=NULL;i++)
  {
  if(args[1][0] == 39)
  {args[1][0]=127;k++;}   
        if(args[i][strlen(args[i])-1] == 39)
        {
            args[i][strlen(args[i])-1] =127;
            k--;
        }
    }  
    if(j==0 && k==0)
    {
     for(i=1;args[i]!=NULL;i++)
     {
        printf("%s ", args[i]);        
     }     
    } 
    else
        printf("error:cant find other quote"); 
    printf("\n"); 
  return 1;
}

void command_pinfo(char **args)
{
  char info[3000],pid[20];
  if(args[1]!=NULL)
  {
    strcpy(pid, args[1]);
  }
  else 
  {
    sprintf(pid, "%d", getpid());
  }
  char dir[]="/proc/"; 
  char path[100];
  char **stat;
  strcat(dir, pid);
  strcpy(path, dir);
  strcat(dir, "/stat");
  strcat(path, "/exe");
  char buf[1000];
  int t = readlink(path, buf, sizeof(buf));
  if(t == -1)
  {
    strcpy(buf, "broken link");
  }
  int fd = open(dir, O_RDONLY);
  if(fd == -1)
  {
    perror("shell");
    return;
  }
  read(fd, info, 250);
  stat = parsing(info, " \t\r\n\a");
  change_cur_dir(buf);
  printf("pid -- %s\nProcess Status -- %s\n- %s {Virtual Memory}\n- Executable Path -- %s\n", stat[0], stat[2], stat[23], buf);
}


