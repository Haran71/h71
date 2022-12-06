#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void h71_cd(){
    if(strlen(cmd_w) ==2){
        getcwd(prev_dir,sizeof(prev_dir));
        chdir(home_dir);
        disp_get();
        return;
    }
    int pos =0;
    for(int i=3;cmd_w[i]!='\0';i++){
        if(cmd_w[i] == ' '){
            write(2,"cd: too many arguments",strlen("cd: too many arguments"));
            new_line_error();
            success=1;
            return;
        }
        cd_flag[pos++] = cmd_w[i];
    }
    cd_flag[pos] = '\0';
    do_cd();
    struct stat st;
    if(stat(cd_target,&st)!=0){
        write(2,"cd: Path specified does not exist",strlen("cd: Path specified does not exist"));
        new_line_error();
        success=1;
        return;
    }
    if(!S_ISDIR(st.st_mode)){
        write(2,"cd: Path specified is not a directory",strlen("cd: Path specified is not a directory"));
        new_line_error();
        success=1;
        return;
    }
    getcwd(prev_dir,sizeof(prev_dir));
    chdir(cd_target);
    if(!strcmp(cd_flag,"-")){
        char temp[MAX_COM];
        getcwd(temp,sizeof(temp));
        prompt_write(temp);
        new_line();
    }
    disp_get();
    return;
    
}