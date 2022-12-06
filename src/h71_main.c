#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void h71_main(){
    // updating command line prompt  
    disp_get();

    CMD[0] = '\0';

    take_inp(); 

    if(CMD[0]=='\0'){
        exit_flag=1;
        return;
    }

    // Now we have to deal with white spaces

    clean_command(CMD);

    // now let's parse on semicolon.
    
    int pos = 0,kill_w=0;
    
    while(1){
        cmd_w[0] = '\0';
        if(kill_w){
            break;
        }
        if(strlen(CMD) == 0){
            return;
        }
        while(CMD[pos]==';'){
            pos++;
        }
        int i=0;
        while(1){
            if(CMD[pos]==';'){
                break;
            }
            cmd_w[i++] = CMD[pos++];
            if(pos>=strlen(CMD)){
                kill_w = 1;
                break;
            }
        }
        cmd_w[i] = '\0'; 
        clean_command(cmd_w);
        history_write();
        h71_pipe();
        h71_redir();
        command_parse();
    }
}