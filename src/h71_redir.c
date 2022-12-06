#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void h71_redir(){
    if(cmd_w[0]=='\0') return;

    int flag_1 =0; // for <
    int flag_2 =0; // for >
    int flag_3 =0; // for >>
    int flag_err =0; // for error

    char *token;
    char out_file[MAX_COM/100];
    char in_file[MAX_COM/100];
    char temp_file[MAX_COM];

    strcpy(temp_file,cmd_w);
    token = strtok(temp_file, " ");

    while(token!=NULL){
        if(!strcmp(token,"<")){
            flag_1 = 1;
            token = strtok(NULL, " ");
            if(token==NULL){
                flag_err = 1;
                break;
            }
            strcpy(in_file,token);
            FILE * read_p = fopen(in_file,"r");
            if(read_p==NULL){
                flag_err = 1;
                prompt_error_write("redir: Input file not found");
                new_line_error();
                success=1;
                cmd_w[0]='\0';
                //fclose(read_p);
                return;
            }
            fclose(read_p);
        }
        if(!strcmp(token,">")){
            flag_2 =1;
            token = strtok(NULL, " ");
            if(token==NULL){
                flag_err = 1;
                break;
            }
            strcpy(out_file,token);
            int w_fd = open(out_file, O_WRONLY | O_TRUNC | O_CREAT,0644);
            if(w_fd<1){
                flag_err = 1;
                prompt_error_write("redir: unable to open write file");
                new_line_error();
                success=1;
                cmd_w[0]='\0';
                close(w_fd);
                return;
            }
            close(w_fd);
            
        }
        if(!strcmp(token,">>")){
            // This command allows you to append to existing files
            flag_3=1;
            token = strtok(NULL, " ");
            if(token==NULL){
                flag_err = 1;
                break;
            }
            strcpy(out_file,token);
            int w_fd = open(out_file, O_WRONLY | O_APPEND | O_CREAT,0644);
            if(w_fd<1){
                flag_err = 1;
                prompt_error_write("redir: unable to open write file");
                new_line_error();
                success=1;
                cmd_w[0]='\0';
                close(w_fd);
                return;
            }
            close(w_fd); 
        }
        if(flag_2 && flag_3){
            flag_err = 1;
            break;
        }
        token = strtok(NULL," ");

    }
    if(flag_err){
        prompt_error_write("redir: Redirection error");
        new_line_error();
        success=1;
        cmd_w[0]='\0';
        return;
    }
    if(!flag_1 && !flag_2 && !flag_3){
        return;
    }
    
    pid_t pid = fork();

    if(pid <0){
        prompt_error_write("Process creation failed");
        new_line_error();
        success=1;
        return;
    }
    else if(pid==0){
        if(flag_1){
            int read_fd = open(in_file,O_RDONLY);
            dup2(read_fd,STDIN_FILENO);
            close(read_fd);
        }
        if(flag_2){
            int write_fd = open(out_file, O_WRONLY | O_TRUNC | O_CREAT,0644);
            dup2(write_fd,STDOUT_FILENO);
            close(write_fd);
        }
        if(flag_3){
            int write_fd = open(out_file, O_WRONLY | O_APPEND | O_CREAT,0644);
            dup2(write_fd,STDOUT_FILENO);
            close(write_fd);
        }
        for(int i=0;i<strlen(cmd_w);i++){
            if(cmd_w[i] == '<' || cmd_w[i] == '>'){
				cmd_w[i] = '\0';
            }
        }
        command_parse();
        exit(0);
    }
    else{
        int status;
        waitpid(pid,&status,WUNTRACED);
        cmd_w[0]='\0';
        return;
    }
}