MAKE = make
GCC = gcc
INCLUDE = include/
SRC = $(wildcard src/*.c)
MOVE = mv
EXC = ./bin/kregit
LIB = lib/
GIT = git
ADD = add .
COMMIT = commit -m
PUSH = push
REBASE = rebase
READ = @read -p
BIN = bin/*
LOG = log/*

all : 
	$(MAKE) compile -s
	
compile :
	$(GCC) -I $(INCLUDE) -c $(SRC)
	$(MOVE) *.o $(LIB)
	$(GCC) -o $(EXC) $(LIB)*.o

run :
	$(EXC)

git :
	$(READ) "Enter the message to set up the commit : " message; \
        $(GIT) $(ADD) && $(GIT) $(COMMIT) "$$message";
		$(GIT) $(REBASE) && $(GIT) $(PUSH);

clean :
	rm $(BIN) $(LIB)*

log :
	rm $(LOG)