#rshell(hw0)
this project is for creating a shell that can run the commands in the terminal
the key function that used in this program:
fork() execvp() wait() exit()

##Features:
1. this program prints the username and hostname
2. this program can run many commands that ** execvp()**
3. this program can run connectors: ** || && ;**
4. 
5. 
##Prerequisite:
1. this program needs to run at Linux system.

##How to install:
`
$ git clone https://github.com/ycao006/rshell.git
$ cd rshell
$ git checkout hw0
$ make
$ bin/rshell


##Bugs:
1. **ls   ;   ** is a bug

