
DIR_INC	= ./include/
DIR_SRC = ./src
DIR_OBJ	= ./build/

OBJS_PREPROC 	= $(addprefix $(DIR_OBJ), main_preprocess.o file.o preprocess.o liveness.o alivebuf.o alivefunc.o record.o node.o util.o)

OBJS_LIVENESS	= $(addprefix $(DIR_OBJ), main_liveness.o file.o preprocess.o liveness.o alivebuf.o alivefunc.o record.o node.o util.o)

OBJS_MERGER 	= $(addprefix $(DIR_OBJ), main_mergebuf.o file.o preprocess.o liveness.o alivebuf.o alivefunc.o record.o node.o util.o)

OBJS_CLEANMERGE = $(addprefix $(DIR_OBJ), main_cleanmerge.o file.o preprocess.o liveness.o alivebuf.o alivefunc.o record.o node.o util.o)

CC		= g++-4.9
CFLAG	= -g -Wall -std=c++11
LFLAG	= -g -Wall -std=c++11
INC 	= -Iinclude

all : preprocessor livenessor merger cleanmerge

preprocessor : $(OBJS_PREPROC) 
	$(CC) $(LFLAG) -o preprocessor $(OBJS_PREPROC) -lboost_program_options

livenessor : $(OBJS_LIVENESS)
	$(CC) $(LFLAG) -o livenessor $(OBJS_LIVENESS) -lboost_program_options

merger : $(OBJS_MERGER)
	$(CC) $(LFLAG) -o merger $(OBJS_MERGER) -lboost_program_options

cleanmerge : $(OBJS_CLEANMERGE)
	$(CC) $(LFLAG) -o cleanmerge $(OBJS_CLEANMERGE) -lboost_program_options

$(DIR_OBJ)main_preprocess.o : $(DIR_SRC)/main_preprocess.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/main_preprocess.cpp -o $(DIR_OBJ)main_preprocess.o

$(DIR_OBJ)main_liveness.o : $(DIR_SRC)/main_liveness.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/main_liveness.cpp -o $(DIR_OBJ)main_liveness.o

$(DIR_OBJ)main_mergebuf.o : $(DIR_SRC)/main_mergebuf.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/main_mergebuf.cpp -o $(DIR_OBJ)main_mergebuf.o

$(DIR_OBJ)main_cleanmerge.o : $(DIR_SRC)/main_cleanmerge.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/main_cleanmerge.cpp -o $(DIR_OBJ)main_cleanmerge.o

$(DIR_OBJ)file.o : $(DIR_SRC)/file.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/file.cpp -o $(DIR_OBJ)file.o

$(DIR_OBJ)preprocess.o : $(DIR_SRC)/preprocess.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/preprocess.cpp -o $(DIR_OBJ)preprocess.o

$(DIR_OBJ)liveness.o : $(DIR_SRC)/liveness.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/liveness.cpp -o $(DIR_OBJ)liveness.o

$(DIR_OBJ)alivebuf.o : $(DIR_SRC)/alivebuf.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/alivebuf.cpp -o $(DIR_OBJ)alivebuf.o

$(DIR_OBJ)alivefunc.o : $(DIR_SRC)/alivefunc.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/alivefunc.cpp -o $(DIR_OBJ)alivefunc.o

$(DIR_OBJ)record.o : $(DIR_SRC)/record.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/record.cpp -o $(DIR_OBJ)record.o

$(DIR_OBJ)node.o : $(DIR_SRC)/node.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/node.cpp -o $(DIR_OBJ)node.o

$(DIR_OBJ)util.o : $(DIR_SRC)/util.cpp
	$(CC) $(INC) $(CFLAG) -c $(DIR_SRC)/util.cpp -o $(DIR_OBJ)util.o 

.PHONY : clean
clean : 
	rm -f preprocessor livenessor merger ./build/*.o