#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc, char** argv){
	cout<<argc<<endl;
	for (int i=0; i<argc; i++){
		
		cout<< argv[i]<<" "<<endl;
	}
	//int i=fork();
	//if(i=0){
	//cout<< "hello world"<<"i=0"<<endl;
	//}	
    //else {
	//cout<<"hello world"<<" i is not 0"<<endl;
	//}
	return 0;
}

