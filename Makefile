
DIR_INC	= ./include/
DIR_SRC = ./src
DIR_OBJ	= ./build/

OBJS_PREPROC = $(addprefix $(DIR_OBJ), main_preprocess.o file.o preprocess.o util.o)

CC		= g++-4.9
CFLAG	= -g -Wall -std=c++11
INC 	= -Iinclude

all : preprocessor

preprocessor : $(OBJS_PREPROC) 
	$(CC) -o preprocessor $(OBJS_PREPROC) -lboost_program_options

$(DIR_OBJ)main_preprocess.o : $(DIR_SRC)/main_preprocess.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/main_preprocess.cpp -o $(DIR_OBJ)main_preprocess.o

$(DIR_OBJ)file.o : $(DIR_SRC)/file.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/file.cpp -o $(DIR_OBJ)file.o

$(DIR_OBJ)preprocess.o : $(DIR_SRC)/preprocess.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/preprocess.cpp -o $(DIR_OBJ)preprocess.o

$(DIR_OBJ)util.o : $(DIR_SRC)/util.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/util.cpp -o $(DIR_OBJ)util.o 

clean : 
	rm -f preprocessor ./build/*.o