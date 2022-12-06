#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void h71_process(){
    char * token;

    char temp[MAX_COM];
    char temp_token[MAX_COM];
    int amp_last = 0;
    if(cmd_w[strlen(cmd_w)-1]=='&'){
        amp_last = 1;
    }
    strcpy(temp,cmd_w);
    token = strtok(temp,"&");
    
    while(1){
        strcpy(temp_token,token);
        token = strtok(NULL,"&");
        if(token==NULL){
            break;
        }
        // any command that comes here is a background process. No fucking clue how to deal with them yet.
        clean_command(temp_token);
        h71_background(temp_token);
    }
    // anything that comes here is a foreground process.
    clean_command(temp_token);
    if(command_check(temp_token)){
        strcpy(cmd_w,temp_token);
        command_parse();
    }
    else{
        if(!amp_last){
            h71_foreground(temp_token);
        }
        else{
            clean_command(temp_token);
            h71_background(temp_token);
        }    
    }


}