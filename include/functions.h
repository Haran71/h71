#include"header.h"
#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_


// writes to stdout
void prompt_write(char * buffer);

// writes to stderror
void prompt_error_write(char * buffer);

// writes a "\n" to stdout
void new_line();

// writes a "\n" to stderror
void new_line_error();

// inserts pace in stdout
void insert_space();

// writes the prompt to the terminal
void disp_get();

// gets input from the user
void take_inp();

// cleans up the string in place
void clean_command(char * buffer);

// calls required function stored in cmd_w string
void command_parse(); 

// checks if given string is a command that not called by execvp
int command_check(char * token);

// gets the command without the flags of string in cmd_w
void get_command(); 

// gets the command without the flags of string token
void get_command_token(char * token);

// returns the target dir from the cd arg
void do_cd(); 

// function to set the home dir and prev-dir
void set_dir();

// function to get the permission string for a file in ls
void get_perm(char * file_path);

// fucntion to convert number XX to coresponding month
void disp_month(char * month);



// bg_aux
void create_child_list();
void add_child(pid_t pid,char * token);
void kill_children();

//signals
sig_handler* handle_signal(int signum, sig_handler* handler);
void bg_child(int signum,siginfo_t* info,void* v);
void ctrl_c(int signum,siginfo_t* info,void* v);
void ctrl_z(int signum,siginfo_t* info,void* v);
void ctrl_d(int signum,siginfo_t* info,void* v);

void enableRawMode();
void disableRawMode();


void h71_main();
void h71_pwd();
void h71_cd();
void h71_echo();
void h71_ls();
void h71_discover();
void h71_pinfo();
void h71_process();
void h71_foreground(char * token);
void h71_background(char* token);
void history_write();
void history_read();
void history_init();
void h71_redir();
void h71_pipe();
void h71_bg(); // in bg_aux
void h71_fg(); // in bg_aux
void h71_sig(); // in bg_aux
void h71_jobs(); // in bg_auxs


#endif