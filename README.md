#rshell(hw0)+ls(hw1)+pipe(hw2)
1. this project is for creating a shell that can run the commands in the terminal
2. the key function that used in this program: fork() execvp() wait() exit()
3. the key function that uses in ls.cpp: stat() opendir() closedir() readdir()
4. the key function that uses in redirect.cpp: fork() execvp() wait() exit() open() write() pipe() dup() dup2()

##Features
1. rshell prints the username and hostname
2. rshell can run many commands that **execvp()**
3. rshell can run connectors: **|| && ;**
4. rshell can run rediect connector: ** >   >>   <   <<<   2>   2>>**
5. rshell can run pipe: **|**
6. rshell can detect wrong inputs;
6. rshell can run run any mix commands and connectors mentioned above.
7. ls.cpp can detect invalid input
8. ls.cpp can behave base **ls** command and flag **-a -l -R**


##Prerequisite
1. this program needs to run at Linux system.

##How to install
To run this program in your Linux system, please do following:
```
$ git clone https://github.com/ycao006/rshell.git
$ cd rshell
$ git checkout hw2
$ make
$ bin/rshell
```

##Situations in my program
1. Input with only space, user can input again.
2. Input start with **#**, program will return nothing, user can input again 
3. Input with wrong connectors including **|| && ; |** in the front or last or midlle, program will tell user syntax error
4. **<<<** in my rshell is a little different with **<<<** in the bash in following:
	Input: **<<< "hello world!!       it is wonderful"   output: hello world!! it is wonderful**
	in my rshell, in the output, there is always only one space between the words 
5. **LHS | RHS** is able to make the LHS output become RHS input; the program will display error message when input is wrong related to **|**
6. **n> n>>** in my program the number n is a number betwen 0 and 9; the program will display error message when input is wrong related to **n> n>>**
7. **LHS > RHS** output redirection let the LHS content go to RHS the program will display error message when input is wrong related to **>**
8. **LHS < RHS** input redirection let the RHS content go to LHS;  the program will display error message when input is wrong related to **<**
9. **<<< str1 str2** **<<<** only pick up the str1 and display on the screen; the program will display error message when input is wrong related to **<<<**
10. If there is a connector between a two double quotations such as ** ";" ** rshell will not interprete these two quotation together. 
 
##Bugs in my program:
1. the input command can not end with space 
2. the input such as |& &| will generate error that affects the following input commands(fixed)
3. after using ls plus a flag such as ls -a. the ls command can not use again(fixed)
4. Segfault will be happened due to the server error when we run -alR sometimes
5. when two pipe show up between two command, my code does not show syntax error 
6. <<<5. when two pipe show up between two command, my code does not show syntax error 
6. <<<5. when two pipe show up between two command, my code does not show syntax error 
6. <<<5. when two pipe show up between two command, my code does not show syntax error 
6. <<<5. when two pipe show up between two command, my code does not show syntax error 

