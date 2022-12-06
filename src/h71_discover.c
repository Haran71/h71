#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"


void find_specific(char * path){
    struct dirent ** file_list;
    char temp_path[MAX_COM/100]; // absolute path of the element
    char out_string[MAX_COM/100];

    int count = scandir(path,&file_list,NULL,alphasort);
    for(int j=0;j<count;j++){
        if(strcmp(file_list[j]->d_name,".")==0){
            continue;
        }
        if(strcmp(file_list[j]->d_name,"..")==0){
            continue;
        }
        sprintf(temp_path,"%s/%s",path,file_list[j]->d_name);
        struct stat sb;
        stat(temp_path,&sb);
        if(S_ISDIR(sb.st_mode)){
            find_specific(temp_path);
        }
        else{
            if(strcmp(target_file,file_list[j]->d_name)==0){
                strcpy(out_string,token_god);
                int pos = strlen(out_string);
                int len= strlen(root_dir_path);
                for(int i=len;i<strlen(temp_path);i++){
                    out_string[pos++] = temp_path[i];
                }
                out_string[pos] = '\0';
                strcpy(final_path,out_string);
                prompt_write(final_path);
                new_line();
            }
        }
    }
    free(file_list);
}

void print_all(char * path){
    struct dirent ** file_list;
    char temp_path[MAX_COM/100]; // absolute path of the element
    char out_string[MAX_COM/100]; // thing needed to get printed on the screen

    strcpy(out_string,token_god);
    int pos = strlen(out_string);
    int len= strlen(root_dir_path);
    for(int i=len;i<strlen(path);i++){
        out_string[pos++] = path[i];
    }
    out_string[pos] = '\0';
    if(flag_d){
        prompt_write(out_string);
        new_line();
    }    

    int count = scandir(path,&file_list,NULL,alphasort);
    for(int j=0;j<count;j++){
        if(strcmp(file_list[j]->d_name,".")==0){
            continue;
        }
        if(strcmp(file_list[j]->d_name,"..")==0){
            continue;
        }
        sprintf(temp_path,"%s/%s",path,file_list[j]->d_name);
        struct stat sb;
        stat(temp_path,&sb);
        if(S_ISDIR(sb.st_mode)){
            print_all(temp_path);
        }
        else{
            strcpy(out_string,token_god);
            int pos = strlen(out_string);
            int len= strlen(root_dir_path);
            for(int i=len;i<strlen(temp_path);i++){
                out_string[pos++] = temp_path[i];
            }
            out_string[pos] = '\0';
            if(flag_f){
                prompt_write(out_string);
                new_line();
            }    
        }
    }
    free(file_list);
}


void h71_discover(){
    flag_d = 0;
    flag_f =0;
    found =0;
    int dir_count =0;
    int file_check =0;
    int flag_check = 0;
    int dir_check =0;
    char temp[MAX_COM/100];
    char * token;
    strcpy(temp,cmd_w);

    token = strtok(temp, " ");
    //This loop tets us all the flags in the command
    while(1){
        token = strtok(NULL, " ");
        if(token==NULL){
            break;
        }
        if(token[0] == '-'){
            for(int i=1;i<strlen(token);i++){
                if(token[i]=='d'){
                    flag_d = 1;
                }
                else if(token[i]=='f'){
                    flag_f = 1;
                }
                else{
                    flag_check = 1;
                    break;
                }
            }   
        }
    }
    if(flag_check){
        prompt_error_write("discover: Invalid option");
        new_line_error();
        success=1;
        return;
    }
    if(!flag_d && !flag_f){
        flag_d = 1;
        flag_f = 1;
    }
    // the next loop is to check for directories and file_names
    strcpy(temp,cmd_w);

    token = strtok(temp, " ");
    //This loop tets us all the flags in the command
    while(1){
        token = strtok(NULL, " ");
        if(token==NULL){
            break;
        }
        if(token[0] == '-'){
            if(strlen(token)!=1) continue;
        }
        if(token[0] == '"'){
            if(strlen(token) <=2) continue;
            file_check = 1;
            int pos =0;
            for(int i=1;i<strlen(token)-1;i++){
                target_file[pos++] = token[i];
            }
            target_file[pos] = '\0';
            continue;
        }
        dir_count++;
        strcpy(cd_flag,token);
        do_cd();
        struct stat sb;
        if(stat(cd_target,&sb)!=0){
            dir_check=1;
            break;
        }
        if(!S_ISDIR(sb.st_mode)){
            dir_check=2;
            break;
        }
    }
    if(dir_check==1){
        prompt_error_write("discover: Invalid target path given");
        new_line_error();
        success=1;
        return;
    }
    if(dir_check==2){
        prompt_error_write("discover: Specified target path is not a directory");
        new_line_error();
        success=1;
        return;
    }
    if(dir_count==0){
        sprintf(cmd_w,"%s .",cmd_w);
    }
    strcpy(temp,cmd_w);
    token = strtok(temp," "); 
    while(1){
        token = strtok(NULL," ");
        if(token==NULL){
            break;
        }
        if(token[0] == '-'){
            if(strlen(token)!=1) continue;
        }
        if(token[0] == '"'){
            continue;
        }
        strcpy(cd_flag,token);
        do_cd();
        realpath(cd_target,root_dir_path); // absolute path of the directory
        strcpy(token_god,token);
        if(!file_check){
            print_all(root_dir_path);
        }
        else{
            find_specific(root_dir_path);
        }    
    }
    
}