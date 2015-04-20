#rshell(hw0)
1. this project is for creating a shell that can run the commands in the terminal
2. the key function that used in this program: fork() execvp() wait() exit()

##Features
1. this program prints the username and hostname
2. this program can run many commands that ** execvp()**
3. this program can run connectors: ** || && ;**
4. this program can run mix among commands and connectors 
 
##Prerequisite
1. this program needs to run at Linux system.

##How to install
To run this program in your Linux system, please do following:
```
$ git clone https://github.com/ycao006/rshell.git
$ cd rshell
$ git checkout hw0
$ make
$ bin/rshell
```

##situations in my program
1. Input with only space, user can input again.
2. Input start with **#**, program will return nothing, user can input again 
3. Input with wrong connecors in the front or last or midlle, program will tell user syntax error

##Bugs in my program:
1. the input command can not end with space 
2. the input such as |& &| will generate error that affects the following input commands
3. after using ls plus a flag such as ls -a. the ls command can not use again
 

