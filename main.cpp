#include <iostream>
#include <unistd.h>//library for fork and execvp
#include <stdio.h> //library for perror
#include <stdlib.h>//libray for exit
#include <sys/types.h> 
#include <sys/wait.h>// library for wait
#include <array>
using namespace std;


int main(int argc, char** argv){
	char* argv1[argc-1];
	
//	cout<<argc<<endl;
	for (int i=0; i<argc; i++){
//		cout<< argv[i]<<" "<<endl;
			argv1[i]=argv[i+1];
			cout << argv1[i] << ' ';
	}

	cout<<endl;

	for (int i=0; i<argc; i++){
	int pid=fork();
	if(pid == -1){
		perror("There was a error in fork line 19"); //syscall to output error
		exit(1);//if it was a error then we try to fix it 
	}

	else if(pid == 0){//if it is the child process running

		cout<<"This is the child process"<<endl;
		cout<<"arrgv1.size(): "<<argv1.size()<<endl;
		if( execvp(argv1[0], argv1)==-1){
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
	}

	return 0;
}

