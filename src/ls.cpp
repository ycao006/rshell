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
#include <termcap.h>

using namespace std;

// all the flags
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

// create colors for DIR EXCUTABLE HIDDEN
#define blue printf("\x1b[38;5;21m")
#define green printf("\x1b[38;5;46m")
#define gray printf("\x1b[48;5;245m")
#define original printf("\x1b[0m")


int checkFlags(const vector<string> &);
vector<string> collectFiles(const string & );
void printFiles(const string &, const vector<string> & , const int &, const int &);
void printFilesL(const string &, const vector<string> &, const int &, const vector<int> &);
void printFilesR(const string &, const vector<string> & , const int & , const int &);
int	numberOfDigits(const int & );
vector<int> findMax(const vector<string> & );
void printFileDetail(const string &, const string &, const int & );
bool checkDir(const string & , const string & );
bool checkEx(const string & , const string & );
bool checkRF(const string & , const string & );

struct stat buf;
struct stat buf1;
static char termbuf[2048];
int main(int argc, char** argv){

	//get terminal size
	char * termtype = getenv("TERM");
	if(tgetent(termbuf, termtype) < 0){
		perror("could not access termcap database. \n ");
	}
	char* terminalC= (char*)"co";
	int terminalWidth = tgetnum(terminalC);
	
	vector<string>arguments;
	vector<string>flags;
	vector<string>files;
	vector<string>testfiles;
	int isalR = 0x000;
	vector<int> maxNLen ;

	//store the inputs
	
	for(unsigned int i = 1 ; i < argc ; i++){

		if(argv[i][0] == '-'){
			flags.push_back(argv[i]);
		}
		else{
			arguments.push_back(argv[i]);	
		}
	}
	
	if((argc == 2)&&(arguments.size() == 1)){		
		bool exist=false;
		testfiles=collectFiles(arguments[0]);
		for(unsigned int i=0;i<testfiles.size();i++){
			if( arguments[0] == testfiles[i] ){
				exist = true;
				break;
			}
		}
		if(exist){
			maxNLen=findMax(arguments);
			if(checkRF(".",arguments[0])){
				printFileDetail(".",arguments[0],maxNLen[0]);
				return 0;
			}
		}
		
	}
	
	if((argc >1)&&(arguments.size()>0) ){
		
		// get the result of -a -l -R
		isalR = checkFlags(flags);
		// sort the argument . .. ./src etc in the vector arguments
		sort(arguments.begin(),arguments.end());
		if(arguments.size()>0){	
			for(unsigned int i=0; i<arguments.size(); i++){
				files = collectFiles (arguments[i]);
				sort(files.begin(),files.end());
				maxNLen=findMax(files);

				if(isalR == result_none){
					cout<<"none "<<endl;
					printFiles(arguments[i],files,isalR,terminalWidth);	
				}
				else if(isalR == result_a){
					cout<<"only a"<<endl;
					printFiles(arguments[i],files,isalR,terminalWidth);
				}
				else if(isalR == result_l){
					cout<<"only l 1"<<endl;
					printFilesL(arguments[i],files,isalR,maxNLen);
				}
				else if(isalR == result_R){
					cout<<"hello R"<<endl;
					printFilesR(arguments[i],files,isalR,terminalWidth);
				}

				else if(isalR == result_al){
					cout<<"a and l"<<endl;
					printFilesL(arguments[i],files,isalR,maxNLen);
				}
				else if(isalR == result_aR){
					cout<<"hello R"<<endl;
					printFilesR(arguments[i],files,isalR,terminalWidth);
				}
				else if(isalR == result_lR){
					cout<<"hello R"<<endl;
					printFilesR(arguments[i],files,isalR,terminalWidth);
				}
				else if(isalR == result_alR){
					cout<<"hello R"<<endl;
					printFilesR(arguments[i],files,isalR,terminalWidth);
				}
	
			}
			cout<<endl;
		}
	}

	if((argc <= 1)||(arguments.size()<=0)){
		arguments.push_back(".");
		files=collectFiles(arguments[0]);
		sort(files.begin(),files.end());
		isalR=checkFlags(flags);
		maxNLen=findMax(files);
		if(isalR == result_none){
			cout<<"none "<<endl;
			printFiles(arguments[0],files,isalR,terminalWidth);	
		}
		else if(isalR == result_a){
			cout<<"only a"<<endl;
			printFiles(arguments[0],files,isalR,terminalWidth);
		}
		else if(isalR == result_l){
			cout<<"only l"<<endl;
			printFilesL(arguments[0],files,isalR,maxNLen);
		}
		else if(isalR == result_R){
			cout<<"hello R"<<endl;
			printFilesR(arguments[0],files,isalR,terminalWidth);
		}
		else if(isalR == result_al){
			cout<<"a and l"<<endl;
			printFilesL(arguments[0],files,isalR,maxNLen);
		}
		else if(isalR == result_aR){
			cout<<"hello R"<<endl;
			printFilesR(arguments[0],files,isalR,terminalWidth);	
		}
		else if(isalR == result_lR){
			cout<<"hello R"<<endl;
			printFilesR(arguments[0],files,isalR,terminalWidth);
		}
		else if(isalR == result_alR){
			cout<<"hello R"<<endl;
			printFilesR(arguments[0],files,isalR,terminalWidth);
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
	if(NULL == (dirp = opendir(v1.c_str()))){
			perror("There was an error with opendir(). ");
			exit(1);
	
	}
	struct dirent *filespecs;
	errno = 0;
	while(NULL != (filespecs = readdir(dirp))){
				temp_files.push_back(filespecs->d_name);
			//	cout << filespecs->d_name << " ";
	}
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

vector<int> findMax(const vector<string> & files){
	int max=0;
	int total=0;
	vector<int> v1;
	for(unsigned int i=0; i<files.size(); i++){		
//  directory name(the one where we want to check files inside) +/+files[i].c_str()
		stat(files[i].c_str(), &buf);
		if(max<numberOfDigits(buf.st_size)){
			max=numberOfDigits(buf.st_size);
		}
		total=total+buf.st_blocks;
	}
	total=total/2;
	v1.push_back(max);
	v1.push_back(total);
	return v1;
}

void printFileDetail(const string & a1, const string & f1, 
			const int & max){
	string path=a1+"/"+f1; 
	stat(path.c_str(), &buf);
	struct passwd *pw = getpwuid(buf.st_uid);
	struct group *gr = getgrgid(buf.st_gid);
	struct tm * tm;
	// the reason to create path: ls -l need to get the file information from current dir
	char getTime[256];
	tm = localtime(&buf.st_mtime);
	strftime(getTime, sizeof(getTime),"%b %d %H:%M", tm);
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
	cout<< buf.st_nlink <<" ";
	cout<< pw->pw_name << " ";
	cout<< gr->gr_name << " ";
	cout<< setw(max) << buf.st_size<<" ";
	cout<<getTime<<" ";
	if(checkEx(a1,f1)){
		green;
	}
	if(checkDir(a1,f1)){
		blue;
	}
	if(f1[0]=='.'){
		gray;		
	}
	cout<<f1;
	original;
	cout<<" "<<endl;

}

void printFiles(const string & a1, const vector<string> & s1, 
				const int & flag , const int & terminalWidth){
	cout<<a1<<": "<<endl;
	int width=terminalWidth-16;
	int counter=0;
	if((flag & flag_a) == 0)
	{
		for(unsigned int i=0; i<s1.size(); i++){
			if(s1[i][0]!='.'){
				if(counter+s1[i].size() > width){
					cout<<endl;
					counter = s1[i].size();		
				}
				else{
					counter = counter+s1[i].size()+2;		
				} 

				if(checkEx(a1,s1[i])){
					green;
				}
				if(checkDir(a1,s1[i])){
					blue;
				}
				cout<<s1[i];
				original;
				cout<<"  ";
			}
		}
	}
	else{

		for(unsigned int i=0; i<s1.size(); i++){
			if(counter+s1[i].size() > width){
				cout<<endl;
				counter = s1[i].size();		
			}
			else{
				counter = counter+s1[i].size()+2;
			}
			if(checkEx(a1,s1[i])){
				green;
			}
			if(checkDir(a1,s1[i])){
				blue;
			}
			if(s1[i][0]=='.'){
				gray;		
			}
			cout<<s1[i];
			original;
			cout<<"  ";
		}
	}
	cout<<endl;
}



void printFilesL(const string & a1, const vector<string> & s1, 
				const int & flag, const vector<int> & max){

	cout<<a1<<":"<<endl;
//	cout<<"size of max"<<max.size()<<endl;
	cout<<"total "<<max[1]<<endl; 

	if((flag & flag_a)==0){
		for(unsigned int i=0 ; i<s1.size(); i++){
			if(s1[i][0] != '.'){
				printFileDetail(a1 , s1[i] , max[0]);
				cout<<"i: "<<i<<endl;	
			}
		}
	}
	else{
		for(unsigned int i=0 ; i<s1.size(); i++){
				printFileDetail(a1,s1[i],max[0]);
				cout<<"i: "<<i<<endl;
		}		
	}
}

bool checkDir(const string & a1, const string & f1){
	bool temp_flag = false;
	string path=a1 + "/" + f1;
	stat(path.c_str(), & buf1);
	if(S_ISDIR(buf1.st_mode)){
		temp_flag = true;	
	}
	return temp_flag;		
}

bool checkEx(const string & a1, const string & f1){
	string path=a1 + "/" + f1;
	stat(path.c_str(), & buf1);
	bool temp_flag = false;
	if(buf1.st_mode & S_IXUSR){
		temp_flag = true;	
	}
	return temp_flag;		
}

bool checkRF(const string & a1, const string & f1){
	string path=a1 + "/" + f1;
	stat(path.c_str(), & buf1);
	bool temp_flag = false;
	if(S_ISREG(buf1.st_mode)){
		temp_flag = true;	
	}
	return temp_flag;		
}

void printFilesR(const string &a1, const vector<string> & files, 
			    	const int & flag, const int & terminalWidth){
	vector<string> tempDirs;
	if( (flag & flag_l) == 0){
		// no l
		cout<<a1<<":"<<endl;
		printFiles(a1,files,flag,terminalWidth);
		cout<<endl;
		for(unsigned int i=0; i<files.size(); i++){
			if((flag & flag_a) != 0){
				//have a
				if(checkDir(a1,files[i])){
					if((files[i]!=".") && (files[i]!="..")){
						tempDirs.push_back((a1 + "/" + files[i]));			
					}
				}		
			}
			else{
				//no a
				if(files[i][0] != '.'){
				//do not check . file
					if(checkDir(a1,files[i])){
						if((files[i]!=".") && (files[i]!="..")){
							tempDirs.push_back((a1 + "/" + files[i]));			
						}
					}
				}
			}
		}
		for(unsigned int i=0; i<tempDirs.size(); i++){
			vector<string> tempFiles;
			tempFiles=collectFiles(tempDirs[i]);
			sort(tempFiles.begin(),tempFiles.end());
			printFilesR(tempDirs[i],tempFiles,flag,terminalWidth);
		}
	}

	else{
		// have l
		vector<int> maxNLen=findMax(files);
		cout<<a1<<":"<<endl;
		printFilesL(a1, files, flag, maxNLen);
		cout<<endl<<endl;
		for(unsigned int i=0; i<files.size(); i++){
			if((flag & flag_a) != 0){
				if(checkDir(a1,files[i])){
					if((files[i]!=".") && (files[i]!="..")){
						tempDirs.push_back((a1 + "/" + files[i]));			
					}
				}		
			}
			else{
				if(files[i][0] != '.'){
					if(checkDir(a1,files[i])){
						if((files[i]!=".") && (files[i]!="..")){
							tempDirs.push_back((a1 + "/" + files[i]));			
						}
					}
				}
			}
		}
		for(unsigned int i=0; i<tempDirs.size(); i++){
			vector<string> tempFiles;
			tempFiles=collectFiles(tempDirs[i]);
			sort(tempFiles.begin(),tempFiles.end());
			printFilesR(tempDirs[i],tempFiles,flag,terminalWidth);
		}	
	}		
}
