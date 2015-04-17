#include <iostream>       // std::cout
#include <string>         // std::string

using namespace std;

int main ()
{
	int strl=0;
	size_t found=0;

	string str ("0123||45 78|||abcdefg");
	string str2 ("||");
	cout<< str << " strating str size: "<<str.length()<<endl<<endl<<endl;	
 	
 	/*
	while(found<strl){
		found = str.find(str2,found);
		cout<<" found1 "<<found <<endl;
		
		if (found!=string::npos){
			cout << " '||' found at: " << found <<endl;	
			str.replace(found,str2.length()," ? ");
			found=found+2;
			strl=str.length();
			cout<<" found2 "<<found<<endl;
		}
		found=found+1;
		cout << str << " str size: "<<str.length()<<endl;
		cout<<" found3: "<<found<<endl<<endl;
	}
	*/
	
	while(found!=string::npos){
		
		cout<<"found0 "<<found<<endl;
		found = str.find("||",found);
		cout<<"found1 "<<found<<endl;
		
		if(found!=string::npos){			
			
			str.replace(found , str2.length() , " || ");
			strl=str.length();
			if((found+3)<strl){
				found=found+3;
			}
			else{
				break;
			}
		
		}

		cout<<"found2 "<<found<<endl;
		cout<<"new str "<<str<<" new str size: "<<str.length()<<endl;
	
	}

	return 0;

}
