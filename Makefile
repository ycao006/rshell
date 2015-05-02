all: rshell ls | bin
	mv ./rshell ./bin
	mv ./ls ./bin
rshell: 
	g++ -Wall -Werror -ansi -pedantic -std=c++11 -o rshell ./src/main.cpp
main:
	g++ -std=c++11 main.cpp
test:
	g++ -std==c++11 test.cpp
main2:
	g++ -std=c++11 main2.cpp
main3:
	g++ -std=c++11 main3.cpp
ls: 
	g++ -Wall -Werror -ansi -pedantic src/ls.cpp -ltermcap -o ls 
bin:
	mkdir bin

