CC = gcc
MPICC = mpicc
CXX = g++
MPICXX = mpicxx
CFLAGS = -Wall -Wextra -Wno-unused-result -Wno-unused-parameter -Wno-unused-but-set-variable -O3
CXXFLAGS = $(CFLAGS) -std=c++11

all:	main

main:	main.o generator.o sort.o merge.o util.o
	$(MPICXX) $(CFLAGS) -o sort $^

%.o:	%.cc
	$(MPICXX) $(CXXFLAGS) -c $<

clean:
	rm -f *o
	rm -f sort
