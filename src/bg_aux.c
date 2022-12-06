#include"../include/functions.h"
#include"../include/global.h"
#include"../include/header.h"

void create_child_list(){
    foreground.pid_no=-1;
    for(int i=0;i<MAX_BG;i++){
        child_list[i] = (child_proc*)malloc(sizeof(child_proc));
        child_list[i]->pid_no = -1;
    }
}

void add_child(pid_t pid,char * token){
    
    for(int i=0;i<MAX_BG;i++){
        if(child_list[i]->pid_no==pid){
            return;
        }
    }
    num_children++;
    for(int i=0;i<MAX_BG;i++){
        if(child_list[i]->pid_no==-1){
            child_list[i]->pid_no = pid;
            strcpy(child_list[i]->p_name,token);
            child_list[i]->job_no = num_children;
            return;
        }
    }
    prompt_error_write("unable to add process to background");
    new_line_error();
    return;
}


void kill_children(){
    for(int i=0;i<MAX_BG;i++){
        if(child_list[i]->pid_no !=-1){
            kill(child_list[i]->pid_no,SIGKILL);
        }
    }
}


//signals

sig_handler* handle_signal(int signum, sig_handler* handler)
{
	struct sigaction new_action, old_action;
	
	memset(&new_action, 0, sizeof(struct sigaction));
	new_action.sa_sigaction = handler;
	sigemptyset(&new_action.sa_mask);
	
	new_action.sa_flags = SA_RESTART|SA_SIGINFO;

	if (sigaction(signum, &new_action, &old_action) < 0)
	{
		prompt_error_write("signal: error occurred");
        new_line_error();
	}
    return (old_action.sa_sigaction);                                           
}

void bg_child(int signum,siginfo_t* info,void* v){
    int status;
    pid_t pid;
    int check=0;
    char temp[MAX_COM];
    while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
	{
		for(int i = 0; i<MAX_BG; i++)
		{
			if(child_list[i]->pid_no == pid)
			{
				check =1;
				sprintf(temp, "\n%s with pid %d exited %s", child_list[i]->p_name, pid, WIFEXITED(status) == 0 ? "abnormally" : "normally");
				prompt_write(temp);
                if(!WIFSTOPPED(status)){
                    // this checks if the process was stopped. If not, it is removed from the list
                    child_list[i]->pid_no= -1;
                    num_children--;
                }    
				break;
			}
		}
	}
    if(check ==1){
        new_line();
        disp_get();
    }
}

// Some builtin bg and fg functions

void h71_bg(){
    char *token;
    char temp_buf[MAX_COM];
    strcpy(temp_buf,cmd_w);

    token = strtok(temp_buf," ");
    token = strtok(NULL," ");

    if(token==NULL){
        prompt_error_write("bg: 2 arguments are to be passed");
        new_line_error();
        success=1;
        return;
    }

    int job_num = atoi(token);
    token = strtok(NULL, " ");
    
    if(token!=NULL){
        prompt_error_write("bg: Only 2 arguments are to be passed");
        new_line_error();
        success=1;
        return;
    }
    for(int i=0;i<num_children;i++){
        if(child_list[i]->pid_no!=-1 && child_list[i]->job_no == job_num){ 
            // sending the SIGCONT to the required child process
            int cont_stat = kill(child_list[i]->pid_no,SIGCONT);
            if(cont_stat<0){
                prompt_error_write("bg: Signal could not be sent");
                new_line_error();
                success=1;
                return;
            }
            return;
        }
    }
    prompt_error_write("bg: invalid job_no given");
    new_line_error();
    success=1;
    return; 
}

void h71_fg(){
    pid_t curr_pid = getpid();
    char *token;
    char temp_buf[MAX_COM];
    strcpy(temp_buf,cmd_w);

    token = strtok(temp_buf," ");
    token = strtok(NULL," ");

    if(token==NULL){
        prompt_error_write("fg: 2 arguments are to be passed");
        new_line_error();
        success=1;
        return;
    }

    int job_num = atoi(token);
    token = strtok(NULL, " ");
    
    if(token!=NULL){
        prompt_error_write("fg: Only 2 arguments are to be passed");
        new_line_error();
        success=1;
        return;
    }
    for(int i=0;i<num_children;i++){
        if(child_list[i]->pid_no!=-1 && child_list[i]->job_no == job_num){

            foreground.pid_no=child_list[i]->pid_no;
            signal(SIGTTOU,SIG_IGN);
            signal(SIGTTIN,SIG_IGN);
            tcsetpgrp(STDIN_FILENO, getpgid(child_list[i]->pid_no));
            // making the foreground process group that of the child process

            int cont_stat = kill(child_list[i]->pid_no,SIGCONT);
            if(cont_stat<0){
                prompt_error_write("fg: Signal could not be sent");
                new_line_error();
                success=1;
                return;
            }

            int status;
            
            waitpid(child_list[i]->pid_no,&status,WUNTRACED);

            foreground.pid_no=-1;
            // resetting the foregorund process group 
            tcsetpgrp(STDIN_FILENO,curr_pid);
            signal(SIGTTOU,SIG_DFL);
            signal(SIGTTIN,SIG_DFL);
            

            if(WIFEXITED(status)){
                child_list[i]->pid_no=-1;
            }

            return;
        }
    }
    prompt_error_write("fg: invalid job_no given");
    new_line_error();
    success=1;
    return; 

}

