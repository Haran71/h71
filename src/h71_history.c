#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void history_read(){
    char * home;
    home = getenv("HOME");
    if(strlen(cmd_w)>7){
        prompt_error_write("history: too many arguments");
        new_line_error();
        return;
    }
    char * his_buffer[15];
    int his_size =0;
    for(int i=0;i<15;i++){
        his_buffer[i] = (char*)malloc(MAX_COM*sizeof(char));
    }

    int command_count = HIST_READ;
    char read_path[30];
    char read_final[MAX_COM];

    

    strcpy(read_path,".h71_history.txt");
    sprintf(read_final,"%s/%s",home,read_path);

    FILE * read_p = fopen(read_final,"r");
    if(read_p==NULL){
        prompt_error_write("history: couldn't read file");
        new_line_error();
        success=1;
        return;
    }

    char * line = NULL;
    size_t len =0;
    while(getline(&line,&len,read_p)!=-1 && command_count>=0){
        if(*line == '\n' || strlen(line)==0){
            continue;
        }
        strcpy(his_buffer[his_size++],line);
        command_count--;
    }
    for(int i=his_size-1;i>=0;i--){
        prompt_write(his_buffer[i]);
    }

    for(int i=0;i<15;i++){
        free(his_buffer[i]);
    }
    fclose(read_p);
    return;
}


void history_write(){
    char * home;
    home = getenv("HOME");
    char read_path[30];
    char write_path[30];
    char read_final[MAX_COM];
    char write_final[MAX_COM];
    strcpy(read_path,".h71_history.txt");
    strcpy(write_path,"temp_history.txt"); // temporary file we make
    
    sprintf(read_final,"%s/%s",home,read_path);
    sprintf(write_final,"%s/%s",home,write_path);
    
    char * line = NULL;
    size_t len =0;
    
    FILE * write_p = fopen(write_final,"w");
    FILE * read_p = fopen(read_final,"r");
    
    if(read_p==NULL || write_p ==NULL){
        prompt_error_write("history: couldn't write to/read file");
        new_line_error();
        success=1;
        return;
    }
    int command_count;
    
    if(getline(&line,&len,read_p)!=-1){
        clean_command(line);
        if(!strcmp(line,cmd_w)){
            command_count =0;
        }
        else{
            command_count = 1;
            fputs(cmd_w,write_p);
            fputs("\n",write_p);
        }
    } 
    else {  
        command_count = 1;
        fputs(cmd_w,write_p);
        fputs("\n",write_p);
    }
    line = NULL;
    fclose(read_p);
    read_p = fopen(read_final,"r");
   
    while(getline(&line,&len,read_p)!=-1 && command_count<HIST_COUNT){
        if(*line == '\n' || strlen(line)==0){
            continue;
        }
        fputs(line,write_p);
        //fputs("\n",write_p);
        command_count++;
    }

    fclose(read_p);
    fclose(write_p);

    remove(read_final);
    rename(write_final,read_final);
}

void history_init(){
    char * home;
    home = getenv("HOME");
    char read_path[30];
    char read_final[MAX_COM];
    strcpy(read_path,".h71_history.txt");
    sprintf(read_final,"%s/%s",home,read_path);
    FILE * read_p = fopen(read_final,"a+");
    fclose(read_p);
}