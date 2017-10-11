all : detector 

detector : main.o
	g++ -o detector ./build/main.o -lboost_program_options

main.o : ./src/main.cpp
	g++ -g -Wall -c ./src/main.cpp -o ./build/main.o 

clean : 
	rm -f detector /build/*.o