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
vector<int> findMax(const string &, const vector<string> & );
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
	unsigned int terminalWidth = tgetnum(terminalC);
	
	vector<string>arguments;
	vector<string>flags;
	vector<string>files;
	vector<string>testfiles;
	int isalR = 0x000;
	vector<int> maxNLen ;
	vector<string> regFiles; 
	bool noReg = true;
 	
	//store the inputs
	for(int i = 1 ; i < argc ; i++){
		int j=1;
		if(argv[i][0] == '-'){
			if( strcmp(argv[i],"-") == 0){
					cout<<"sorry invalid input."<<endl;	
					return 0;
				}

			while(argv[i][j] != '\0'){
				if((argv[i][j] == 'a')||(argv[i][j] == 'l')||(argv[i][j] == 'R')){
					flags.push_back(argv[i]);
				}
				else{
					cout<<"sorry invalid input."<<endl;
					return 0;
				}
				j++;
			}
		}
		else{
			
			arguments.push_back(argv[i]);
		}
			
		
		
	}
	if((argc >1)&&(arguments.size()>0) ){
		
		// get the result of -a -l -R
		isalR = checkFlags(flags);
		// sort the argument . .. ./src etc in the vector arguments
		sort(arguments.begin(),arguments.end());
			

		if(arguments.size()>0){	
			//check regular files
			for(unsigned int i=0; i<arguments.size(); i++){

				if(	-1 == stat(arguments[i].c_str(), & buf1)){
					perror("There was an error with stat(). ");
					arguments.erase(arguments.begin() + i);
					i--;					
				}
				if(!S_ISDIR(buf1.st_mode)){
					regFiles.push_back(arguments[i]);
					arguments.erase(arguments.begin()+i);
					i--;
					noReg = false;
				}
			}
			//output regular files
			if(regFiles.size() > 0){
				unsigned int regWord_width = 0;
				int regCounter = 0;
				for(unsigned int i=0; i<regFiles.size(); i++){
					if(regWord_width < regFiles[i].size()){
							regWord_width = regFiles[i].size();
							}
					}

				if((isalR & flag_l)== 0){
					for(unsigned int i=0; i<regFiles.size(); i++){
						if(regCounter+regWord_width > terminalWidth){
							cout << endl;
							regCounter = 0;
							regCounter = regWord_width + 2;		
						}
						else{
							regCounter = regCounter + regWord_width + 2;
						}
						if(checkEx(".",regFiles[i])){
							green;
						}
						if(checkDir(".",regFiles[i])){
							blue;
						}
						if(regFiles[i][0]=='.'){
							gray;		
						}
						cout << left <<setw(regWord_width)<<regFiles[i];
						original;
						cout<<"  ";
					}
				}
				else{
					vector<int> tempVrg=findMax(".", regFiles);
					int tempMaxrg=tempVrg[0];
					for(unsigned int i=0; i<regFiles.size(); i++){
						printFileDetail(".",regFiles[i],tempMaxrg);	
					}	
				}
				cout<<endl;
			}
			for(unsigned int i=0; i<arguments.size(); i++){
				files = collectFiles (arguments[i]);
				sort(files.begin(),files.end());
				
				maxNLen=findMax(arguments[i],files);
				
				if(isalR == result_none){
					printFiles(arguments[i],files,isalR,terminalWidth);	
				}
				else if(isalR == result_a){
					printFiles(arguments[i],files,isalR,terminalWidth);
				}
				else if(isalR == result_l){
					printFilesL(arguments[i],files,isalR,maxNLen);
				}
				else if(isalR == result_R){
					printFilesR(arguments[i],files,isalR,terminalWidth);
				}

				else if(isalR == result_al){
					printFilesL(arguments[i],files,isalR,maxNLen);
				}
				else if(isalR == result_aR){
					printFilesR(arguments[i],files,isalR,terminalWidth);
				}
				else if(isalR == result_lR){
					printFilesR(arguments[i],files,isalR,terminalWidth);
				}
				else if(isalR == result_alR){
					printFilesR(arguments[i],files,isalR,terminalWidth);
				}
	
			}
			cout<<endl;
		}
	}

	if( ((argc <= 1)||(arguments.size()<=0)) && noReg ){
		arguments.push_back(".");
		files=collectFiles(arguments[0]);
		sort(files.begin(),files.end());
		isalR=checkFlags(flags);
		maxNLen=findMax(arguments[0],files);
		if(isalR == result_none){
			printFiles(arguments[0],files,isalR,terminalWidth);	
		}
		else if(isalR == result_a){
			printFiles(arguments[0],files,isalR,terminalWidth);
		}
		else if(isalR == result_l){
			printFilesL(arguments[0],files,isalR,maxNLen);
		}
		else if(isalR == result_R){
			printFilesR(arguments[0],files,isalR,terminalWidth);
		}
		else if(isalR == result_al){
			printFilesL(arguments[0],files,isalR,maxNLen);
		}
		else if(isalR == result_aR){
			printFilesR(arguments[0],files,isalR,terminalWidth);	
		}
		else if(isalR == result_lR){
			printFilesR(arguments[0],files,isalR,terminalWidth);
		}
		else if(isalR == result_alR){
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
	}
	if(errno != 0){
			perror("There was an error with readdir(). ");
			exit(1);
	}
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

vector<int> findMax(const string &a1 , const vector<string> & files){
	int max=0;
	int total=0;
	vector<int> v1;
	string pathFind;
	for(unsigned int i=3; i<files.size(); i++){	
		pathFind = a1 + "/"+files[i];	
//  directory name(the one where we want to check files inside) +/+files[i].c_str()
		if(-1 == stat(pathFind.c_str(), &buf)){
			perror(" 308 There was an error with stat()");
			exit(1);	
		}
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
	if(-1 == stat(path.c_str(), &buf)){
		perror("316 There was an error with stat()");	
		exit(1);
	}
	struct passwd *pw ;
	errno = 0;
	pw = getpwuid(buf.st_uid);
	if(errno != 0){
		perror("There was an error with getpwuid()");
		exit(1);
	}
	struct group *gr ;
	errno = 0;
	gr = getgrgid(buf.st_gid);
	if(errno != 0){
		perror("There was an error with getpwuid()");
		exit(1);
	}
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
	unsigned int word_width=0;
	for(unsigned int i=0; i<s1.size(); i++){
			if(word_width < s1[i].size()){
				word_width = s1[i].size();
			}
		}
	cout<<a1<<": "<<endl;
	unsigned int width=terminalWidth-8;
	int counter=0;
	if((flag & flag_a) == 0)
	{
		for(unsigned int i=0; i<s1.size(); i++){
			if(s1[i][0]!='.'){
				if(counter+word_width > width){
					cout<<endl;
					counter = 0;
					counter = word_width + 2;		
				}
				else{
					counter = counter + word_width + 2;		
				} 

				if(checkEx(a1,s1[i])){
					green;
				}
				if(checkDir(a1,s1[i])){
					blue;
				}
				cout << left <<setw(word_width)<<s1[i];
				original;
				cout<<"  ";
			}
		}
	}
	else{
		for(unsigned int i=0; i<s1.size(); i++){
			if(counter+word_width > width){
				cout << endl;
				counter = 0;
				counter = word_width + 2;		
			}
			else{
				counter = counter + word_width + 2;
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
			cout << left <<setw(word_width)<<s1[i];
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
			}
		}
	}
	else{
		for(unsigned int i=0 ; i<s1.size(); i++){
				printFileDetail(a1,s1[i],max[0]);
		}		
	}
}

bool checkDir(const string & a1, const string & f1){
	bool temp_flag = false;
	string path=a1 + "/" + f1;
	if(-1 ==stat(path.c_str(), & buf1)){
		perror(" 459 There was an error with stat()");	
		exit(1);
	}
	if(S_ISDIR(buf1.st_mode)){
		temp_flag = true;	
	}
	return temp_flag;		
}

bool checkEx(const string & a1, const string & f1){
	string path=a1 + "/" + f1;
	if(-1 == stat(path.c_str(), & buf1) ){
		perror(" 471 There was an error with stat()");	
		exit(1);
	}
	bool temp_flag = false;
	if(buf1.st_mode & S_IXUSR){
		temp_flag = true;	
	}
	return temp_flag;		
}

bool checkRF(const string & a1, const string & f1){
	string path=a1 + "/" + f1;
	if(-1 == stat(path.c_str(), & buf1)){
		perror("493 There was an error with stat()");
		exit(1);
	}
	bool temp_flag = false;
	if(S_ISREG(buf1.st_mode)){
		temp_flag = true;	
	}
	return temp_flag;		
}

void printFilesR(const string &a1, const vector<string> & files, 
			    	const int & flag, const int & terminalWidth){
	vector<string> tempDirs;
	//test
//	cout<<"a1: "<<a1<<endl;
	

	if( (flag & flag_l) == 0){
		// no l
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
		vector<int> maxNLen;
		maxNLen=findMax(a1, files);
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
