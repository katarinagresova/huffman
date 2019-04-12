#
# This is an example Makefile for a countwords program.  This
# program uses both the scanner module and a counter module.
# Typing 'make' or 'make count' will create the executable file.
#

# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC=g++
FLAGS=-std=c++11 -Wall -pedantic -Wextra

# typing 'make' will invoke the first target entry in the file
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: huff_codec

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#
huff_codec:  main.o huffman_static.o huffman_adaptive.o file_manipulation.o
	$(CC) $(CFLAGS) -o huff_codec main.o huffman_static.o huffman_adaptive.o file_manipulation.o

huffman_static.o:  ../huffman_static.cpp
	$(CC) $(CFLAGS) -c ../huffman_static.cpp ../huffman_static.hpp

huffman_adaptive.o:  ../huffman_adaptive.cpp
	$(CC) $(CFLAGS) -c ../huffman_adaptive.cpp ../huffman_adaptive.hpp

file_manipulation.o:  ../file_manipulation.cpp
	$(CC) $(CFLAGS) -c ../file_manipulation.cpp ../file_manipulation.hpp

main.o:  ../main.cpp
	$(CC) $(CFLAGS) -c ../main.cpp


# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean:
	$(RM) huff_codec *.o *~