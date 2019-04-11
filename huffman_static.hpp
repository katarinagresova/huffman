//
// Created by Katarina Gresova on 07/04/2019.
//

#ifndef HUFFMAN_HUFFMAN_HPP
#define HUFFMAN_HUFFMAN_HPP

#include <string>
using namespace std;

struct Node_static {
    u_int8_t ch;
    unsigned int freq;
    Node_static *left, *right;
};

void encoder_static(string ifile, string ofile, bool model);
void decoder_static(string ifile, string ofile, bool model);

#endif //HUFFMAN_HUFFMAN_HPP
