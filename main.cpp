/**
 * Autor: Katarina Gresova, xgreso00
 * Datum: 01.04.2019
 * Name: main.cpp
 * Description: Entree file. Responsible for parsing command line arguments.
 */

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdio.h>

using namespace std;

static void help(char* argv[]) {
    cerr << "Usage: " << argv[0] << " -c/-d -h static/adaptive -i <ifile> -o <ofile> [-m]\n"
         << "Options:\n"
         << "\t-c\t\tApplication will compress input file\n"
         << "\t-d\t\tApplication will decompress input file\n"
         << "\t-h static\tChoose static Huffman code\n"
         << "\t-h adaptive\tChoose adaptive Huffman code\n"
         << "\t-m\t\tActivate model\n"
         << "\t-i <ifile>\tPath to input file\n"
         << "\t-o <ofile>\tPath to output file\n"
         << "\t-h\t\tShow this help message\n"
         << endl;
}

int main(int argc, char* argv[]) {

	if (argc < 8) {
		cerr << "Too few input arguments.\n";
		help(argv);
		exit(1);
	}

	bool compress = false;
	bool decompress = false;
	bool huffmanStatic = false;
	bool huffmanAdaptive = false;
	bool model = false;
	string inputFileName = "";
	string outputFileName = "";

	int c;
	while ((c = getopt (argc, argv, "cdh:i:o:m")) != -1)
    
    switch (c) {
    	case 'c':
    		compress = true;
    		break;
		case 'd':
			decompress = false;
			break;
		case 'h':
			if (string("static").compare(optarg) == 0) {
				huffmanStatic = true;
			} else if (string("adaptive").compare(optarg) == 0) {
				huffmanAdaptive = true;
			} else {
				help(argv);
				exit(1);
			}
			break;
        case 'i':
            inputFileName = optarg;
            break;
        case 'o':
            outputFileName = optarg;
            break;
        case 'm':
            model = true;
            break;
        default:
            help(argv);
			exit(1);
	}

	if (compress == decompress ||
		huffmanStatic == huffmanAdaptive ||
		inputFileName.empty() ||
		outputFileName.empty()) {
		cerr << "Invalid input arguments.\n";
		help(argv);
		exit(1);
	}

	FILE* inputFile = fopen(inputFileName.c_str(), "rb");
	FILE* outputFile = fopen(outputFileName.c_str(), "wb");

	if (huffmanAdaptive) {

	}

	fclose(inputFile);
	fclose(outputFile);
	exit(0);
}