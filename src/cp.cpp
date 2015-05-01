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
    
    struct stat sb;
    if(!(stat(argv[2], &sb)==-1)){
        cout << "file already exists" << endl;
        exit(1);
    }

    ifstream in(argv[1]);
    ofstream out(argv[2]);
    int fd1 = open(argv[1], O_RDWR);
    if(fd1 == -1){
        perror("open1");
        exit(1);
    }
    int fd2 = open(argv[2], O_RDWR | O_CREAT, 0777);
    if(fd2 == -1){
        perror("open2");
        exit(1);
    }
    
    char check1[] = "a";
    char check2[] = "b";
    char check3[] = "c";
    if(strcmp(argv[3], check1) == 0){
        char letter;
	Timer Time_1;
	double UT1, WT1, ST1;
	Time_1.start();
    
        if(!in.good()){
            cout << "Cannot read file.\n";
            exit(1);
        }
        while(in.good()){
            letter = in.get();
            out.put(letter);
        }
	Time_1.elapsedTime(WT1, UT1, ST1);
	cout << WT1 << " " << UT1 << " " << ST1 << endl;
    }
    else if(strcmp(argv[3], check2) == 0){
        char charbuff;
        void* buff = &charbuff;
	    Timer Time_1;
	    double UT1, WT1, ST1;
	    Time_1.start();
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
	    Time_1.elapsedTime(WT1, UT1, ST1);
	    cout << WT1 << " " << UT1 << " " << ST1 << endl;
    }
    
    else if(strcmp(argv[3], check3)==0){
        char charbuff2[BUFSIZ];
        void* buff2 = &charbuff2;

	    Timer Time_1;
	    double UT1, WT1, ST1;
	    Time_1.start();
        int s;
        while((s = read(fd1, buff2, BUFSIZ)) != 0)
        {
            if(s == -1){
                perror("read2");
                exit(1);
            }
            if(write(fd2, buff2, BUFSIZ) == -1){
                perror("write2");
                exit(1);
            }   

        }

       	Time_1.elapsedTime(WT1, UT1, ST1);
	    cout << WT1 << " " << UT1 << " " << ST1 << endl;
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
