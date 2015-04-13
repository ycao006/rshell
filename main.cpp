#include <iostream>
#include <unistd.h>//library for fork and execvp
#include <stdio.h> //library for perror
#include <stdlib.h>//libray for exit
#include <sys/types.h> 
#include <sys/wait.h>// library for wait
using namespace std;


int main(int argc, char** argv){

	cout<<argc<<endl;
	for (int i=0; i<argc; i++){
		cout<< argv[i]<<" "<<endl;
	}

	int pid=fork();
	if(pid == -1){
		perror("There was a error in fork line 19"); //syscall to output error
		exit(1);//if it was a error then we try to fix it 
	}

	else if(pid == 0){//if it is the child process running
		cout<<"This is the child process"<<endl;
		cout<<"THis is the argv[2]: "<<argv[2]<<endl;
		int result_execvp = execvp(argv[2], argv);
		if(result_execvp==-1){
			perror("There was an error in execvp line 27");
			exit(1); //when this child process finishes, then kills this process
		}
	}

	else if(pid > 0){//if it is the parent function
		int result_wait=wait(0);//wait wait for the child process
		if(result_wait==-1){
			perror("There was an error in wait line35");			
		
		}
	}	
	
	return 0;
}

