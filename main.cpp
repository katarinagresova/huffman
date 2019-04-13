/**
 * Author: Katarina Gresova, xgreso00
 * Datum: 03.04.2019
 * Name: main.cpp
 * Description: Entry file. Responsible for parsing command line arguments.
 */

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include "huffman_static.hpp"
#include "huffman_adaptive.hpp"

using namespace std;

/**
 * Prints help
 * @param argv  array of command line arguments
 */
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

/**
 * Entry point.
 * @param argc  count of command line arguments
 * @param argv  array of command line arguments
 * @return      0 on success, 1 otherwise
 */
int main(int argc, char* argv[]) {

	if (argc < 8) {
		cerr << "Too few input arguments.\n";
		help(argv);
		return 1;
	}

	bool compress = false;
	bool decompress = false;
	bool huffmanStatic = false;
	bool huffmanAdaptive = false;
	bool model = false;
	string inputFileName = "";
	string outputFileName = "";

	int c;
	while ((c = getopt (argc, argv, "cdh:i:o:mw")) != -1)
    
    switch (c) {
    	case 'c':
    		compress = true;
    		break;
		case 'd':
			decompress = true;
			break;
		case 'h':
			if (string("static") == optarg) {
				huffmanStatic = true;
			} else if (string("adaptive") == optarg) {
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
		case 'w':
			break;
        default:
            help(argv);
			return 1;
	}

	if (compress == decompress ||
		huffmanStatic == huffmanAdaptive ||
		inputFileName.empty() ||
		outputFileName.empty()) {
		cerr << "Invalid input arguments.\n";
		help(argv);
		exit(1);
	}

	if (huffmanAdaptive) {
        if (compress) {
            encoder_adaptive(inputFileName, outputFileName, model);
        } else {
            decoder_adaptive(inputFileName, outputFileName, model);
        }
	} else {
        if (compress) {
            encoder_static(inputFileName, outputFileName, model);
        } else {
            decoder_static(inputFileName, outputFileName, model);
        }
    }
	return 0;
}