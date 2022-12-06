#include"../include/header.h"
#include "../include/functions.h"
#include"../include/global.h"


void prompt_write(char * buffer){
    write(1,buffer,strlen(buffer));
}

void prompt_error_write(char * buffer){
    write(2,buffer,strlen(buffer));
}

void new_line(){
    write(1,"\n",strlen("\n"));
}
void new_line_error(){
    write(2,"\n",strlen("\n"));
}

void insert_space(){
    write(1, " ", 1);
}
void set_dir(){
    getcwd(home_dir,sizeof(home_dir));
    getcwd(prev_dir,sizeof(prev_dir));
}
void disp_get(){
    struct utsname buf;
    struct passwd* pwd;
    int info = uname(&buf);
    pwd = getpwuid(getuid());


    char dir_prompt[MAX_COM];
    char curr_dir[MAX_COM];

    getcwd(curr_dir,MAX_COM);
    if(strcmp(curr_dir,home_dir)==0){
        strcpy(dir_prompt,"~");
    }
    else if(strlen(curr_dir) > strlen(home_dir)){
        int flag =0; // flag to check if the given path is a sub-directory
        for(int i=0;i<strlen(home_dir);i++){
            if(home_dir[i]!=curr_dir[i]){
                flag=1;
                break;
            }
        }
        if(flag){
            // it is not a sub-directory, then we display as is
            strcpy(dir_prompt,curr_dir);
        }
        else{
            dir_prompt[0] = '~';
            dir_prompt[1] = '/';
            int pos =2;
            for(int i=strlen(home_dir)+1;i<strlen(curr_dir);i++){
                dir_prompt[pos++] = curr_dir[i];
            }
            dir_prompt[pos] = '\0';
        }
        
    }
    else if(strlen(curr_dir) < strlen(home_dir)){
        int pos=0,i=0;
        for(; i<strlen(curr_dir);i++){
            dir_prompt[pos++] = curr_dir[i];
        }
        dir_prompt[pos] = '\0';
    }


    //char BUF[MAX_COM];
    if(!disp_with_time){
        sprintf(PROMPT_BUF,"\r%d <%s@%s: %s>",success,pwd->pw_name,buf.nodename,dir_prompt);
        write(1,PROMPT_BUF,strlen(PROMPT_BUF));
    }
    else{
        disp_with_time=0;
        char time_statement[50];
        sprintf(time_statement,"took %.2fs",((double)(end-start))/1000000);
        sprintf(PROMPT_BUF,"\r%d <%s@%s: %s%s>",success,pwd->pw_name,buf.nodename,dir_prompt,time_statement);
        write(1,PROMPT_BUF,strlen(PROMPT_BUF));
    } 
    if(success){
        success=0;
    }   
    
}



void clean_command(char * buffer){
    int count = 0;
    for(int i =0;i<strlen(buffer);i++){
        if(isspace(buffer[i]) && (i==0 || isspace(buffer[i-1]))){
            continue;
        }
        else if(isspace(buffer[i])){
            buffer[count++] = ' ';
        }
        else{
            buffer[count++] = buffer[i];
        }
    }
    buffer[count] = '\0';
    count--;
    while(count>=0 && (isspace(buffer[count]) || buffer[count]=='\t')){
        count--;
    }
    count++;
    buffer[count] = '\0';
}

void command_parse(){
    if(cmd_w[0]=='\0'){
        return;
    }
    get_command();
    if(!strcmp(command,"q") || !strcmp(command,"quit") || !strcmp(command,"exit")){
        exit_flag = 1;
        return;
    }
    
    if(!strcmp(command,"pwd")){
        h71_pwd();
    }
    else if(!strcmp(command,"cd")){
        h71_cd();
    }
    else if(!strcmp(command,"echo")){
        h71_echo();
    }
    else if(!strcmp(command,"ls")){
        h71_ls();
    }
    else if(!strcmp(command,"discover")){
        h71_discover();
    }
    else if(!strcmp(command,"pinfo")){
        h71_pinfo();
    }
    else if(!strcmp(command,"history")){
        history_read();
    }
    else if(!strcmp(command,"fg")){
        h71_fg();
    }
    else if(!strcmp(command,"bg")){
        h71_bg();
    }
    else if(!strcmp(command,"sig")){
        h71_sig();
    }
    else if(!strcmp(command,"jobs")){
        h71_jobs();
    }
    else{
        h71_process();
    }
    
}

