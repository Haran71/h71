#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void h71_pwd(){
    if(strlen(cmd_w)>3){
        write(2,"pwd: too many arguments",strlen("pwd: too many arguments"));
        new_line_error();
        success=1;
        return;
    }
    char curr_dir[MAX_COM];
    getcwd(curr_dir,sizeof(curr_dir));
    prompt_write(curr_dir);
    new_line();
}