void h71_sig(){
    char *token;
    char temp_buf[MAX_COM];
    strcpy(temp_buf,cmd_w);

    token = strtok(temp_buf," ");
    token = strtok(NULL," ");

    if(token==NULL){
        prompt_error_write("sig: 3 arguments are to be passed");
        new_line_error();
        success=1;
        return;
    }

    int job_num = atoi(token);
    token = strtok(NULL, " ");
    
    if(token==NULL){
        prompt_error_write("sig: 3 arguments are to be passed");
        new_line_error();
        success=1;
        return;
    }
    int sig_no = atoi(token);
    token = strtok(NULL, " ");

    if(token!=NULL){
        prompt_error_write("sig: Only 3 arguments are to be passed");
        new_line_error();
        success=1;
        return;
    }

    for(int i=0;i<num_children;i++){
        if(child_list[i]->pid_no!=-1 && child_list[i]->job_no == job_num){
            int cont_stat = kill(child_list[i]->pid_no,sig_no);
            if(cont_stat<0){
                prompt_error_write("sig: Signal could not be sent");
                new_line_error();
                success=1;
                return;
            }
            return;
        }
    }
    prompt_error_write("sig: invalid job_no given");
    new_line_error();
    success=1;
    return; 
}

void h71_jobs(){
    int flag_r =0;
    int flag_s = 0;
    int flag_check=0;
    char temp_buf[MAX_COM];

    char * token;
    strcpy(temp_buf,cmd_w);

    token = strtok(temp_buf," ");
    while(token!=NULL){
        if(token[0]=='-'){
            for(int i=1;i<strlen(token);i++){
                if(token[i]=='r'){
                    flag_r =1;
                }
                else if(token[i]=='s'){
                    flag_s=1;
                }
                else{
                    flag_check=1;
                }
            }
        }
        token = strtok(NULL, " ");
    }
    if(flag_check){
        prompt_error_write("jobs: Invalid flag specified");
        new_line_error();
        success=1;
        return;
    }
    if(!flag_r && !flag_s){
        flag_r=1;
        flag_s=1;
    }
    child_proc * sorted_list[MAX_BG]; // the list that is going to be sorted alpabetically
    for(int i=0;i<MAX_BG;i++){
        sorted_list[i] = (child_proc*)malloc(sizeof(child_proc));
    }
    int pos=0;
    for(int i=0;i<MAX_BG;i++){
        if(child_list[i]->pid_no!=-1){
            sorted_list[pos]->pid_no=child_list[i]->pid_no;
            sorted_list[pos]->job_no=child_list[i]->job_no;
            strcpy(sorted_list[pos]->p_name,child_list[i]->p_name);
            pos++;
        }
    }
    for(int i=0;i<pos;i++){
        for(int j=0;j<pos-i-1;j++){
            if(strcmp(sorted_list[j]->p_name,sorted_list[j+1]->p_name)>0){
                char temp[MAX_COM/100];
                strcpy(temp,sorted_list[j]->p_name);
                strcpy(sorted_list[j]->p_name,sorted_list[j+1]->p_name);
                strcpy(sorted_list[j+1]->p_name,temp);

                int temp_i;
                temp_i = sorted_list[j]->pid_no;
                sorted_list[j]->pid_no = sorted_list[j+1]->pid_no;
                sorted_list[j+1]->pid_no = temp_i;

                temp_i = sorted_list[j]->job_no;
                sorted_list[j]->job_no = sorted_list[j+1]->job_no;
                sorted_list[j+1]->job_no = temp_i;
            }
        }
    }
    char proc_path[30];
    char out_string[100];
    for(int i=0;i<pos;i++){
        sprintf(proc_path,"/proc/%d/stat",sorted_list[i]->pid_no);
        FILE * fp = fopen(proc_path,"r");
        if(fp!=NULL){
            pid_t pid;
            char proc_status;
            fscanf(fp,"%d",&pid);

            fscanf(fp,"%c",&proc_status);
            while(proc_status!=')') fscanf(fp,"%c",&proc_status);
            while(proc_status==')') fscanf(fp,"%c",&proc_status);

            fscanf(fp,"%c",&proc_status);

            if(proc_status=='T' || proc_status=='t'){
                sprintf(out_string,"[%d] Stopped",sorted_list[i]->job_no);
                if(!flag_s){
                    continue;
                }
            }
            else{
                sprintf(out_string,"[%d] Running",sorted_list[i]->job_no);
                if(!flag_r){
                    continue;
                }
            }
            sprintf(out_string,"%s %s [%d]",out_string,sorted_list[i]->p_name,sorted_list[i]->pid_no);
            prompt_write(out_string);
            new_line();
        }
    }

}

// ctrl commands +c,+z,+d

void ctrl_c(int signum,siginfo_t* info,void* v){
    return;
}
void ctrl_z(int signum,siginfo_t* info,void* v){
    pid_t pid_f = foreground.pid_no;
    pid_t pid = getpid();
    if(pid!=godfather){
        return;
    }
    if(foreground.pid_no==-1){
        return;
    }

    if(num_children==MAX_BG){
        prompt_error_write("&: the number of background processes is already maximum");
        new_line_error();
        success=1;
        return;
    }
    setpgid(pid_f,0); // changes the process group id of the foreground process

    int stop_stat = kill(pid_f, SIGSTOP);
    if(stop_stat<0){
        prompt_error_write("ctrl_z: Couldn't send signal");
        new_line_error();
        success=1;
        return;
    }
    
    tcsetpgrp(STDIN_FILENO,getpgid(godfather));
    
    signal(SIGTTIN,SIG_DFL);
    signal(SIGTTOU,SIG_DFL);        

    add_child(pid_f,foreground.p_name);
}