int command_check(char * token){
    get_command_token(token);
    int ret =0;
    if(!strcmp(command,"q") || !strcmp(command,"quit") || !strcmp(command,"exit")){
        ret = 1;
    }
    
    if(!strcmp(command,"pwd")){
        ret = 1;
    }
    else if(!strcmp(command,"cd")){
        ret = 1;
    }
    else if(!strcmp(command,"echo")){
        ret = 1;
    }
    else if(!strcmp(command,"ls")){
        ret = 1;
    }
    else if(!strcmp(command,"discover")){
        ret = 1;
    }
    else if(!strcmp(command,"pinfo")){
        ret = 1;
    }
    else if(!strcmp(command,"history")){
        ret = 1;
    }
    else if(!strcmp(command,"fg")){
        ret = 1;
    }
    else if(!strcmp(command,"bg")){
        ret = 1;
    }
    else if(!strcmp(command,"sig")){
        ret = 1;
    }
    else if(!strcmp(command,"jobs")){
        ret = 1;
    }
    return ret;

}

void get_command(){
    // to get the command string without args
    int pos=0;
    int i=0;
    for(int i = 0; cmd_w[i]!=' ' && cmd_w[i]!='\0' && cmd_w[i]!='\n'; i++)
		command[pos++] = cmd_w[i];
	command[pos] = '\0';
}

void get_command_token(char * token){
    int pos=0;
    int i=0;
    for(int i = 0; token[i]!=' ' && token[i]!='\0' && token[i]!='\n'; i++)
		command[pos++] = token[i];
	command[pos] = '\0';
}

void do_cd(){
    // cd_flag string contains what we need to work with
    // cd_target is the ouput string
    struct passwd* pwd;
    pwd = getpwuid(getuid());
    if(!strcmp(cd_flag,".")){
        getcwd(cd_target,sizeof(cd_target));
        return;
    }
    if(!strcmp(cd_flag,"~") || cd_flag[0]=='\0'){
        strcpy(cd_target,home_dir);
        return;
    }
    if(cd_flag[0]=='~'){
        strcpy(cd_target,home_dir);
        int pos = strlen(cd_target);
        for(int i=1;cd_flag[i]!='\0';i++){
            cd_target[pos++] = cd_flag[i];
        }
        cd_target[pos] = '\0';
        return;
    }
    if(!strcmp(cd_flag,"..")){
        char curr_dir[MAX_COM];
        getcwd(curr_dir,sizeof(curr_dir));
        int i=strlen(curr_dir) -1;
        for(;i>=0;i--){
            if(curr_dir[i]=='/'){
                break;
            }
        }
        cd_target[i]='\0';
        for(int j=0;j<i;j++){
            cd_target[j] = curr_dir[j];
        }
        return;
    }
    if(!strcmp(cd_flag,"-")){
        strcpy(cd_target,prev_dir);
        return;
    }

    strcpy(cd_target,cd_flag);

}
void get_perm(char * file_path){
    struct stat st;
    int stat_v = stat(file_path,&st);

    permissions[0] ='-';
    if(S_ISDIR(st.st_mode)) permissions[0] = 'd';
    else if(S_ISLNK(st.st_mode)) permissions[0] = 'l';
    else if(S_ISCHR(st.st_mode)) permissions[0] = 'c';
    else if(S_ISSOCK(st.st_mode)) permissions[0] = 's';
    else if(S_ISBLK(st.st_mode)) permissions[0] = 'b';    	
    else if(S_ISFIFO(st.st_mode)) permissions[0] = 'p';

    permissions[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';
        	

}

void disp_month(char *month)
{
	if(!strcmp(month, "01"))
		write(1, "Jan", 3);
	else if(!strcmp(month, "02"))
		write(1, "Feb", 3);
	else if(!strcmp(month, "03"))
		write(1, "Mar", 3);
	else if(!strcmp(month, "04"))
		write(1, "Apr", 3);
	else if(!strcmp(month, "05"))
		write(1, "May", 3);
	else if(!strcmp(month, "06"))
		write(1, "Jun", 3);
	else if(!strcmp(month, "07"))
		write(1, "Jul", 3);
	else if(!strcmp(month, "08"))
		write(1, "Aug", 3);
	else if(!strcmp(month, "09"))
		write(1, "Sep", 3);
	else if(!strcmp(month, "10"))
		write(1, "Oct", 3);
	else if(!strcmp(month, "11"))
		write(1, "Nov", 3);
	else if(!strcmp(month, "12"))
		write(1, "Dec", 3);
}


