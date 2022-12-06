#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void h71_echo(){
    char * echo_buffer;
    echo_buffer = (char*)malloc(MAX_COM*sizeof(MAX_COM));
    int pos=0;
    for(int i=4;cmd_w[i]!='\0';i++){
        echo_buffer[pos++] = cmd_w[i];
    }
    echo_buffer[pos] = '\0';
    clean_command(echo_buffer);
    prompt_write(echo_buffer);
    new_line();
    free(echo_buffer);
    return;
}