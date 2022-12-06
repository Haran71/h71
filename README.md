# h71

  

h71 is a shell coded in C that simulates bash.Certain functions were implemented from scratch while others were implemented using the `execvp()` function.

  

## How to Run

  

Navigate to the required directory where the files are present and run the `make` command, which will produce an executable file `h71`. In order to run the executable, use the command `./h71`.

  

## File Structure

  

### include

##### functions.h

Contains the declaration of all functions used across files.

##### global.h

Contains the declaration of all global variables

##### header.h

Contains all the headers used in the shell and also some buffer size limits

### src

##### Main.c

File that contains the main() function. Runs various initial function like signal handling. Contains indefenite while loop that callas h71_main function

##### genFun.c

Contains various helper functions that are used in order to implemnt the various specifications of the shell

##### bg_aux.c

Contains some auxillary functions that help with background processes and signal handling

##### h71_main.c

Contains the function that parses the command based on the semi-colon and calls various functions in order to handle the parsed string

##### h71_cd.c

Contains the function that executes the `cd` command

##### h71_pwd.c

Contains the function that executes the `pwd` command

##### h71_pinfo.c

Contains the function that executes the `pinfo` command

##### h71_ls.c

Contains the function that executes the `ls` command

##### h71_echo.c

Contains the function that executes the `echo` command

##### h71_discover.c

Contains the function that executes the `discover` command

##### h71_history.c

Contains the function that executes the `history` command. Also contains the function which writes the command to the .h71_history.txt file which is created in the home directory of the system as a hidden file by the `history_init()` function.

##### h71_process.c

Contains the function that processes commands that are not to be implemented via execvp and parses that string based on '&' and checks whether the command is to be execueted in the foregorund or the background

##### h71_foregound.c

Runs the required command as a foregound process

##### h71_background.c

Runs the required command as a background process

##### autocomp.c

Contains the functions that handle autocomplete functionality

##### h71_pipe.c

Contains the function that handles piping

##### h71_redir.c

Contains the function that handles I/O redirection

  

## Command Usage

##### echo

Syntax:`echo <string>`

Echo simply writes the given string to the terminal

##### pwd

Syntax:`pwd`

Displays the absolute path of the current working directory

##### cd

Syntax:`cd <arg>`

arg->dirrectory name. Also supports `. , .. , ~ , - `.

Changes current directory to the directory specified in the argument. If no argument is given then the current directory is changed to the home directory

##### ls

Syntax:`ls <flags><dir_name/file_name>`

Lists all the files and directories in the specified directory. Supports -a and -l flags. If a file name is given as an argument, then information about the file itself is displayed

##### discover

Syntax:`discover <dir_name><flags><target_file>`

Lists all the files and directories in the given directory hierarchy. Supports -f and -d flags. If a target file is given, then the command will diplay the path of the file if found.

##### pinfo

Syntax:`pinfo <pid>`

Displays information about the process with the pid given as argument. Information inlcudes status code, executable path, virtual memory size and PID.

##### history

Syntax:`history`

Displays atmost the last 10 commands taken as input

##### quit

Syntax:`quit`

Terminates the shell program. Aliases include `q , exit `

##### fg

Syntax:`fg <job_no>`

Brings a process from the background to the foreground and changes it's status to running

##### bg

Syntax:`bg <job_no>`

Signals a stopped background process to continue running

##### sig

Syntax:`sig <job_no> <sig_no>`

Sends the given signal to process with the given job_no

##### jobs

Syntax:`jobs <flags>`

Outputs the list of processes (in alphabetical order) in the job pool with their status

  
  
  

## Assumptions

  

-> Current max command length is 10^5 characters

  

-> If multiple forground processes are run, then the prompt will display the time taken by the foreground process that finished execueting last

  

-> A maximum of 1024 processes can be run in the background at any given point

  

-> `history` command is implemented by creating a hidden file .h71_history.txt in the home file of the system and storing the previous 20 commands. The command accepts no arguments and will just return atmost the last 10 commands.