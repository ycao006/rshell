#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sstream>
#include <cstring>

using namespace std;

bool isD=false;
int main(int argc , char* argv[]){
	
	struct stat s;

	if(argc < 3){
		cout<<"not enough argument."<<endl;		
		return 0;
	}	
	
	struct stat s1;
	if((stat(argv[1],&s1))== -1){
		perror("stat2");
		exit(EXIT_FAILURE);
	}

	if(S_ISDIR(s1.st_mode)){
		cout<<"wrong input"<<endl;
		return 0;
	}

	if(stat(argv[2],&s) < 0){
		if(link(argv[1],argv[2])<0){
			perror("link");
		}
		if(unlink(argv[1]) != 0){
			perror("unlink");		
		}	
	}	

	else if(S_ISDIR(s.st_mode)){	
		string path;
		path = string(argv[2])+"/"+string(argv[1]);
		link(argv[1],path.c_str());		
		if(unlink(argv[1]) != 0){
			perror("unlink");		
		}
	}
	else{
		cout<<"wrong input 1"<<endl;
		return 0;
	}
	

return 0;	
}


