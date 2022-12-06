#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void h71_pinfo(){

    pid_t proc_id;
    char proc_path[MAX_COM/100];
    char pinfo_out[MAX_COM];
    pid_t curr_group = tcgetpgrp(STDOUT_FILENO);
    int plus_flag =0;

    if(strlen(cmd_w)==5){
        proc_id = getpid();
    }
    else {
        int pos=0;
        for(int i=6;i<strlen(cmd_w);i++){
            proc_path[pos++] = cmd_w[i];
        }
        proc_path[pos] = '\0';
        proc_id = atoi(proc_path);
    }
    if(curr_group==getpgid(proc_id)){ // to check if it's a foreground process
        plus_flag=1;
    }

    sprintf(proc_path,"/proc/%d",proc_id);

    
    struct stat sb;
    if(lstat(proc_path,&sb)!=0){
        prompt_error_write("pinfo: Process ID doesn't exist");
        new_line_error();
        success=1;
        return;
    }

    // getting the executable path
    char execute_path[MAX_COM/100];
    char executable[MAX_COM/100];
    sprintf(execute_path,"/proc/%d/exe",proc_id);

    readlink(execute_path,executable,100);

    if(strlen(home_dir)<strlen(executable)){
        int flag =0; // flag to check if the given path is a sub-directory
        for(int i=0;i<strlen(home_dir);i++){
            if(home_dir[i]!=executable[i]){
                flag=1;
                break;
            }
        }
        if(!flag){
            executable[0] = '~';
            executable[1] = '/';
            int pos =2;
            for(int i=strlen(home_dir)+1;i<strlen(executable);i++){
                executable[pos++] = executable[i];
            }
            executable[pos] = '\0';
        } 
    }
    sprintf(proc_path, "/proc/%d/stat",proc_id);
	FILE *proc_stat = fopen(proc_path, "r");

    long long virtual_mem_size;
    long long placeholder;

    char proc_status;
    fscanf(proc_stat,"%d",&proc_id);
    fscanf(proc_stat,"%c",&proc_status);
    while(proc_status!=')') fscanf(proc_stat,"%c",&proc_status);
    while(proc_status==')') fscanf(proc_stat,"%c",&proc_status);

    fscanf(proc_stat,"%c",&proc_status);
    int i=0;
    while(i<19){
        fscanf(proc_stat,"%lld",&placeholder);
        i++;
    }
    fscanf(proc_stat,"%lld",&virtual_mem_size);
    if(!plus_flag){
        sprintf(pinfo_out, "PID -- %d\nProcess Status -- %c\nMemory -- %lld \nExecutable Path -- %s", proc_id, proc_status, virtual_mem_size, executable);
    }
    else{
        sprintf(pinfo_out, "PID -- %d\nProcess Status -- %c+\nMemory -- %lld \nExecutable Path -- %s", proc_id, proc_status, virtual_mem_size, executable);
    }    
    prompt_write(pinfo_out);
    new_line();
    fclose(proc_stat);

}