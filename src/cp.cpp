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
    int fd2 = open(argv[1], O_RDWR|O_CREAT);
    if(fd2 == -1){
        perror("open2");
        exit(1);
    }
    
    char check1[] = "a";
    char check2[] = "b";
    char check3[] = "c";
    if(strcmp(argv[3], check1)== 0){
        char letter;
    
        if(!in.good()){
            cout << "Cannot read file.\n";
            exit(1);
        }
        while(in.good()){
            letter = in.get();
            out.put(letter);
        }
    }
    else if(strcmp(argv[3], check2)==0){
        char charbuff;
        void* buff = &charbuff;
        while(read(fd1, buff, 1))
        {
            if(!write(fd2, buff, 1)){
                perror("write");
                exit(1);
            }
        }
    }
    
    else if(strcmp(argv[3], check3)==0){
        char charbuff2;
        void* buff2 = &charbuff2;

        if(-1 == read(fd1, buff2, BUFSIZ)){
            perror("read2");
            exit(1);
        }
        if(!write(fd2, buff2,BUFSIZ)){
            perror("write2");
            exit(1);
        }
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
