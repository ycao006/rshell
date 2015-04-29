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

#define flag_a 0x100
#define flag_l 0x010
#define flag_R 0x001
#define result_a 0x100
#define result_l 0x010
#define result_R 0x001
#define result_al 0x110
#define result_aR 0x101
#define result_lR 0x011
#define result_alR 0x111
#define result_none 0x000

int checkFlags(const vector<string> &);
vector<string> collectFiles(const string & );
void printFiles(const vector<string> & , const int &);
void printFilesL(const string &, const vector<string> &, const int &, const int &);
int	numberOfDigits(const int & );
int findMax(const vector<string> & );
void printFileDetail(const string &, const string &, const int & );

struct stat buf;
int main(int argc, char** argv){
	
	vector<string>arguments;
	vector<string>flags;
	vector<string>files;
	int isalR = 0x000;
	int maxNLen = 0;
	cout<<"argc: "<<argc<<endl;
	//store the inputs
	cout<<"arguments size: "<<arguments.size()<<endl;
	
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
	cout<<"arguments 2 size: "<<arguments.size()<<endl;

	if((argc >1)&&(arguments.size()>0) ){

		// get the result of -a -l -R
		isalR = checkFlags(flags);
		cout<<"isalR: "<<isalR<<endl;
		// sort the argument . .. ./src etc in the vector arguments
		sort(arguments.begin(),arguments.end());
		cout<<"arguments size: "<<arguments.size()<<endl;
		if(arguments.size()>0){	
			for(unsigned int i=0; i<arguments.size(); i++){
				files = collectFiles (arguments[i]);
				sort(files.begin(),files.end());
				maxNLen=findMax(files);

				if(isalR == result_none){
					cout<<"none "<<endl;
					printFiles(files,isalR);	
				}
				else if(isalR == result_a){
					cout<<"only a"<<endl;
					printFiles(files,isalR);
				}
				else if(isalR == result_l){
					cout<<"only l"<<endl;
					printFilesL(arguments[i],files,isalR,maxNLen);
				}
				else if(isalR == result_R){
					cout<<"hello R"<<endl;
				}
				else if(isalR == result_al){
					cout<<"a and l"<<endl;
					printFilesL(arguments[i],files,isalR,maxNLen);
				}
				else if(isalR == result_aR){
					cout<<"hello R"<<endl;
				}
				else if(isalR == result_lR){
					cout<<"hello R"<<endl;
				}
				else if(isalR == result_alR){
					cout<<"hello R"<<endl;
				}
	
			}
			cout<<endl;
		}
	}

	if((argc <= 1)||(arguments.size()<=0)){
		arguments.push_back(".");
		cout<<"pos1 is fine: "<<arguments[0]<<endl;
		files=collectFiles(arguments[0]);
		sort(files.begin(),files.end());
		cout<<"pos 2 is fine"<<endl;
		isalR=checkFlags(flags);
		cout<<"isalR: "<<isalR<<endl;
		if(isalR == result_none){
			cout<<"none "<<endl;
			printFiles(files,isalR);	
		}
		else if(isalR == result_a){
			cout<<"only a"<<endl;
			printFiles(files,isalR);
		}
		else if(isalR == result_l){
			cout<<"only l"<<endl;
			printFilesL(arguments[0],files,isalR,maxNLen);
		}
		else if(isalR == result_R){
			cout<<"hello R"<<endl;
		}
		else if(isalR == result_al){
			cout<<"a and l"<<endl;
			printFilesL(arguments[0],files,isalR,maxNLen);
		}
		else if(isalR == result_aR){
			cout<<"hello R"<<endl;
		}
		else if(isalR == result_lR){
			cout<<"hello R"<<endl;
		}
		else if(isalR == result_alR){
			cout<<"hello R"<<endl;
		}		
	}

	return 0;

}

int checkFlags(const vector<string>& v1){
	int temp_alR = 0x000;
	for(unsigned int i = 0; i < v1.size(); i++){
		for(unsigned int j = 0; j < v1[i].size(); j++){
			if(v1[i][j] == 'a'){
				temp_alR = temp_alR | flag_a;
			}
			if(v1[i][j] == 'l'){
				temp_alR = temp_alR | flag_l;
			}					
			if(v1[i][j] == 'R'){
				temp_alR = temp_alR | flag_R;
			}
		}	
	}
	return temp_alR;
}

vector<string> collectFiles(const string & v1){
	vector<string> temp_files;
	DIR *dirp;
	cout<<"cf1: "<<v1<<endl;
	if(NULL == (dirp = opendir(v1.c_str()))){
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

void printFileDetail(const string & a1, const string & f1, const int & max){

	struct passwd *pw = getpwuid(buf.st_uid);
	struct group *gr = getgrgid(buf.st_gid);
	struct tm * tm;
	// the reason to create path: ls -l need to get the file information from current dir
	string path=a1+"/"+f1; 
	char getTime[256];
	tm = localtime(&buf.st_mtime);
	strftime(getTime, sizeof(getTime),"%b %d %H:%M", tm);
	stat(path.c_str(), &buf);
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
	cout<< f1 <<endl;

}

void printFiles(const vector<string> & s1, const int & flag){
	if((flag & flag_a)==0){
		for(unsigned int i=0; i<s1.size(); i++){
			if(s1[i][0]!='.'){
				cout<<s1[i]<<"  ";
			}
		}
	}
	else{
		for(unsigned int i=0; i<s1.size(); i++){
			cout<<s1[i]<<"  ";
		}		
	}
	cout<<endl;
}



void printFilesL(const string & a1, const vector<string> & s1, 
				const int & flag, const int & max){
	cout<<"flag: "<<flag<<endl;
	cout<<"flag_a: "<<flag_a<<endl;
	cout<<"flag & flag_a: "<<(flag & flag_a)<<endl;
	if((flag & flag_a)==0){
		cout<<"printFilesL part1"<<endl;
		for(unsigned int i=0 ; i<s1.size(); i++){
			if(s1[i][0] != '.'){
				printFileDetail(a1 , s1[i] , max);
			}
		}
	}
	else{
		cout<<"printFilesL part2"<<endl;
		for(unsigned int i=0 ; i<s1.size(); i++){
				printFileDetail(a1,s1[i],max);
		}		
	}
}
