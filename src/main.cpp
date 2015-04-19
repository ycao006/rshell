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
void inputStore(char*[],char[],const char*);
void inputReform(string&, char*[]);
void connector_format(string&);
void syscall(char**, bool&, int&); 


int main(){

	while(1){
Origin:		
		int maxCap=2048;
		int I=0;
		char* commandList[2048];
		string input;
		string input2;
		int status;
		bool pass = true;
		char* temp[2048];
		int pos1 = 0;
		int pos2 = 0;
		char errorList[3] = {'|', '&', ';'};
		bool errorFlag1 = false;
		char hostname[256];
		size_t hostname_len = 256;
		gethostname(hostname,hostname_len);
		cout<<getlogin()<<"@"<<hostname<<" $ ";
		getline(cin,input);
		//check 4 different types of input conditions when input is a string
checkInput:
		if((input== "")||(input.at(0))=='#'){
			goto Origin;		
		}
		if(input.size() == 1){
			for(unsigned int k=0 ; k<3 ; k++){
				if(input.at(0) == errorList[k]){
					cout<<"syntax error with | & ; in the front"<<endl;
					goto Origin;		
				}		
			}		
		}

		for(unsigned int k = 0 ; k<3 ; k++ ){
			if(input.at(input.size()-1) == errorList[k]){

				if(input.at(input.size()-2) == errorList[k]){
					if(input.at(input.size()-2) == ';'){
						cout<<"syntax error with ;; in the end"<<endl;	
						goto Origin;
					}
					else{
						cout<<"> ";
						getline(cin,input2);
						input = input+input2;
						goto checkInput;
					}
				}
				else{
					cout<<"syntax error in the end of the input"<<endl;
					goto Origin;					
				}
			
			}
		}

		for (I = 0; I < maxCap; ++I) {
			commandList[I] = new char[maxCap];
		}
		//convert string into cstring;
		//breaks the cstring into parts and store them into input array commandList;
		inputReform(input, commandList);

		// check different input conditions when inputs in cstring
		if(commandList[0] == NULL){
			goto Origin;	
		}
		if(commandList[1] == NULL){
				for(unsigned int k=0 ; k<3 ; k++){
					if(commandList[0][0] == errorList[k]){
						cout<<"syntax errors: || && ; at front"<<endl;
						goto Origin;
					}
				}		
		}
		for(int k=0; k<3; k++){
			if(commandList[0][0] == errorList[k]){
				cout<< "syntax errors: || && ; at front"<<endl;		
				goto Origin;
			}		
		}
		int i=0;
		while((commandList[i] != NULL)&&(!errorFlag1)){
			if(strcmp(commandList[i],"||") == 0){
				for(int k=0 ; k<3 ; k++){
					if(commandList[i+1][0] == errorList[k]){
						cout << "syntax errors related to || && ;"<<endl;
						goto Origin;		
					}
				}
				int j = 0;
				while(j < pos2-pos1){
					temp[j] = commandList[j+pos1];		
					j++;
				}
				//exit program
				if(strcmp(temp[0],"exit") == 0){
					return 0;
				}
				syscall(temp,pass,status);
				if(pass){
					break;
				}
				else{
					pos1 = pos2+1;
				}
			}
	
			else if(strcmp(commandList[i],"&&") == 0){
				for(int k=0 ; k<3 ; k++){
					if(commandList[i+1][0]==errorList[k]){
						cout << "syntax errors related to || && ;"<<endl;
						goto Origin;		
					}
				}
				int j = 0;
				while(j < pos2-pos1){
					temp[j] = commandList[j+pos1];		
					j++;
				}
				if(strcmp(temp[0],"exit") == 0){
					return 0;
				}
				syscall(temp,pass,status);
				if(!pass){
					break;
				}
				else{
					pos1 = pos2+1;
				}
	
			}
	
			else if(strcmp(commandList[i],";") == 0){
				for(int k=0 ; k<3 ; k++){
					if(commandList[i+1][0]==errorList[k]){
						cout << "syntax errors related to || && ;"<<endl;
						goto Origin;		
					}
				}
				int j = 0;
				while(j < pos2-pos1){
					temp[j] = commandList[j+pos1];		
					j++;
				}
				if(strcmp(temp[0],"exit") == 0){
					return 0;
				}
				syscall(temp,pass,status);
				pos1=pos2+1;
			}	
			i++;
			pos2=i;
			
			if(commandList[i]==NULL){
				int j = 0;
				while(j < pos2-pos1){
					temp[j] = commandList[j+pos1];		
					j++;
				}
				if(strcmp(temp[0],"exit") == 0){
					return 0;
				}
				syscall(temp,pass,status);
			}	
						
		}
	}
	return 0;
	
}

	//converted string into cstring;
	void inputReform(string& str, char*array[]){
		unsigned int pos = str.find('#');
		char cstr[2048]; //  
		// ignore the string after #
		if(pos != string::npos){
			str = str.substr(0,pos);
		}
	
		connector_format(str);
		strcpy(cstr , str.c_str());
		int i = 0;
		array[i] = strtok(cstr , " ");
		while(array[i] != NULL){
			i++;
			array[i] = strtok(NULL," ");
		}
		return;
	}

	//add " " in front of and behind "||" "&&". in addition replace ";" with " "
	void connector_format(string& str){		
 		size_t found = 0;
		unsigned int strl = 0;
		string connectors[3] = {"||", "&&", ";"};
		string Newconnectors[3] = {" || ", " && ", " ; "}; 
	
		for (unsigned int i=0; i<3; i++){
			found = 0;
			while(found != string::npos){
				found = str.find(connectors[i],found);
				
				if(found!=string::npos){
					str.replace(found , connectors[i].length() , Newconnectors[i]);
					strl = str.length();
					if((found+3) < strl){
						found = found+3;
					}
					else{
						break;
					}
				} 
				
			}
		}

		return;					
	}

	void syscall(char* argv[], bool& pass, int& status){
	//	int status;	
		int pid = fork();	
		if(pid == -1){
			perror("There was a error in fork line 19"); //syscall to output error
			exit(1);//if it was a error then we try to fix it 
		}	

		else if(pid == 0){	
			if( execvp(argv[0], argv) == -1){
				perror("There was an error in execvp");
				exit(EXIT_FAILURE); //when this child process finishes, then kills this process
			}
			else{
				exit(EXIT_SUCCESS);
			}
		}

		else if(pid > 0){//if it is the parent function
			int result_wait = wait(&status);//wait wait for the child process
			if(result_wait==-1){
			perror("There was an error in wait line35");			
			exit(1);
		}
		}

		if(WEXITSTATUS(status)==EXIT_SUCCESS){
			pass=true;
		}
		else if(WEXITSTATUS(status)==EXIT_FAILURE){
			pass=false;
		}

	}


