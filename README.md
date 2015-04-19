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

##Limitations
1. Input **ls;** will show: syntax error in the end of the input
2. Input **ls;;** will show: syntax error with ;; in the end
3. Input start with **#** will return nothing, user can input again 
4. Input such as **ls||||pwd** will show the syntax error about connector and do the first command
5. Input such as **   |   ** will show up two syntax error

##Bugs:
1. **ls  ;** is a bug


