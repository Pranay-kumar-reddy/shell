#include "shell.h"
char *reading_input() 
{
    char *input = NULL;
    ssize_t size = 0;
    getline(&input, &size, stdin);
    return input;
}
void change_cur_dir(char cur_dir[])
{
    int i, flag=0, len = strlen(cur_dir),j,len2=strlen(home_dir);
    char new_cur_dir[200];
    for(i=0; i<len2; i++) 
    {
        if(cur_dir[i]!=home_dir[i])
            {
                flag = 1;
                break;
            }
    }
    if(flag==1) return;
    else
        {
            new_cur_dir[0]='~';
            for(j=1; i<len; i++, j++)
                {
                    new_cur_dir[j] = cur_dir[i];
                }
            strcpy(cur_dir,new_cur_dir);
        }
}

void processing()
{   
    int status,i,c=0,background_check,check=0;
    char *read_input,**get_commands,**get_space_seperated,**get_pipe_seperated,**get_reright_seperated,**get_releft_seperated,**output_file,**input_file;
    do
    {   childPID = -1;
        getcwd(cur_dir, 100);
        if(check==0)
            {                
                strcpy(home_dir,cur_dir);
                check=1;
                background_c=0;
            }
            // printf("%s",home_dir);
        char* username = getenv("USER");
        if(strcmp(cur_dir,home_dir)==0)
            {
                strcpy(show_cur_dir,"~");
            }
        else
            {
                strcpy(show_cur_dir,cur_dir);
                change_cur_dir(show_cur_dir);
            }
        gethostname(hostname, 200);
        printf(blue_colour "<%s@%s:%s> " reset_colour, username, hostname, show_cur_dir);
        read_input=reading_input();
        get_commands=parsing(read_input,";");
        for (i = 0; get_commands[i] != NULL; i++)
            {   
                int loop_input,loop_output,fd1_open,fd2_open,stdo=dup(1),stdi=dup(0),stdo_d=dup(1),stdi_d=dup(0),ifin=0,ifout=0,pipe_array[2];
                loop_input=dup(0);loop_output=dup(1);
                c=0;
                background_check=0;
                get_pipe_seperated = parsing(get_commands[i], "|");
                // int ll=0;
                // while(get_pipe_seperated[ll]!=NULL){
                    // printf("%s\n",get_pipe_seperated[0]);
                //     ll++;
                // }
                for(int k=0;get_pipe_seperated[k] != NULL;k++)
                {
                    dup2(loop_input,0);
                    close(loop_input);
                    if(get_pipe_seperated[k+1]==NULL)
                    {
                        loop_output=dup(stdo);
                    }
                    else
                    {
                        pipe(pipe_array);
                        loop_output=pipe_array[1];
                        loop_input=pipe_array[0];
                    }
                    dup2(loop_output,1);
                    close(loop_output);
                int check1 = check_append(get_pipe_seperated[k]);
                get_reright_seperated = parsing(get_pipe_seperated[k], ">");
                if(get_reright_seperated[1]!=NULL){output_file=parsing(get_reright_seperated[1]," \t\r\n\a");ifout=1;}
                get_releft_seperated = parsing(get_reright_seperated[0], "<");
                if(get_releft_seperated[1]!=NULL){input_file=parsing(get_releft_seperated[1]," \t\n\a\r");ifin=1;}
                if(ifout==1){
                    if(check1){fd1_open=open(output_file[0],O_WRONLY | O_APPEND | O_CREAT, 0644);}
                    else fd1_open=open(output_file[0],O_WRONLY | O_TRUNC | O_CREAT, 0644);
                    if(fd1_open<0)perror("open");
                    stdo_d=dup(1);
                    dup2(fd1_open,1);
                    close(fd1_open);
                }
                if(ifin==1){
                    fd2_open=open(input_file[0],O_RDONLY);
                    if(fd2_open<0)perror("open");
                    stdi_d=dup(0);
                    dup2(fd2_open,0);
                    close(fd2_open);
                }

                get_space_seperated = parsing(get_releft_seperated[0], " \t\r\n\a");
                if(get_space_seperated[0]==NULL) continue;
                while(get_space_seperated[c]!=NULL)
                {
                    c++;
                }
                if (get_space_seperated[c-1][strlen(get_space_seperated[c-1])-1] == '&')
                    {
                        background_check=1;
                        if(strlen(get_space_seperated[c-1])==1)
                        {get_space_seperated[c-1]=NULL;}
                        else
                        {
                            get_space_seperated[c-1][strlen(get_space_seperated[c-1])-1] = 0;
                        }    
                    } 
                if(background_check==1){

                }      
                status = command_execute(get_space_seperated,background_check);
                free(get_space_seperated);  

                if(ifout==1){
                    dup2(stdo_d,1);
                    close(stdo_d);
                }
                if(ifin==1){
                    dup2(stdi_d,0);
                    close(stdi_d);
                }
            }
            dup2(stdi,0);
            close(stdi);
            dup2(stdo,1);
            close(stdo);
            }
        free(read_input);    
    }while(status);
}