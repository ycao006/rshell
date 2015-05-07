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

bool isR = false;
using namespace std;

void rmfile(const char*);
void rmoveDir(const char*);
int isDirectoryEmpty(const char* );

int main(int argc , char* argv[]){
	struct stat s;
	vector<char*> dir;
	vector<char*> file;
	if(argc < 2){
	cout<<"not enough argument."<<endl;		
	}
	
	for(unsigned int i=0; i<argc; i++){
		string temp =argv[i];
		string R = "-r";
		if(strcmp(temp.c_str(),R.c_str()) == 0){
			isR = true;
		}
	}

	for(unsigned int i=1; i<argc; i++){
		string temp =argv[i];
		string R = "-r";
		if(strcmp(temp.c_str(),R.c_str()) != 0){
			if(stat(argv[i], &s) == -1){
				perror("stat");
				exit(EXIT_FAILURE);
			}
			if(S_ISDIR(s.st_mode) && isR){
				rmoveDir(argv[i]);
			}
			else if(S_ISDIR(s.st_mode) && (!isR)){
				cout<<"can not remove direcoty."<<endl;
			}
			else if(s.st_mode & S_IFREG){
				rmfile(argv[i]);
			}

			else{
				cout<<"invalid input"<<endl;
				exit(1);
			}	
		}
	}
	
}

void rmfile(const char* fileName){
	int status;
	errno = 0;
	if(fileName[0] != '.'){
		status = unlink(fileName);
		if(status != 0){
			perror("unlink");		
		}
	}
	return ;	
}

void rmoveDir(const char* dirName){
		if(isDirectoryEmpty(dirName)){
			if(-1 == (rmdir (dirName))){
				perror("rmdir");
				exit(1);
			};		
		}
		else{
		DIR* dirp;
		struct stat s2;
		if(NULL == (dirp = opendir(dirName))){
			perror("opendir");
			exit(1);		
		}
		struct dirent *dirFiles;
		while((dirFiles = readdir(dirp)) != NULL){
			if( dirFiles->d_name[0] != '.'){
				string path; 
				path =string(dirName)+"/"+string(dirFiles->d_name);
				if(stat(path.c_str(),&s2) == -1){
					perror("stat");
					exit(EXIT_FAILURE);
				}
				if(S_ISDIR(s2.st_mode)){
					rmoveDir(path.c_str());
				}
				else if(s2.st_mode & S_IFREG){
					rmfile(path.c_str());
		 		}					
			}
		}
		closedir(dirp);
		
		if(isDirectoryEmpty(dirName)){
			if(-1 == (rmdir (dirName))){
				perror("rmdir");
				exit(1);
			};		
		}
		}
}

int isDirectoryEmpty(const char* dirName){
	int n =0;
	struct dirent *d;
	DIR *dir = opendir(dirName);
	while((d = readdir(dir)) != NULL){
			if(++n > 2){
				break;
			}
	}
	closedir(dir);
	if(n <= 2){
	return 1;	
	}
	else{
	return 0;
	}

}



