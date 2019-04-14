CC=g++
FLAGS=-std=c++11 -Wall -pedantic -Wextra
ARCHNAME="kko_xgreso00"

default: huff_codec

huff_codec:  main.o huffman_static.o huffman_adaptive.o file_manipulation.o model.o
	$(CC) $(CFLAGS) -o huff_codec main.o huffman_static.o huffman_adaptive.o file_manipulation.o model.o

huffman_static.o:  huffman_static.cpp
	$(CC) $(CFLAGS) -c huffman_static.cpp huffman_static.hpp

huffman_adaptive.o:  huffman_adaptive.cpp
	$(CC) $(CFLAGS) -c  huffman_adaptive.cpp huffman_adaptive.hpp

file_manipulation.o:  file_manipulation.cpp
	$(CC) $(CFLAGS) -c  file_manipulation.cpp file_manipulation.hpp

model.o:  ../model.cpp
	$(CC) $(CFLAGS) -c model.cpp model.hpp

main.o:  ../main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	$(RM) huff_codec $(ARCHNAME).zip *.o *~

zip:
	zip $(ARCHNAME).zip *.cpp *.hpp Makefile xgreso00.pdf