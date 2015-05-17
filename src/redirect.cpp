#include <iostream>
#include <unistd.h>//library for fork and execvp
#include <stdio.h> //library for perror
#include <stdlib.h>//libray for exit
#include <sys/types.h>
#include <sys/wait.h>// library for wait
#include <array>
#include <string>
#include <cstring>
#include <string.h>
#include <ctype.h> //isdigit
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <functional>
#include <algorithm>
#include <math.h>
using namespace std;
void inputStore(char*[],char[],const char*);
int inputReform(string&, char*[]);
int connector_format(string&);
int connector2_format(string &);
int syscall(char**, bool&, int&);
int  IOredirection(char* str[]);
int piping(char** , bool& , int& , int savestdin = -10);

// int savestdin is a default parameter:
// default parameter works like following:
// if we never change the value of default parameter, 
// then the default parameter keeps its original value;
// if we change the value of default parameter into a new value; 
// then the default parameter will change to its new value.

struct IOpair{
	string connector;
	string file;	
};

int main(){

	while(1){
Origin:
		unsigned int maxCap=64;
//		unsigned int I=0;
		char* commandList[64];
		string input;
		string input2;
		int status;
		bool pass = true;
		int pos1 = 0;
		int pos2 = 0;
		char errorList[3] = {'|', '&', ';'};
		char errorList2[5] = {'|', '&', ';', '<', '>'};
		string connector2List[4] = {"<","<<<",">",">>"};
		string connector3List[7] = {"<","<<<",">",">>","||","&&",";"};
		bool errorFlag1 = false;
		char hostname[256];
		size_t hostname_len = 256;
		char* username;
		if(-1 == gethostname(hostname,hostname_len)){
			perror("There was an error with gethostname(). ");
			exit(1);
		}
		if((username = getlogin()) == NULL){
			perror("There was an erro with getlogin(). ");
			exit(1);
		}
		cout << username << "@" << hostname << " $ ";
		getline(cin,input);
		//check 4 different types of input conditions when input is a string
checkInput:
		if((input== "")||(input.at(0))=='#'){
			goto Origin;
		}
		// check the input at front
		if(input.size() == 1){
			for(unsigned int k=0 ; k<3 ; k++){
				if(input.at(0) == errorList2[k]){
					cout<<"syntax error with | & ; < > in the front"<<endl;
					goto Origin;
				}
			}
		}

		// check the back of input
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
		//
		//when we create a new pointer array, it is best to set all the pointers to null pointers
		//so that all the pointers do not point to wired places.
		//
		for (unsigned int I = 0; I < maxCap; ++I) {
			commandList[I] = new char[maxCap];
			for(unsigned int j=0; j<maxCap;j++){
				commandList[I][j]='\0';
			}
				
		}
		//convert string into cstring;
		//breaks the cstring into parts and store them into input array commandList;
		if(-1 == inputReform(input, commandList)){
			for (unsigned int I = 0; I < maxCap; ++I) {
				delete[] commandList[I];
				commandList[I]=0;
			}
			goto Origin;
		}
		
		// check different input conditions when inputs in cstring
		if(commandList[0][0] == '\0'){
			for (unsigned int I = 0; I < maxCap; ++I) {
				delete[] commandList[I];
				commandList[I]=0;
			}
			goto Origin;
		}
		if(commandList[1][0] == '\0'){
				for(unsigned int k=0 ; k<5 ; k++){
					if(commandList[0][0] == errorList2[k]){
						cout<<"syntax errors: || && ; < > at front"<<endl;
						for (unsigned int I = 0; I < maxCap; ++I) {
							delete[] commandList[I];
							commandList[I]=0;
						}
						goto Origin;
					}
				}
		}
		for(int k=0; k<3; k++){
			if(commandList[0][0] == errorList[k]){ //< > can at the front
				cout<< "syntax errors: || && ;  at front"<<endl;
				for (unsigned int I = 0; I < maxCap; ++I) {
					delete[] commandList[I];
					commandList[I]=0;
				}
				goto Origin;
			}
		}
	
		int i=0;
		while(commandList[i][0] != '\0'){
			for(unsigned j=0; j<4; j++){
				if(strcmp(commandList[i],connector2List[j].c_str()) == 0){
					if(commandList[i+1] == NULL){
						cout<<"wrong input 154"<<endl;
						for (unsigned int I = 0; I < maxCap; ++I) {
							delete[] commandList[I];
							commandList[I]=0;
						}
						goto Origin;
					}
					else{
						for(unsigned int k=0; k<7; k++){
							if(strcmp(commandList[i+1],connector3List[k].c_str()) == 0){
								cout<<"wrong input 163"<<endl;
								for (unsigned int I = 0; I < maxCap; ++I) {
									delete[] commandList[I];
									commandList[I]=0;
								}
								goto Origin;
							}
						}
					}	
				}
			}
			i++;
		}
	    i = 0;	
		while((commandList[i][0] != '\0')&&(!errorFlag1)){

			if(strcmp(commandList[i],"||") == 0){
				for(int k=0 ; k<3 ; k++){
					if(commandList[i+1][0] == errorList[k]){
						cout << "syntax errors related to || &&"<<endl;
						for (unsigned int I = 0; I < maxCap; ++I) {
							delete[] commandList[I];
							commandList[I]=0;
						}
						goto Origin;
					}
				}
				int j = 0;
				char* temp[64]={0};
				while(j < pos2-pos1){
					temp[j] = commandList[j+pos1];
					j++;
				}
				//exit program
				
				if(strcmp(temp[0],"exit") == 0){
					for (unsigned int I = 0; I < maxCap; ++I) {
						delete[] commandList[I];
						commandList[I]=0;
					}
					return 0;
				}
				if(-1 == piping(temp,pass,status)){
					for (unsigned int I = 0; I < maxCap; ++I) {
						delete[] commandList[I];
						commandList[I]=0;
					}

					goto Origin;
				}
				
				memset(temp, '\0', 64);	
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
						cout << "syntax errors related to || && ; < >"<<endl;
						for (unsigned int I = 0; I < maxCap; ++I) {
							delete[] commandList[I];
							commandList[I]=0;
						}		

						goto Origin;
					}
				}
				int j = 0;	
				char* temp[64]={0};
				while(j < pos2-pos1){
					temp[j] = commandList[j+pos1];
					j++;
				}
				if(strcmp(temp[0],"exit") == 0){
					for (unsigned int I = 0; I < maxCap; ++I) {
						delete[] commandList[I];
						commandList[I]=0;
					}

					return 0;
				}
				if(-1 == piping(temp,pass,status)){
					for (unsigned int I = 0; I < maxCap; ++I) {
						delete[] commandList[I];
						commandList[I]=0;
					}
					goto Origin;	
				}
				memset(temp, '\0', 64);
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
						cout << "syntax errors related to || && ; < >"<<endl;
						for (unsigned int I = 0; I < maxCap; ++I) {
							delete[] commandList[I];
							commandList[I]=0;
						}
						goto Origin;
					}
				}
				int j = 0;
				char* temp[64]={0};
				while(j < pos2-pos1){
					temp[j] = commandList[j+pos1];
					j++;
				}
				if(strcmp(temp[0],"exit") == 0){
					for (unsigned int I = 0; I < maxCap; ++I) {
						delete[] commandList[I];
						commandList[I]=0;
					}
					return 0;
				}
				if(-1 == piping(temp,pass,status)){
					for (unsigned int I = 0; I < maxCap; ++I) {
						delete[] commandList[I];
						commandList[I]=0;
					}
					goto Origin;		
				}
				memset(temp, '\0', 64);
				pos1=pos2+1;
			}
			i++;
			pos2=i;

			if(commandList[i][0]=='\0'){
				int j = 0;
				char* temp[64]={0};
				while(j < pos2-pos1){
					temp[j] = commandList[j+pos1];
					j++;
				}
				if(strcmp(temp[0],"exit") == 0){
					for (unsigned int I = 0; I < maxCap; ++I) {
						delete[] commandList[I];
						commandList[I]=0;
					}
					return 0;
				}
				if(-1 == piping(temp,pass,status)){
					for (unsigned int I = 0; I < maxCap; ++I) {
						delete[] commandList[I];
						commandList[I]=0;
					}
					goto Origin;		
				}
				memset(temp, '\0',64);
			}

		}

		for (unsigned int I = 0; I < maxCap; ++I) {
			if(commandList[I] != 0){
				delete[] commandList[I];
				commandList[I]=0;
			}
		}

	}
	return 0;

}

	//converted string into cstring;
	int inputReform(string& str, char*array[]){
		int	connector_Flag = 0;
		int connector2_Flag = 0;
		long unsigned int pos = str.find('#');
		char cstr[64]; //
		// ignore the string after #
		if(pos != string::npos){
			str = str.substr(0,pos);
		}
		
		connector_Flag = connector_format(str);
		connector2_Flag = connector2_format(str);
		strcpy(cstr , str.c_str());
		int i = 0;
		char *temp = strtok(cstr , " ");
		while(temp != NULL){
			strcpy(array[i], temp);
			i++;
			temp = strtok(NULL," ");
		}

		if((connector_Flag == -1) || (connector2_Flag == -1)){
			return -1;
		}
		else{
			return 0;
		}
	}

	//add " " in front of and behind "||" "&&". in addition replace ";" with " "
	int connector_format(string& str){
 		size_t found = 0;
		unsigned int strl = 0;
		string connectors[4] = {"||", "&&", "|", ";" };
		char errorCheck[3] = {'|' , '&', ';'};
		string Newconnectors[4] = {" || ", " && ", " | " ," ; "};

		for (unsigned int i=0; i<4; i++){
			found = 0;
			while(found != string::npos){
				found = str.find(connectors[i],found);

				if(found!=string::npos){//find a potential connectors.
					if(i == 2){ // find "|"
						if(found == 0){ // if | in the front
							cout<<"invalid connector | at front 280"<<endl;	
							return -1;
						}
						else if(str[found+1] == errorCheck[0]){
							//consider it is a ||, so espace this || 
							found = found + 2;
						}
						else if(str[found+1] == errorCheck[1]){ // |& is wrong
							cout<<"invalid connector |& 283"<<endl;
							return -1;
						}
						else if(str[found+1] == errorCheck[2]){ // |; is wrong
							cout<<"invalid connector 287"<<endl;
							return -1;	
						}
						else if(str[found-1] == errorCheck[1]){ // &| is wrong
							cout<<"invalid connector 287"<<endl;
							return -1;	
						}
						else if(str[found-1] == errorCheck[2]){ // ;| is wrong
							cout<<"invalid connector 287"<<endl;
							return -1;	
						}
						else{
							str.replace(found, connectors[2].length(), Newconnectors[2]);
							strl = str.length();
							if((found+3) < strl){
								found = found + 3;
							}
							else{
								break;		
							}		
						} 
					}
					
					else{ // find || && ;
						for(unsigned int j=0; j<3; j++){
							if(str[found+2] == errorCheck[j]){
								cout<<"invalid connectors 228"<<endl;
								return -1; // when input is wrong return -1;
							}
						}

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
		}

		return 0;
	}

	int syscall(char* argv[], bool& pass, int& status){
	//	int status;
		int pid = fork();
		if(pid == -1){
			perror("There was a error in fork line 19"); //syscall to output error
			exit(1);//if it was a error then we try to fix it
		}

		else if(pid == 0){
			if(-1 == IOredirection(argv)){
				return-1;
			}
			if( execvp(argv[0], argv) == -1){
				perror("there was an error with execvp()");
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
		else if(WEXITSTATUS(status) != 0){
			pass=false;
		}

		return 0;
	}

int connector2_format(string & str1){
	//check < <<<
	string connectors[4] = {"<" , "<<<" , ">" , ">>"};
	string Newconnectors[4] = {" < " , " <<< ", " > " , " >> "};
	for(unsigned int i=0; i<str1.size(); i++){
		 if (str1[i] == '<'){

			if((i+1) < str1.size()){ // check the 2nd bit
				
				if(str1[i+1] == '<'){ // if the 2nd bit is also '<'
					
					if((i+2) < str1.size()){// check the 3rd bit
	
						if(str1[i+2] == '<'){
							
							if((i+3) < str1.size()){
	
								if(str1[i+3] == '<' ){
									cout<<"wrong input"<<endl; // '<<<<' does not exit
									return -1;
								}
								else{
									//v2.push_back(i);		
									str1.replace(i,connectors[1].length(), Newconnectors[1]);
									i=i+4;
								}
									
							}
							else{
								cout<<"wrong input"<<endl; // connector in the end
								return -1;
							}							

						}
						else{
							cout<<"wrong input"<<endl; // '<<' does not exist		
							return -1;
						}			
					}

					else{
						cout<<"wrong input"<<endl;// '<<' does not exist		
						return -1;
					}		

				}

				else{// the connector is <
				//	v1.push_back(i);
					str1.replace(i, connectors[0].length(), Newconnectors[0]);
					i=i+2;
				}
			
			}
			else{
				cout<<"wrong input"<<endl; // connector in the end;		
				return -1;
			}
			
				 
		 }
	
	}
	
	// check > >> 2> 2>>
	for(unsigned int i=0; i<str1.size(); i++){
		
		if(str1[i] == '>'){
			
			if((i+1) < str1.size()){
				
				if(str1[i+1] == '>'){
					
					if((i+2)< str1.size() ){

						if(str1[i+2] == '>'){
							cout<<"wrong input"<<endl; // '>>>' does not exit;
							return -1;
						} 


						else{ //find >> 
							if(i >= 1){
								
									if(isdigit(str1[i-1]) != 0){ // there is a number before >
								
									if(i >= 2){  // find 2>>
								
										if(str1[i-2] == ' '){
											str1.insert(i+2," ");
											i=i+2;
										}
										else{ // something like file2>
											str1.replace(i,connectors[3].length(), Newconnectors[3]);
											i=i+2;
										}
									}
									else{ //2>> infront of command;
										str1.insert(0," ");
										str1.insert(i+3," ");
										i=i+2;	
									}
								}

								else{
									str1.replace(i,connectors[3].length(), Newconnectors[3]);
									i=i+2;
								}
						
							}
							else{ // find '>>' at front;
						    	str1.replace(i,connectors[3].length(), Newconnectors[3]);
								i=i+3;
							}
						}	
					
					}

					else{
						cout<<"wrong input"<<endl; // connector in the end;
						return -1;
					}
				}			

				else{
				//	v3.push_back(i);		//find '>'
					if(i>=1){
						if(isdigit(str1[i-1]) != 0){ // there is a number before >
							if(i >= 2){  // find 2>
								if(str1[i-2] == ' '){
									str1.insert(i+1," ");
									i=i+1;
								}
								else{ // something like file2>
									str1.replace(i,connectors[2].length(), Newconnectors[2]);
									i=i+2;
								}
							}
							else{ //2> infront of command;
								str1.insert(0," ");
								str1.insert(3," ");
								i=i+2;	
							}
						}

						else{
							str1.replace(i,connectors[2].length(), Newconnectors[2]);
							i=i+2;
						}
					
					}
					else{
						str1.replace(i,connectors[2].length(), Newconnectors[2]);
						i=i+2;
					}
				}
			}
				
			else{
				cout<<"wrong input >"<<endl; //connector in the end;		
				return -1;
			}

		}
	} 
	return 0;			
}
	

int IOredirection(char* str1[]){
	vector<IOpair>v1;
	vector<string>s1;
	unsigned int i=0;
	string temp1;
	while(str1[i] != NULL){
		temp1=str1[i];
		s1.push_back(temp1);
	i++;
	}
	for(i=0 ; i<s1.size() ; i++){ // check connectors: < <<< > >>
		if(strcmp(s1[i].c_str(),"<") == 0){
			IOpair temp;
			temp.connector = s1[i];
			temp.file = s1[i+1];
			v1.push_back(temp); 
			s1.erase(s1.begin()+i,s1.begin()+i+2);
			i = i-1;
		}
		else if(strcmp(s1[i].c_str(),"<<<") == 0){
			IOpair temp;
			temp.connector = s1[i];
			int symbol_count = 0;
			bool finish_flag = false;
			unsigned int j;
			for(j=i+1; ((j<s1.size()) && (!finish_flag)) ; j++){
			// if the the number of " is a odd number; or it is no the end of s1. 	
				for(unsigned m=0 ; m<s1[j].size() ; m++){
					if (s1[j][m] == '\"'){		
					symbol_count++;
					}
				}

				if( (symbol_count % 2) == 0){
					finish_flag = true;
				}
			}
			// after the above foor loop
			if(!finish_flag){ // if the number of " is still odd, it is an error
				cout<<" number of  \" is odd "<<endl;
				return -1;	
			}
			// if the number of " is even, we found the string
			else{
				j=j-1;
				for(unsigned int k=i+1; k<=j ; k++){
					temp.file=temp.file + " " +s1[k];
				}
				for(unsigned int k=0; k<temp.file.size(); k++){
					if(temp.file[k] == '\"'){
						temp.file.erase(temp.file.begin() + k);
						k=k-1;
					}	
				}
				v1.push_back(temp);
				s1.erase(s1.begin()+i,s1.begin()+j+1);
				i=i-1;
			}


		}
		else if(strcmp(s1[i].c_str(),">") == 0){
			IOpair temp;
			temp.connector = s1[i];
			temp.file = s1[i+1];
			v1.push_back(temp); 
			s1.erase(s1.begin()+i,s1.begin()+i+2);
			i = i-1;
		}
		else if(strcmp(s1[i].c_str(),">>") == 0){
			IOpair temp;
			temp.connector = s1[i];
			temp.file = s1[i+1];
			v1.push_back(temp); 
			s1.erase(s1.begin()+i,s1.begin()+i+2);
			s1[i].c_str();
			i = i-1;
		}
		// for check 2> 2>>
		else if(isdigit(s1[i][0]) != 0){ 
			if( (s1[i].size()==2) || (s1[i].size() == 3) ){
				if(s1[i][1] == '>'){
						IOpair temp;
						temp.connector = s1[i];
						temp.file = s1[i+1];
						v1.push_back(temp);
						s1.erase(s1.begin()+i,s1.begin()+i+2);
						i = i-1;
				}	
			}
		}

	} 
	// at this point
	// s1 contains the argument for execvp; v1 contains the IO redirection pairs.
	
		int fd;
		for(unsigned int i=0; i<v1.size(); i++){
			if(strcmp(v1[i].connector.c_str(),">") == 0){
				if(-1 == (fd = open(v1[i].file.c_str() , O_WRONLY | O_CREAT | O_TRUNC , 
							S_IRUSR | S_IWUSR ))){
					perror("There was an error with open(). ");
					exit(1);
				}
				if(-1 == dup2(fd,1)){
					perror("There was an error with dup2(). ");
					exit(1);
				}
			}

			else if(strcmp(v1[i].connector.c_str(),">>") == 0){
				if(-1 == (fd = open(v1[i].file.c_str() , O_WRONLY | O_CREAT | O_APPEND,
						S_IRUSR | S_IWUSR ))){
					perror("There was an error with open(). ");
					exit(1);
				}
				if(-1 == dup2(fd,1)){
					perror("There was an error with dup2(). ");
					exit(1);
				}
			}
			
			else if(strcmp(v1[i].connector.c_str(),"<") == 0){
				if(-1 == (fd = open(v1[i].file.c_str() , O_RDONLY ))){
					perror("There was an error with open(). ");
					exit(1);
				}
				if(-1 == dup2(fd,0)){
					perror("There was an error with dup2(). ");
					exit(1);
				}
			}
			
			else if( (isdigit(v1[i].connector[0]) != 0) && v1[i].connector[1] == '>' 
					&& (v1[i].connector.size()== 2) ){// 2>
				int tempFd;
				char tempChar[256];
				tempChar[0] = v1[i].connector[0];
				tempFd = atoi(tempChar);
				if(-1 == (fd = open(v1[i].file.c_str() , O_WRONLY | O_CREAT | O_TRUNC, 
						S_IRUSR | S_IWUSR))){
					perror("There was an error with open(). ");
					exit(1);
				}
				if(-1 == dup2(fd,tempFd)){
					perror("There was an error with dup2(). ");
					exit(1);
				}
			}

			else if( (isdigit(v1[i].connector[0]) != 0) && (v1[i].connector.size()== 3)
					 && v1[i].connector[1] == '>' && v1[i].connector[2] == '>'){//  2>>
				int tempFd;
				char tempChar[256];
				tempChar[0] = v1[i].connector[0];
				tempFd = atoi(tempChar);
				if(-1 == (fd = open(v1[i].file.c_str() , O_WRONLY | O_CREAT | O_APPEND, 
						S_IRUSR | S_IWUSR))){
					perror("There was an error with open(). ");
					exit(1);
				}
				if(-1 == dup2(fd,tempFd)){
					perror("There was an error with dup2(). ");
					exit(1);
				}
			}
			
			else if(strcmp(v1[i].connector.c_str(),"<<<") == 0){
				int fd1[2];
				
				if (-1 == pipe(fd1)){
					perror("There was an error with pipe(). ");
					exit(1);
				}
				
				v1[i].file = v1[i].file + "\n";
				
				if(-1 == (write(fd1[1], v1[i].file.c_str() , v1[i].file.size()+1))){
					perror("There was an error with write(). ");
					exit(1);
				}
				
				if(-1 == dup2(fd1[0],0)){
					perror("There was an error with dup2(). ");
					exit(1);
				}
				
				if(-1 == close(fd1[1])){
					perror("There was an error with close(). ");
					exit(1);	
				}
			
			}
		}	
	i=0;
	while(str1[i] != NULL){
		str1[i]='\0';
		i++;
	}
	for(unsigned int i=0;i<s1.size();i++){	
		str1[i]=const_cast<char*>(s1[i].c_str());
	}
	return 0;

}

int piping(char** str1, bool& pass, int& status, int savestdin){
	char* s1[1024];
	char* LHS[1024];
	char* RHS[1024];
	int i=0;
	while(s1[i] != 0){
		s1[i]=0;
		i++;
	}
	i=0;
	while(LHS[i] != 0){
		LHS[i]=0;
		i++;
	}
	i=0;
	while(RHS[i] != 0){
		RHS[i]=0;
		i++;
	}
	bool foundPipe = false;
	string pipeFlag ="|";
	
	i = 0;
	while(str1[i] != 0){
		if( strcmp(str1[i],pipeFlag.c_str()) == 0 ){
			foundPipe = true;
		}
		s1[i]=str1[i];
		i++;
	}
	if(!foundPipe){
		if(-1 == syscall(s1,pass,status)){
			return -1;
		}
	}
	else{
		i=0;
		while( strcmp(s1[i],pipeFlag.c_str()) != 0 ){
			LHS[i] = s1[i];
			i++;
		}
		i=i+1;//scape i
		int k=0 ;
		while(s1[i] != NULL){
			RHS[k] = s1[i];
			k++;
			i++;	
		}
		
		int fd[2];   // fd[0] is PIPE_READ; fd[1] is PIPE_WRITE
		if(pipe(fd) == -1){
			perror("There was an error with pipe(). ");
			exit(1);
		}
		int pid = fork();
		if(pid == -1){
			perror("There was an eror with fork(). ");
			exit(1);
		}
		else if(pid == 0){ // in the first child process
			if(-1 == dup2(fd[1],1)){ // instead of write to stdout; write to pipe
				perror("There was an error with dup2()");
				exit(1);
			}
			if(-1 == close(fd[0])){ // close read sied of pipe
				perror("There was an error with close()");
				exit(1);
			}
			if(-1 == syscall(LHS,pass, status)){ // call syscall to run command
				perror("There was an error with syscall()");	
				exit(1);
			}
			exit(0);
		}
		else if(pid > 0){ // first parent process
			if(savestdin == -10){
	// if we already save the original stdin, we do not want to save it everytime
	// since the 0 will refer to the the readside of pipe after the first pipe
	// if we savestdin = dup(0) again, we will not get the original stdin.
	// as a result, we only save the original stdin at the first time. 		
			if(-1 == (savestdin = dup(0))){ // save stdin for recovering later on
					perror("There was an error with dup()");
				}
			}
			if(-1 == dup2(fd[0],0)){// instead of read from stdin, read from pipe.
				perror("There was an error with dup2()");
				exit(1);
			}
			if(-1 == close(fd[1])){// close write side of pipe;
				perror("There was an error with close()");
				exit(1);
			}
			if(-1 == piping(RHS,pass,status,savestdin)){// recursively do pipe
				perror("There was an error with piping()");
				exit(1);
			}
			if(-1 == dup2(savestdin,0)){// restore stdin. 
				perror("There was an error with dup2()");
				exit(1);
			}	
			if(-1 == close(fd[0])){
	// close current pipe-read.
	// [cmd1]----pipe----[cmd2] close the pipe-read that is refer to the right side of the piple 
				perror("There was an error with dup2()");
				exit(1);
			}	
			if(-1 == wait(0)){ //wait for child process to finish
				perror("There was an error with wait()");
				exit(1);		
			}
		
		}
		return 0;
	}
return 0;
}









