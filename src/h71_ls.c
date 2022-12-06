#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"


void h71_ls(){
    char temp[MAX_COM/100];
    char file_name[MAX_COM];
    char curr_dir[MAX_COM]; // storing the command 
    char format_size[4] = {0}; // in order to ensure uniformity in printng 
    struct dirent ** file_list;
    int alloc_check =0;
    

    char temp_buf[MAX_COM];
    strcpy(temp_buf,cmd_w);
    char * token;
    token = strtok(temp_buf," ");
    int flag_l = 0;
    int flag_a = 0;
    int flag_check =0; // check for undefined flag
    while(1){
        token = strtok(NULL," ");
        if(token==NULL){
            break;
        }
        if(token[0]=='-'){
            for(int i=1;i<strlen(token);i++){
                if(token[i]=='l'){
                    flag_l =1;
                }
                else if(token[i]=='a'){
                    flag_a = 1;
                }
                else{
                    flag_check = 1;
                    break;
                }
            }
        }    
    }
    if(flag_check){
        prompt_error_write("ls:Invalid option");
        new_line_error();
        success=1;
        return;
    }
    int dir_count = 0; // used to count the number of directories in command input
    int file_count =0; // used to count the number of files.
    strcpy(temp_buf,cmd_w);
    int dir_check=0; // to check that the given directory is valid
    token = strtok(temp_buf," ");
    while(1){
        token = strtok(NULL," ");
        if(token==NULL){
            break;
        }
        if(token[0]=='-'){
            if(strlen(token)!=1){
                continue;
            }
            dir_check=1;
            break;
        }
        dir_count++;
        strcpy(cd_flag,token);
        do_cd();
        struct stat st;
        if(stat(cd_target,&st)!=0){
            dir_check=1;
            break;
        }
        if(S_ISREG(st.st_mode)){
            file_count++;
        }
        
        
    }
    if(dir_check){
        prompt_error_write("ls: No such file or directory");
        new_line_error();
        success=1;
        return;
    }
    if(dir_count==0){
        sprintf(cmd_w,"%s .",cmd_w); // to add the current directory as an argument
    }

    int temp_fc = file_count;
    strcpy(temp_buf,cmd_w);
    token = strtok(temp_buf," ");

    // first loop for files in order to format the output
    while(file_count>0){
        // printf("Here\n");
        token = strtok(NULL," ");
        if(token==NULL) break;
        if(token[0]=='-'){
            if(strlen(token)!=1){
                continue;
            }
        }
        strcpy(cd_flag,token);
        do_cd();
        struct stat sb;
        stat(cd_target,&sb);
        if(S_ISREG(sb.st_mode)){
            
            if(flag_l){

                sprintf(temp,"%hu",sb.st_nlink);
                format_size[0] = strlen(temp) > format_size[0] ? strlen(temp):format_size[0];
                

                struct passwd * pwd = getpwuid(sb.st_uid);
                sprintf(temp,"%s",pwd->pw_name);
                format_size[1] = strlen(temp) > format_size[1] ? strlen(temp):format_size[1];

                struct group * gre = getgrgid(sb.st_gid);
                sprintf(temp,"%s",gre->gr_name);
                format_size[2] = strlen(temp) > format_size[2] ? strlen(temp):format_size[2];

                sprintf(temp,"%lld",sb.st_size);
                format_size[3] = strlen(temp) > format_size[3] ? strlen(temp):format_size[3];

            }
            file_count--;
            continue;

        }
    }

    // one iteration to get all the  files as they have to be printed first.
    file_count = temp_fc;
    strcpy(temp_buf,cmd_w);
    token = strtok(temp_buf," ");
    while(file_count>0){
        // printf("Here\n");
        token = strtok(NULL," ");
        if(token==NULL) break;
        if(token[0]=='-'){
            if(strlen(token)!=1){
                continue;
            }
        }
        strcpy(cd_flag,token);
        do_cd();
        struct stat sb;
        stat(cd_target,&sb);
        if(S_ISREG(sb.st_mode)){
            
            if(flag_l){

                get_perm(cd_target);
                prompt_write(permissions);
                insert_space();

                sprintf(temp,"%hu",sb.st_nlink);
                for(int i=0;i<format_size[0]-strlen(temp);i++){
                    insert_space();
                }
                prompt_write(temp);
                insert_space();

                struct passwd * pwd = getpwuid(sb.st_uid);
                sprintf(temp,"%s",pwd->pw_name);
                for(int i=0;i<format_size[1]-strlen(temp);i++){
                    insert_space();
                }
                prompt_write(temp);
                insert_space();

                struct group * gre = getgrgid(sb.st_gid);
                sprintf(temp,"%s",gre->gr_name);
                for(int i=0;i<format_size[2]-strlen(temp);i++){
                    insert_space();
                }
                prompt_write(temp);
                insert_space();

                sprintf(temp,"%lld",sb.st_size);
                for(int i=0;i<format_size[3]-strlen(temp);i++){
                    insert_space();
                }
                prompt_write(temp);
                insert_space();

                strftime(temp, 10, "%m", localtime(&sb.st_mtime));
				disp_month(temp);
				strftime(temp, 50, " %d %H:%M", localtime(&sb.st_mtime));
				prompt_write(temp);
				write(1, " " , 1);

            }
            char colors[MAX_COM];
            if(S_IEXEC & sb.st_mode){
                sprintf(colors,"%s%s%s",KGRN,token,KNRM);
            } 
            else{
                strcpy(colors,token);
            }
            prompt_write(colors);
            new_line();
            file_count--;
            continue;

        }
    }
    for(int i=0;i<4;i++){
        format_size[i] = 0;
    }
    strcpy(temp_buf,cmd_w);
    token = strtok(temp_buf," ");
    // final interation through the cmd_w string to display the ls output
    while(1){
        token = strtok(NULL, " ");
        if(token==NULL){
            break;
        }  
        if(token[0]=='-'){
            if(strlen(token)!=1){
                continue;
            }
        }
        strcpy(cd_flag,token);
        do_cd();
        struct stat sb;
        stat(cd_target,&sb);
        if(S_ISREG(sb.st_mode)){
            continue;
        }
        int count = scandir(cd_target,&file_list,NULL,alphasort);
        alloc_check = 1;

        if(dir_count>1){
            char  disp_temp[MAX_COM/100];
            int pos =0;
            int i;
            for(i=strlen(cd_target)-1;i>=0;i--){
                if(cd_target[i]=='/'){
                    break;
                }
            }
            i++;
            for(;i<strlen(cd_target);i++){
                disp_temp[pos++] = cd_target[i];
            }
            disp_temp[pos] = '\0';
            prompt_write(disp_temp);
            prompt_write(":");
            new_line();
        }
        long long int total=0; 
        for(int i=0;i<count;i++){
            if(!flag_a && file_list[i]->d_name[0] =='.'){
                continue;
            }
            if(flag_l){
                sprintf(file_name,"%s/%s",cd_target,file_list[i]->d_name);

                struct stat st_s;
                lstat(file_name,&st_s);
                total += st_s.st_blocks;
                sprintf(temp,"%hu",st_s.st_nlink);
                format_size[0] = strlen(temp) > format_size[0] ? strlen(temp):format_size[0];

                struct passwd * pwd = getpwuid(st_s.st_uid);
                sprintf(temp,"%s",pwd->pw_name);
                format_size[1] = strlen(temp) > format_size[1] ? strlen(temp):format_size[1];

                struct group * gre = getgrgid(st_s.st_gid);
                sprintf(temp,"%s",gre->gr_name);
                format_size[2] = strlen(temp) > format_size[2] ? strlen(temp):format_size[2];

                sprintf(temp,"%lld",st_s.st_size);
                format_size[3] = strlen(temp) > format_size[3] ? strlen(temp):format_size[3];
            }
        }

        sprintf(temp,"total %lld",total/2);
        if(flag_l){
            prompt_write(temp);
            new_line();
        }

        for(int i=0;i<count;i++){
            if(!flag_a && file_list[i]->d_name[0] =='.'){
                continue;
            }
            sprintf(file_name,"%s/%s",cd_target,file_list[i]->d_name);
            if(flag_l){
                
                get_perm(file_name);
                prompt_write(permissions);
                insert_space();

                struct stat st_s;
                lstat(file_name,&st_s);
                sprintf(temp,"%hu",st_s.st_nlink);
                for(int i=0;i<format_size[0]-strlen(temp);i++){
                    insert_space();
                }
                prompt_write(temp);
                insert_space();
            
                struct passwd * pwd = getpwuid(st_s.st_uid);
                sprintf(temp,"%s",pwd->pw_name);
                for(int i=0;i<format_size[1]-strlen(temp);i++){
                    insert_space();
                }
                prompt_write(temp);
                insert_space();

                struct group * gre = getgrgid(st_s.st_gid);
                sprintf(temp,"%s",gre->gr_name);
                for(int i=0;i<format_size[2]-strlen(temp);i++){
                    insert_space();
                }
                prompt_write(temp);
                insert_space();

                sprintf(temp,"%lld",st_s.st_size);
                for(int i=0;i<format_size[3]-strlen(temp);i++){
                    insert_space();
                }
                prompt_write(temp);
                insert_space();

                strftime(temp, 10, "%m", localtime(&st_s.st_mtime));
				disp_month(temp);
				strftime(temp, 50, " %d %H:%M", localtime(&st_s.st_mtime));
				prompt_write(temp);
				insert_space();
            }
            struct stat st_c;
            lstat(file_name,&st_c);
            if(S_ISDIR(st_c.st_mode)){
                sprintf(colors,"%s%s%s",KBLU,file_list[i]->d_name,KNRM);
            }   
            else if(S_IEXEC & st_c.st_mode){
                sprintf(colors,"%s%s%s",KGRN,file_list[i]->d_name,KNRM);
            } 
            else{
                strcpy(colors,file_list[i]->d_name);
            }
            prompt_write(colors);
            new_line();
        }
        if(dir_count>1) new_line();
        
    } 
    if(alloc_check){
        free(file_list);
    }
      
}
