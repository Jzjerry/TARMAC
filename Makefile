OBJS = edge.o vertex.o graph.o visitor.o interpreter.o
HEADER := $(patsubst %.o,%.h,$(OBJS))

# ROOT = ### The root path contains Z3
INCLUDE = 
# LIBRARY = $(ROOT)/lib

CC = g++
CFLAGS = -D DEBUG -O3 -I$(INCLUDE) -std=c++17 -Wall

all: tarmac

tarmac: main.cpp $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lz3

%.o: %.cpp $(HEADER)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f tarmac *.o
