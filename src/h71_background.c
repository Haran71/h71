#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void h71_background(char * token){

    if(num_children==MAX_BG){
        prompt_error_write("&: the number of background processes is already maximum");
        new_line_error();
        return;
    }
    int pid = fork();

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
        setpgid(0,0); // to prevent it from accessing the terminal we change the group of the process
        if(execvp(args[0],args)<0){
            prompt_error_write("Command not found");
            new_line_error();
            exit(1);
        }           
        exit(1);

    }
    else{
        char temp_buf[10];
        add_child(pid,token);
        sprintf(temp_buf,"[%d] %d",num_children,pid);
        prompt_write(temp_buf);
        new_line();
        return;
    }
}