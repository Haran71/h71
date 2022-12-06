#ifndef _HEADER_H_
#define _HEADER_H_

// headers used in my shell

#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/utsname.h>
#include<sys/time.h>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<pwd.h>
#include<ctype.h>
#include<dirent.h>
#include<grp.h>
#include<signal.h>
#include<stddef.h>
#include<termios.h>
// some limits

#define MAX_COM 100000
#define HIST_COUNT 20
#define HIST_READ 10
#define MAX_BG 1024

// template used to pass a function as an argument to another function. Pretty cool :)!
typedef void sig_handler(int ,siginfo_t*,void *);

// struct containing information about the background child processes to be called upon later
typedef struct child_p{
    pid_t pid_no;
    char p_name[MAX_COM/100];
    int job_no;
}child_proc;


#endif