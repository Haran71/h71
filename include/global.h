#include"header.h"
#ifndef _GLOBAL_H_
#define _GLOBAL_H_  


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m" 
#define KGRN  "\x1B[32m" 
#define KYEL  "\x1B[33m"  
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// Intializing all the global variables

extern int godfather; // pid of the original shell process

// main buffer in which input is taken
extern char CMD[MAX_COM];  

// buffer in which command is stored after being parsed by semicolon
extern char cmd_w[MAX_COM]; 

// the actual command without the flags
extern char command[MAX_COM];   

// the argument given with the cd argument
extern char cd_flag[MAX_COM];  

// the path of the diretory we need to get to
extern char cd_target[MAX_COM];  

// path of the home directory
extern char home_dir[MAX_COM];

// path of the previous working directory
extern char prev_dir[MAX_COM]; 

// Variables used for ls

extern char permissions[15]; // to be used in the ls command for storing permissions
extern char colors[MAX_COM/100]; // used to store the colored string

// running variables
extern int exit_flag; // used to exit the shell
extern int success; // flag to check if a process executed successfully



//used for the discover command as recursion is to be used
extern int flag_d; // check for -d flag
extern int flag_f; // check for -f flag
extern int found; // if a target file is given, check if its found
extern char target_file[MAX_COM/100]; // path of the file to be found
extern char root_dir_path[MAX_COM/100]; // gets absolute path of the directory
extern char token_god[MAX_COM/100]; // what the the root_dir_path must be replaced by
extern char final_path[MAX_COM/100]; // contains the absolute path of the file to be found;

// for bg_aux
extern child_proc * child_list[MAX_BG]; // array of structs to store information about the bg processes
extern int num_children; // number of active bg processes
extern child_proc foreground; // stores the pid of the current foregound process

// for time for foreground processes
extern struct timeval tv; 
extern struct timezone tz;
extern time_t start;
extern time_t end;
extern int disp_with_time;


extern struct termios auto_term;
extern char PROMPT_BUF[MAX_COM];

#endif