#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <iomanip> // setw
#include <pwd.h> //user name
#include <grp.h> //group name
using namespace std;

int checkFlags(const vector<string> &);
vector<string> collectFiles(const vector<string> & );
void printFiles(const vector<string> & , const int &);
int	numberOfDigits(const int & );
int findMax(const vector<string> & );

void printFilesL(const string &, const int & );

struct stat buf;
int main(int argc, char** argv){
	
	vector<string>arguments;
	vector<string>flags;
	vector<string>files;
	const int flag_a = 0b100;
	const int flag_l = 0b010;
	const int flag_R = 0b001;
	int isalR;
	int maxNLen=0;
	cout<<"argc: "<<argc<<endl;


	if(argc >=1 ){
		for(unsigned int i = 1 ; i < argc ; i++){
			cout<<"i: "<<i<<endl;
			cout<<"argv[i]: "<<argv[i]<<endl;

			if(argv[i][0] == '-'){
				flags.push_back(argv[i]);
				cout<<"pos1 is fine"<<endl;
			}
			else{
				arguments.push_back(argv[i]);	
			}
		}
		cout<<"pos2 is fine"<<endl;
		isalR = checkFlags(flags);
		cout<<"pos3"<<endl;
		sort(arguments.begin(),arguments.end());
		cout<<"pos4"<<endl;
		cout<<"arguments size: "<<arguments.size()<<endl;
		if(arguments.size()>0){	
		files = collectFiles(arguments);
		cout<<"pos5"<<endl;
		sort(files.begin(),files.end());
		cout<<"pos6"<<endl;
		maxNLen=findMax(files);
		cout<<"pos7"<<endl;
		for(unsigned int i=0; i<files.size(); i++){
			if(isalR & flag_l){
				if(isalR & flag_a){	
					printFilesL(files[i],maxNLen);
				}
				else{
					if(files[i][0] != '.'){
						printFilesL(files[i],maxNLen);
					}
				}
			}
			else{
				if(isalR & flag_a){
					cout<<files[i]<<" ";	
				}
				else{
					if(files[i][0] != '.'){
						cout<<files[i]<<" ";		
					}
				}
			}
		}
		cout<<endl;
		}
	}

	if((argc <= 1)||(arguments.size()<=0)){
		DIR *dirp;
		if(NULL == (dirp = opendir("."))){
				perror("There was an error with opendir(). ");
				exit(1);
		}
		struct dirent *filespecs;
		errno = 0;

		while(NULL != (filespecs = readdir(dirp))){
			files.push_back(filespecs->d_name);	
		}
		sort(files.begin(),files.end());
		for(unsigned int i=0; i<files.size(); i++){
			if(files[i][0] != '.' ){
				cout <<files[i] << " ";
			}
		}
		if(errno != 0){
				perror("There was an error with readdir(). ");
				exit(1);
		}
		cout << endl;
		if(-1 == closedir(dirp)){
				perror("There was an error with closedir(). ");
				exit(1);
		}
		for(unsigned int i = 1 ; i < argc ; i++){
			if(argv[i][0] == '-'){
				flags.push_back(argv[i]);
			}
			else{
				arguments.push_back(argv[i]);	
			}
		}
	}

	return 0;

}

int checkFlags(const vector<string>& v1){
	int temp_alR = 0b000;
	for(unsigned int i = 0; i < v1.size(); i++){
		for(unsigned int j = 0; j < v1[i].size(); j++){
			if(v1[i][j] == 'a'){
				temp_alR = temp_alR | 0b100;
			}
			if(v1[i][j] == 'l'){
				temp_alR = temp_alR | 0b010;
			}					
			if(v1[i][j] == 'R'){
				temp_alR = temp_alR | 0b010;
			}
		}	
	}
	return temp_alR;
}

vector<string> collectFiles(const vector<string> & v1){
	vector<string> temp_files;
	DIR *dirp;
	cout<<"cf1"<<endl;
	cout<<"v1[0]: "<<v1[0]<<endl;
	if(NULL == (dirp = opendir(v1[0].c_str()))){
			perror("There was an error with opendir(). ");
			exit(1);
	
	}
	cout<<"cf2"<<endl;
	struct dirent *filespecs;
	errno = 0;
	cout<<"cf3"<<endl;
	while(NULL != (filespecs = readdir(dirp))){
				temp_files.push_back(filespecs->d_name);
			//	cout << filespecs->d_name << " ";
	}
	cout<<"cf4"<<endl;
	if(errno != 0){
			perror("There was an error with readdir(). ");
			exit(1);
	}
	//cout << endl;
	if(-1 == closedir(dirp)){
			perror("There was an error with closedir(). ");
			exit(1);
	}		
	return temp_files;
}



int  numberOfDigits(const int & n){
		unsigned int len=0;
		int temp=n;
		for(len=0;temp>0;len++){
			temp=temp/10;		
		}
		return len;		
}

int findMax(const vector<string> & files){
	int max=0;
	for(unsigned int i=0; i<files.size(); i++){		
//  directory name(the one where we want to check files inside) +/+files[i].c_str()
		stat(files[i].c_str(), &buf);
		if(max<numberOfDigits(buf.st_size)){
			max=numberOfDigits(buf.st_size);
		}
	}
	return max;
}

void printFilesL(const string & s1, const int & max){

	struct passwd *pw = getpwuid(buf.st_uid);
	struct group *gr = getgrgid(buf.st_gid);
	struct tm * tm;
	char getTime[256];
	tm = localtime(&buf.st_mtime);
	strftime(getTime, sizeof(getTime),"%b %d %H:%M", tm);
	stat(s1.c_str(), &buf);
	cout<< ((S_ISDIR(buf.st_mode))? "d" : "-");
	cout<< ((buf.st_mode & S_IRUSR)? "r" : "-"); 	
	cout<< ((buf.st_mode & S_IWUSR)? "w" : "-");
	cout<< ((buf.st_mode & S_IXUSR)? "x" : "-"); 
	cout<< ((buf.st_mode & S_IRGRP)? "r" : "-");
	cout<< ((buf.st_mode & S_IWGRP)? "w" : "-"); 
	cout<< ((buf.st_mode & S_IXGRP)? "x" : "-");
	cout<< ((buf.st_mode & S_IROTH)? "r" : "-"); 
	cout<< ((buf.st_mode & S_IWOTH)? "w" : "-");
	cout<< ((buf.st_mode & S_IXOTH)? "x" : "-"); 
	cout<<" ";
	cout<< pw->pw_name << " ";
	cout<< gr->gr_name << " ";
	cout<< setw(max) << buf.st_size<<" ";
	cout<<getTime<<" ";
	cout<< s1 <<endl;

}


