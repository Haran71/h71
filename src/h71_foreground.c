#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void h71_foreground(char * token){
    int pid = fork();
    gettimeofday(&tv,NULL);
    start = (tv.tv_sec)*1000000 + tv.tv_usec;
    if(pid < 0){
        prompt_error_write("New process creation failed");
        new_line_error();
        return;
    }
    else if (pid==0){
        //first we have to get the arguments
        int n_args = 1; // the command itself is a part of the argumentrs
        for(int i=0;i<strlen(token);i++){
            if(isspace(token[i])){
                n_args++;
            }
        }
        char * args[n_args+1];
        args[n_args] = NULL;
        int arg =0;
        int pos =0;
        for(int i=0;i<n_args;i++){
            args[i] = (char*)malloc((MAX_COM/100)*sizeof(char));
        }
        for(int i=0;i<strlen(token);i++){
            if(isspace(token[i])){
                args[arg++][pos] = '\0';
                pos=0;
                continue;
            }
            args[arg][pos++] = token[i];
        }
        args[arg][pos] = '\0';
        if(args[0][0]=='~'){
            // case where ~ is give as a part of some path
            char temp_buf[MAX_COM];
            strcpy(temp_buf,home_dir);
            int pos=strlen(temp_buf);
            for(int i=1;i<strlen(args[0]);i++){
                temp_buf[pos++] = args[0][i];
            }
            temp_buf[pos]='\0';
            strcpy(args[0],temp_buf);
        }
        

        if(execvp(args[0],args)<0){
            prompt_error_write("Command not found");
            new_line_error();
            success=1;
            exit(1);        
        }
        
        exit(1);

    }
    else{
        
        strcpy(foreground.p_name,token);
        foreground.pid_no=pid;

        int status;
        waitpid(pid,&status,WUNTRACED);
        foreground.pid_no=-1;
        gettimeofday(&tv,NULL);
        end = (tv.tv_sec)*1000000 + tv.tv_usec;
        if(((double)(end-start))/1000000>1){
            disp_with_time = 1;
        }
        foreground.pid_no=-1;
    }
}