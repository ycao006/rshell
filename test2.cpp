#include<iostream>
#include<string>
#include<cstring>

using namespace std;

int main(){
	string input;
	getline(cin,input);
	int pos=input.find('#');
	if(pos!=string::npos){
		string str=input.substr(0,pos);
		cout<<input<<endl;
		cout<<"str: "<<str<<endl;
	}

	return 0;

}


