all: rshell cp
	mkdir bin
	mv ./rshell ./bin
	mv ./cp ./bin
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
testmain:
	g++ -Wall -Werror -ansi -pedantic -std=c++11 src/main.cpp
cp:
	g++ src/cp.cpp -Wall -Werror -ansi -pedantic -o cp
