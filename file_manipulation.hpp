#ifndef HUFFMAN_FILE_MANIPULATION_H
#define HUFFMAN_FILE_MANIPULATION_H

#include <iostream>
#include <string>
using namespace std;

#define EOF_VAL 2

void check_files(string ifile, string ofile, FILE **fp_in, FILE **fp_out);
int readBit(FILE *fp);
u_int8_t readByte(FILE *fp);
void write_bit(u_int8_t i, FILE *fp);
void write_byte(u_int8_t i, FILE *fp);

#endif //HUFFMAN_FILE_MANIPULATION_H
