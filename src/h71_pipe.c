#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void h71_pipe(){
    char temp_buf[MAX_COM];
    char * token;
    strcpy(temp_buf,cmd_w);
    token = strtok(temp_buf," ");
    if(!strcmp(token,"|")){
        prompt_error_write("pipe: Pipe cannot read from NULL");
        new_line_error();
        success=1;
        cmd_w[0]='\0';
        return;
    }
    int pipe_check=0;
    // checking for the existence of pipes
    while(token!=NULL){
        if(!strcmp(token,"|")){
            pipe_check++;
            token=strtok(NULL," ");
            if(token == NULL){
                prompt_error_write("pipe: Pipe cannot write to NULL");
                new_line_error();
                success=1;
                cmd_w[0]='\0';
                return;
            }
        }
        token=strtok(NULL," ");
    }
    if(!pipe_check){
        return;
    }
    int fd_in = STDIN_FILENO; // intial input fd
    strcpy(temp_buf,cmd_w);
    token = strtok(temp_buf,"|");

    for(int i=0;i<=pipe_check;i++){
        int pipefd[2];
        pipe(pipefd);

        strcpy(cmd_w,token);
        token=strtok(NULL,"|");
        clean_command(cmd_w);

        pid_t pid = fork();
        if(pid<0){
            prompt_error_write("pipe: process could not be created");
            new_line_error();
            success=1;
            cmd_w[0]='\0';
            return;
        }
        else if(pid==0){
            dup2(fd_in,0);
            if(i!=pipe_check){
                dup2(pipefd[1],1);
            }
            close(pipefd[0]);
            h71_redir();
            if(cmd_w[0]!='\0') command_parse();
            exit(0);
        }
        else{
            int status;
            waitpid(pid,&status,WUNTRACED);
            close(pipefd[1]);
            fd_in = pipefd[0];
            if(i==pipe_check){
                close(pipefd[0]);
            }
        }
    }
    cmd_w[0]='\0';
}