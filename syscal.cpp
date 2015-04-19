#include <iostream>
#include <unistd.h>//library for fork and execvp
#include <stdio.h> //library for perror
#include <stdlib.h>//libray for exit
#include <sys/types.h> 
#include <sys/wait.h>// library for wait
#include <array>
#include <string>
#include <cstring>

using namespace std;

void syscall(char**, bool&); 

void syscall(char* argv[], bool& pass){
	int status;	
	cout << " argv: " << argv << endl;		
	int pid=fork();	
	if(pid == -1){
		perror("There was a error in fork line 19"); //syscall to output error
		exit(1);//if it was a error then we try to fix it 
	}	

	else if(pid == 0){	
		cout<<"This is the child process"<<endl;
		if( execvp(argv[0], argv)==-1){
			perror("There was an error in execvp line 27");
			exit(EXIT_FAILURE); //when this child process finishes, then kills this process
		}
		else{
			exit(EXIT_SUCCESS);
		}
	}

	else if(pid > 0){//if it is the parent function
		int result_wait=wait(0);//wait wait for the child process
		if(result_wait==-1){
			perror("There was an error in wait line35");			
			exit(1);
		}
	}

	if(WEXITSTATUS(status)==EXIT_SUCCESS){
			cout << "child process success and exit" << endl;
			pass=true;
			}
	else if(WEXITSTATUS(status)==EXIT_FAILURE){
			cout << "child process not success and exit"<<endl;
			pass=false;
			}

}

int main(){
	char* cmdtest[100];
	string test1="ls"
	;
//mdtest[1] = "-a";
	bool pass=true;
	syscall(cmdtest,pass);	
	if(pass){
		cout <<" command successs "<< endl;	
	}

	if(!pass){
		cout << " command fails " << endl;
	}
	
	cout << " end of this program " << endl;

//	char *cinput= new char [input.length()+1];
//	strcpy (cinput,input.c_str());//cinput now contains a c-string copy of input
//	char *argv = strtok(cinput, " ");//
//	char **argv1= new char*[]
			
/*	while (i<10){
		int i=0;
		cout<<argv<<endl;
		argv1[i]=argv;
		argv = strtok(NULL, " ");
		i++;
		
		int pid=fork();
		if(pid == -1){
			perror("There was a error in fork line 19"); //syscall to output error
			exit(1);//if it was a error then we try to fix it 
		}	

		else if(pid == 0){//if it is the child process running
			cout<<"This is the child process"<<endl;
			cout<<"argv.size(): "<<strlen(argv)<<endl;
			if( execvp(argv[0], argv)==-1){
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

*/
	/*
	int i=0;
	while(i<20){
		cout<<argv1[i]<<" ";
		i++;		
	}	
	*/
//	delete[] cinput;

	
//	cout<<argc<<endl;
//	for (int i=0; i<argc; i++){
//		cout<< argv[i]<<" "<<endl;
//			argv1[i]=argv[i+1];
//			cout << argv1[i] << ' ';
//	}

//	cout<<endl;

/*
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
*/
	return 0;
}

