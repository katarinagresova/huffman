CC=g++
FLAGS=-std=c++11 -Wall -pedantic -Wextra

all: main
main:
		$(CC) $(FLAGS) -O3 main.cpp -o huffman
debug:
		$(CC) $(FLAGS) -ggdb3 main.cpp -o huffman

doc:
	pdflatex ahed.tex

clean:
	rm -rf huffman *.zip *.aux *.log *.out ../huffman_xgreso00.zip 2> /dev/null

pack: clean
	mv test.sh ../
	mv ahed.tex ../
	cd .. ; zip -r huffman_xgreso00.zip huffman --exclude *.git* 
	mv ../test.sh .
	mv ../ahed.tex .