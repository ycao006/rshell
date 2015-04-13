// strings and c-strings
 #include <iostream>
 #include <cstring>
 #include <string>

using namespace std;

 int main (){
  string str ("Please split this sentence into tokens:ls  ls ls  ls -a   \n  ls ls    ls      ; || ls && ls     ls");

  char * cstr = new char [str.length()+1];
  strcpy (cstr, str.c_str());

         // cstr now contains a c-string copy of str

  char * p = std::strtok (cstr," ");
  while (p!=0)
 {
  std::cout << p << '\n';
  p = std::strtok(NULL," ");
 }

  delete[] cstr;
  return 0;
 }                           
