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

int main(){
	
	int maxCap=2048;
	int I=0;
	char* commandList[maxCap];
	char  userInput[maxCap];
	cin.getline(userInput, maxCap);	
	inputStore(commandList, userInput," ");
	int i=0;
	while(commandList[i]!=0){
		cout<<commandList[i]<<endl;
		i++;	
	}		
}	

void inputStore(char* array[], char cstr[],const char* symbols){
	int i=0;
	array[i]=strtok(cstr,symbols);
	while(array[i]!=0){
		i++;
		array[i]=strtok(NULL,symbols);
	}
	return; 
}
