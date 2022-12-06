#include"../include/header.h"
#include"../include/functions.h"
#include"../include/global.h"

// Intializing all the global variables

int godfather; // pid of the original shell process

// main buffer in which input is taken
char CMD[MAX_COM];  

// buffer in which command is stored after being parsed by semicolon
char cmd_w[MAX_COM]; 

// the actual command without the flags
char command[MAX_COM];   

// the argument given with the cd argument
char cd_flag[MAX_COM];  

// the path of the diretory we need to get to
char cd_target[MAX_COM];  

// path of the home directory
char home_dir[MAX_COM];

// path of the previous working directory
char prev_dir[MAX_COM]; 

// Variables used for ls

char permissions[15]; // to be used in the ls command for storing permissions
char colors[MAX_COM/100]; // used to store the colored string

// running variables
int exit_flag; // used to exit the shell
int success; // flag to check if a process executed successfully



//used for the discover command as recursion is to be used
int flag_d; // check for -d flag
int flag_f; // check for -f flag
int found; // if a target file is given, check if its found
char target_file[MAX_COM/100]; // path of the file to be found
char root_dir_path[MAX_COM/100]; // gets absolute path of the directory
char token_god[MAX_COM/100]; // what the the root_dir_path must be replaced by
char final_path[MAX_COM/100]; // contains the absolute path of the file to be found;

// for bg_aux
child_proc * child_list[MAX_BG]; // array of structs to store information about the bg processes
int num_children; // number of active bg processes
child_proc foreground;

// for time for foreground processes
struct timeval tv; 
struct timezone tz;
time_t start;
time_t end;
int disp_with_time;

struct termios auto_term;
char PROMPT_BUF[MAX_COM];

int main(){

    exit_flag = 0;
    num_children =0;
    disp_with_time=0;
    success =0;

    // functions that have to be called once
    godfather=getpid();

    set_dir();
    create_child_list();
    handle_signal(SIGCHLD,bg_child);
    handle_signal(SIGINT,ctrl_c);
    handle_signal(SIGTSTP,ctrl_z);
    history_init();     

    while(1){
        
        h71_main();
        
        if(exit_flag == 1){
            kill_children(); // killing all background processes
            exit(0);
        }
    }
    return 0;
}