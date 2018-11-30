CC=g++ -std=c++17 -c
CFLAGS=-I./lib/


BIN_FOLDER=./bin/
SRC_FOLDER=./src/

DBFLAGS=-ggdb3 -O0 -g -Wall -Werror
RELEASEFLAGS=-O2

SRC=$(wildcard $(SRC_FOLDER)*.cpp)
OBJ=$(addprefix $(BIN_FOLDER),$(notdir $(SRC:.cpp=.o)))
EXE=./bin/main

all: $(OBJ)
	g++ -o $(EXE) $(OBJ)

./bin/%.o: ./src/%.cpp
	@mkdir -p $(BIN_FOLDER)
	$(CC) -o $@ $< $(CFLAGS) $(DBFLAGS)
clean:
	rm -f $(BIN_FOLDER)*.o
