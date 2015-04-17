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
void inputReform(string&);
void connector_format(string&);

int main(){
	
	string input;
	getline(cin,input);

	int maxCap=2048;
	int I=0;
	char* commandList[maxCap];
	char  userInput[maxCap]; 

	//convert string into cstring;
	=inputReform(input);

	//breaks the cstring into parts and store them into input array commandList;
	inputStore(commandList, userInput," ");

//display the input array
	int i=0;
	while(commandList[i]!=NULL){
		cout<<commandList[i]<<endl;
		i++;	
	}		
}
/*	
//breaks the string into parts and store them inthe char* array[];
void inputStore(char* array[], char cstr[],const char* symbols){
	int i=0;
	array[i]=strtok(cstr,symbols);
	while(array[i]!=0){
		i++;
		array[i]=strtok(NULL,symbols);
	}
	return; 
}
*/

//converted string into cstring;
char* inputReform(string str){
	int pos = str.find('#');
//	char* cstr=new char[2084];
	char* cstr=new char[str.length()+1];
	
	// ignore the string after #
	if(pos!=string::npos){
		str=str.substr(0,pos);
		cout<<"str: "<<str<<" str size:  "<<str.length()<<endl<<endl<<endl;
	}
	
	//add " " in front of and behind "||" "&&". in addition replace ";" with " "
	connector_format(str);

	//convert spring into cstring
	strcpy(cstr,str.c_str());
	
	int i=0;
	commandList[i]=strtok(cstr," ");
	while(commandList[i]!=0){
		i++;
		commandList[i]=strtok(NULL," ");
	}
	return ;
}

//add " " in front of and behind "||" "&&". in addition replace ";" with " "
void connector_format(string& str){
			
 	size_t found = 0;
	int strl = 0;
	string connectors[3] = {"||", "&&", ";"};
	string Newconnectors[3] = {" || ", " && ", "   "}; 
	
	for (unsigned int i=0; i<3; i++){
		found=0;
		cout<<connectors[i]<<endl;     
		while(found!=string::npos){
			cout<<"found0 "<<found<<endl;
			found = str.find(connectors[i],found);
			cout<<"found1 "<<found<<endl;
			
			if(found!=string::npos){
				str.replace(found , connectors[i].length() , Newconnectors[i]);
				strl=str.length();
				if((found+3)<strl){
					found=found+3;
				}
				else{
					break;
				}
			} 
			cout<<"found2 "<<found<<endl;
			cout<<"new str "<<str<<" new str size: "<<str.length()<<endl;
		}
	}
	return;					

}

