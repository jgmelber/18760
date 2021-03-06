# A template C++ Makefile for your SAT solver.

# Optimizing flags
FLAGS=-w -m32 -Wno-deprecated -Wno-write-strings -O3

# List all the .o files you need to build here
OBJS=parser.o sat.o

# This is the name of the executable file that gets built.  Please
# don't change it.
EXENAME=760sat

# Compile targets
all: clean $(OBJS)
	g++ $(FLAGS) $(OBJS) -lz -o $(EXENAME)
parser.o: parser.cpp parser.h
	g++ $(FLAGS) -c parser.cpp
sat.o: sat.cpp parser.h clause.h ValliJGM.h
	g++ $(FLAGS) -c sat.cpp

# Add more compilation targets here



# The "phony" `clean' compilation target.  Type `make clean' to remove
# your object files and your executable.
.PHONY: clean
clean:
	rm -rf $(OBJS) $(EXENAME)
