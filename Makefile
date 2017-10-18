
DIR_INC	= ./include/
DIR_SRC = ./src
DIR_OBJ	= ./build/

objs 	= main.o file.o util.o 
OBJS 	= $(addprefix $(DIR_OBJ), main.o file.o util.o)

CC		= g++-4.9
CFLAG	= -g -Wall -std=c++11
INC 	= -Iinclude

all : detector 

detector : $(objs) 
	$(CC) -o detector $(OBJS) -lboost_program_options

main.o : ./src/main.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/main.cpp -o $(DIR_OBJ)main.o

file.o : $(DIR_SRC)/file.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/file.cpp -o $(DIR_OBJ)file.o

util.o : $(DIR_SRC)/util.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/util.cpp -o $(DIR_OBJ)util.o 

clean : 
	rm -f detector ./build/*.o