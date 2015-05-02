#include "Timer.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <fcntl.h>
#include <string.h>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv){
    bool flag = false;
    char achar[] = "a";
    if(argc > 3){
        if((strcmp(achar, argv[3])) == 0){
	        flag = true;
        }
    }
    struct stat sb;
    if(!(stat(argv[2], &sb)==-1)){
        cout << "file already exists" << endl;
        exit(1);
    }

    ifstream in(argv[1]);
    ofstream out(argv[2]);
    int fd1 = open(argv[1], O_RDONLY);
    if(fd1 == -1){
        perror("open1");
        exit(1);
    }
    int fd2 = open(argv[2], O_WRONLY | O_CREAT, 0777);
    if(fd2 == -1){
        perror("open2");
        exit(1);
    }

    if(!flag){
        
        char charbuff2[BUFSIZ];
        void* buff2 = &charbuff2; 
        int s;
        while((s = read(fd1, buff2, BUFSIZ)) != 0)
        {
            if(s == -1){
                perror("read2");
                exit(1);
            }
            if(write(fd2, buff2, s) == -1){
                perror("write2");
                exit(1);
            } 

        }
              
    }
    else{
	cout << "CP Type : Wall Clock Time - User Time - System Time\n";

	char letter;
	Timer Time_1;
	double UT1, WT1, ST1;
	Time_1.start();
    
        if(!in.good()){
            cout << "Cannot read file.\n";
            exit(1);
        }

        while((in.get(letter))){
            out.put(letter);
        }

        

	Time_1.elapsedTime(WT1, UT1, ST1);
	cout << "get and put: " << WT1 << " - " << UT1 << " - " << ST1 << endl;

        char charbuff;
        void* buff = &charbuff;
	    Timer Time_2;
	    double UT2, WT2, ST2;
	    Time_2.start();
	    int t;
        while( (t = read(fd1, buff, 1)) != 0)
        {
	        if(t == -1){
		        perror("read");
		        exit(1);
	        }
            if(write(fd2, buff, 1) == -1){
                perror("write");
                exit(1);
            }
        }
	    Time_2.elapsedTime(WT2, UT2, ST2);
	    cout << "read and write: " << WT2 << " - " << UT2 << " - " << ST2 << endl;
    
   
        char charbuff2[BUFSIZ];
        void* buff2 = &charbuff2;

	    Timer Time_3;
	    double UT3, WT3, ST3;
	    Time_3.start();
        int s;
        while((s = read(fd1, buff2, BUFSIZ)) != 0)
        {
            if(s == -1){
                perror("read2");
                exit(1);
            }
            if(write(fd2, buff2, s) == -1){
                perror("write2");
                exit(1);
            }   

        }

       	Time_3.elapsedTime(WT3, UT3, ST3);
	    cout << "read and write with buffer: " << WT3 << " - " << UT3 << " - " << ST3 << endl;
	}
    

    in.close();
    out.close();

    if(-1 == close(fd1)){
        perror("close1");
        exit(1);
    }
    if(-1 == close(fd2)){
        perror("close2");
        exit(1);
    }
    return 0;
}
