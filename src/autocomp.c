#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

char auto_curr_dir[MAX_COM];

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &auto_term); 
    
    atexit(disableRawMode);
    struct termios raw = auto_term;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); 
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &auto_term);
}

int handle_word_list(int word_count,char * word_list[]){
    int pointer[word_count];
    for(int i=0;i<word_count;i++) pointer[i]=0;
    int flag_word =0;
    while(1){
        for(int i=0;i<word_count;i++){
            if(pointer[i]==strlen(word_list[i])){
                flag_word=1;
                break;
            }
        }
        if(flag_word) break;
        for(int i=0;i<word_count;i++){
            if(word_list[0][pointer[0]]!=word_list[i][pointer[0]]){
                flag_word=1;
                break;
            }
        }
        if(flag_word) break;

        for(int i=0;i<word_count;i++) pointer[i]++;
    }
    return pointer[0];
    
}

void handle_auto(){
    char * cmp_word[MAX_COM];
    for(int i=0;i<MAX_COM;i++){
        cmp_word[i] =(char *)malloc(MAX_COM*sizeof(char));
    }
    int word_count =0;// for the pointer stuff
    char anti_auto_word[MAX_COM];
    char auto_word[MAX_COM];
    memset(auto_word, '\0', MAX_COM);
    memset(anti_auto_word, '\0', MAX_COM);
    //printf("\n%d",strlen(CMD));
    int space_pos=-1;
    for(int i=strlen(CMD)-1;i>=0;i--){
        if(isspace(CMD[i])){
            space_pos=i;
            break;
        }
    }
    int pos =0;
    for(int i=space_pos+1;i<strlen(CMD);i++){
        auto_word[pos++]=CMD[i]; 
    }
    pos=0;
    for(int i=0;i<=space_pos;i++){
        anti_auto_word[pos++]=CMD[i]; 
    }
    if(strlen(auto_word)==0){
        struct dirent ** file_list;
        int count = scandir(".",&file_list,NULL,alphasort);
        if(count==2){
            return;
        }
        if(count==3){

            for(int i=0;i<count;i++){
                if(!strcmp(file_list[i]->d_name,".")){
                    continue;
                }
                if(!strcmp(file_list[i]->d_name,"..")){
                    continue;
                }
                strcpy(auto_word,file_list[i]->d_name);
                struct stat sau;
                lstat(file_list[i]->d_name,&sau);
                if(S_ISDIR(sau.st_mode)){
                    sprintf(auto_word,"%s/",auto_word);
                }
                else if(S_ISREG(sau.st_mode)){
                    sprintf(auto_word,"%s ",auto_word);
                }
                if(strlen(anti_auto_word)!=0){
                    sprintf(CMD,"%s%s",anti_auto_word,auto_word);
                }
                else{
                    sprintf(CMD,"%s",auto_word);
                }    
                
            }
        }
        else{
            new_line();
            for(int i=0;i<count;i++){
                if(!strcmp(file_list[i]->d_name,".")){
                    continue;
                }
                if(!strcmp(file_list[i]->d_name,"..")){
                    continue;
                }
                strcpy(cmp_word[word_count++],file_list[i]->d_name);
                struct stat sau;
                lstat(file_list[i]->d_name,&sau);
                if(S_ISDIR(sau.st_mode)){
                    sprintf(file_list[i]->d_name,"%s/",file_list[i]->d_name);
                }
                else if(S_ISREG(sau.st_mode)){
                    sprintf(file_list[i]->d_name,"%s ",file_list[i]->d_name);
                }
                prompt_write(file_list[i]->d_name);
                new_line();
            }
            int common = handle_word_list(word_count,cmp_word); // number of common letter of each
            char temp[MAX_COM/100];
            int pos_c =0;
            for(int i=0;i<common;i++){
                temp[pos_c++] = cmp_word[0][i];
            }
            temp[pos_c]='\0';
            strcpy(auto_word,temp);
            if(strlen(anti_auto_word)!=0){
                sprintf(CMD,"%s%s",anti_auto_word,auto_word);
            }
            else{
                sprintf(CMD,"%s",auto_word);
            } 
        }
        return;
    }
    else{
        // case where there is actually something written in the word
        int n_cmp = strlen(auto_word);
        struct dirent ** file_list;
        int count = scandir(".",&file_list,NULL,alphasort);
        for(int i=0;i<count;i++){
            if(!strcmp(file_list[i]->d_name,".")){
                continue;
            }
            if(!strcmp(file_list[i]->d_name,"..")){
                continue;
            }
            if(strlen(file_list[i]->d_name)<n_cmp){
                continue;
            }
            if(!strncmp(auto_word,file_list[i]->d_name,n_cmp)){
                strcpy(cmp_word[word_count++],file_list[i]->d_name);
            }
        }
        if(word_count==0){
            return;
        }
        if(word_count==1){
            strcpy(auto_word,cmp_word[0]);
            struct stat sau;
            lstat(auto_word,&sau);
            if(S_ISDIR(sau.st_mode)){
                sprintf(auto_word,"%s/",auto_word);
            }
            else if(S_ISREG(sau.st_mode)){
                sprintf(auto_word,"%s ",auto_word);
            }
            if(strlen(anti_auto_word)!=0){
                sprintf(CMD,"%s%s",anti_auto_word,auto_word);
            }
            else{
                sprintf(CMD,"%s",auto_word);
            } 
        }
        else{
            new_line();
            for(int i=0;i<word_count;i++){
                struct stat sau;
                lstat(cmp_word[i],&sau);
                if(S_ISDIR(sau.st_mode)){
                    sprintf(cmp_word[i],"%s/",cmp_word[i]);
                }
                else if(S_ISREG(sau.st_mode)){
                    sprintf(cmp_word[i],"%s ",cmp_word[i]);
                }
                prompt_write(cmp_word[i]);
                new_line();
            }
            int common = handle_word_list(word_count,cmp_word);
            char temp[MAX_COM/100];
            int pos_c =0;
            for(int i=0;i<common;i++){
                temp[pos_c++] = cmp_word[0][i];
            }
            temp[pos_c]='\0';
            strcpy(auto_word,temp);
            if(strlen(anti_auto_word)!=0){
                sprintf(CMD,"%s%s",anti_auto_word,auto_word);
            }
            else{
                sprintf(CMD,"%s",auto_word);
            } 
        }
    }
    

}
void take_inp(){
    strcpy(auto_curr_dir,".");
    char auto_prompt[MAX_COM];
    char c;
    setbuf(stdout,NULL);
    int pos =0;
    memset(CMD, '\0', MAX_COM);
    enableRawMode();
    while (read(STDIN_FILENO, &c, 1) == 1){
        if(iscntrl(c)){
            if(c==10){ // '\n' character
                CMD[pos++] =c;
                new_line();
                break;
            }
            else if(c==9){ // tab character
                //new_line();
                disableRawMode();
                handle_auto();
                //printf("\n");
                sprintf(auto_prompt,"%s%s",PROMPT_BUF,CMD);
                prompt_write(auto_prompt);
                enableRawMode();
                pos=strlen(CMD);
                continue;
            }
            else if(c==127){ // backspace character
                if (pos > 0) {
                    CMD[--pos] = '\0';
                    printf("\b \b");
                }
                continue;
            }
            else if(c==4){
                kill_children();
                disableRawMode();
                exit(1);
            }
        }
        if(isspace(c)){
            strcpy(auto_curr_dir,".");
        }
        CMD[pos++] =c;
        printf("%c",c);
    }
    CMD[pos]='\0';
    //printf("hey\n");
    disableRawMode();
    // prompt_write(CMD);
    // new_line();
    
    //fgets(CMD, sizeof(CMD), stdin);
    // scanf("%[^\n]%*c", CMD); did not use this as it had an issue with enter command as it doesn/t consider \n as a character
}