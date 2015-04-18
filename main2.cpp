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

int main(){

	int maxCap=2048;
	int I=0;
	char* commandList[maxCap];
	char  userInput[maxCap]; 
	string input;
	getline(cin,input);

	for (I = 0; I < maxCap; ++I) {
		commandList[I] = new char[maxCap];
	}
	//convert string into cstring;
	//breaks the cstring into parts and store them into input array commandList;
	inputReform(input, commandList);
	cout << "inputReform is fine" << endl;
	//display the input array
	cout << "commandList includes: " << endl;
	int i=0;
	while(commandList[i]!=NULL){
		cout << commandList[i] << endl;
		i++;	
	}		
	
	return 0;

}

	//converted string into cstring;
	void inputReform(string& str, char*array[]){
		int pos = str.find('#');
		char cstr[2048]; // in my code at this point char* cstr does not work.
		const char* test;
		int i = 0;
		// ignore the string after #
		if(pos != string::npos){
			str = str.substr(0,pos);
			cout << "str: "<< str << " str size:  " << str.length() << endl << endl << endl;
		}
	
		connector_format(str);
		cout << "connector function is fine " << endl;
		test=str.c_str();//test
		cout << "c_str function is fine " << endl;
		strcpy(cstr , str.c_str());
		cout << "strcpy function is fine" << endl;
		array[i] = strtok(cstr , " ");
		while(array[i] != NULL){
			i++;
			array[i] = strtok(NULL," ");
		}
		i=0;
		cout << "stroke function is fine"<<endl;
		while(array[i] != NULL){
			cout<<" i: "<<i<<" array[i] "<<array[i]<<endl;		
			i++;
		}
		return;
	}

	//add " " in front of and behind "||" "&&". in addition replace ";" with " "
	void connector_format(string& str){		
 		size_t found = 0;
		int strl = 0;
		string connectors[3] = {"||", "&&", ";"};
		string Newconnectors[3] = {" || ", " && ", " ; "}; 
	
		for (unsigned int i=0; i<3; i++){
			found = 0;
			cout << connectors[i] << endl;     
			while(found != string::npos){
				cout << "found0 " << found << endl;
				found = str.find(connectors[i],found);
				cout << "found1 " << found << endl;
				
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
				
				cout << "found2 " << found << endl;
				cout << "new str " << str << " new str size: " << str.length() << endl;
			}
		}

		return;					
	}

