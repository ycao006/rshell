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
					errorFlag1 = true;
					cout<<"syntax error in the end of the input"<<endl;
				
				}
			
			}
		}

		for (I = 0; I < maxCap; ++I) {
			commandList[I] = new char[maxCap];
		}
		//convert string into cstring;
		//breaks the cstring into parts and store them into input array commandList;
		cout<<"input is fine"<<endl;
		inputReform(input, commandList);
		cout<<"input inputreform is fine"<<endl;
		if(commandList[1] == NULL){
				for(unsigned int k=0 ; k<3 ; k++){
					if(commandList[0][0] == errorList[k]){
						cout<<"syntax errors: || && ; at front"<<endl;
					}
				}		
		}
		for(int k=0; k<3; k++){
			if(commandList[0][0] == errorList[k]){
				cout<< "syntax errors: || && ; at front"<<endl;		
				errorFlag1 = true;
				break;
			}		
		}
		int i=0;
		while((commandList[i] != NULL)&&(!errorFlag1)){
			if(strcmp(commandList[i],"||") == 0){
				for(int k=0 ; k<3 ; k++){
					if(commandList[i+1][0] == errorList[k]){
						cout << "syntax errors related to || && ;"<<endl;
						break;		
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
						break;		
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
						break;		
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
		char cstr[2048]; // in my code at this point char* cstr does not work.
//		const char* test;
		int i = 0;
		// ignore the string after #
		if(pos != string::npos){
			str = str.substr(0,pos);
//			cout << "str: "<< str << " str size:  " << str.length() << endl << endl << endl;
		}
	
		connector_format(str);
//		cout << "connector function is fine " << endl;
//		test=str.c_str();//test
//		cout << "c_str function is fine " << endl;
		strcpy(cstr , str.c_str());
//		cout << "strcpy function is fine" << endl;
		array[i] = strtok(cstr , " ");
		while(array[i] != NULL){
			i++;
			array[i] = strtok(NULL," ");
		}
//		i=0;
//		cout << "stroke function is fine"<<endl;
//		while(array[i] != NULL){
//			cout<<" i: "<<i<<" array[i] "<<array[i]<<endl;		
//			i++;
//		}
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
//			cout << connectors[i] << endl;     
			while(found != string::npos){
//				cout << "found0 " << found << endl;
				found = str.find(connectors[i],found);
//				cout << "found1 " << found << endl;
				
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
				
//				cout << "found2 " << found << endl;
//				cout << "new str " << str << " new str size: " << str.length() << endl;
			}
		}

		return;					
	}

	void syscall(char* argv[], bool& pass, int& status){
	//	int status;	
//		cout << " argv: " << argv << endl;		
		int pid = fork();	
		if(pid == -1){
			perror("There was a error in fork line 19"); //syscall to output error
			exit(1);//if it was a error then we try to fix it 
		}	

		else if(pid == 0){	
//			cout<<"This is the child process"<<endl;
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
//			cout <<endl <<"child process success and exit" << endl;
			pass=true;
		}
		else if(WEXITSTATUS(status)==EXIT_FAILURE){
//			cout << "child process not success and exit"<<endl;
			pass=false;
		}

	}


