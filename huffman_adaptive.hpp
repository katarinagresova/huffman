//
// Created by Katarina Gresova on 11/04/2019.
//

#ifndef HUFFMAN_HUFFMAN_ADAPTIVE_H
#define HUFFMAN_HUFFMAN_ADAPTIVE_H

#include <string>
using namespace std;

void encoder_adaptive(FILE *fp_in, FILE *fp_out);
void decoder_adaptive(FILE *fp_in, FILE *fp_out);

struct Node_adaptive {
    u_int16_t ch;
    unsigned int weight;
    unsigned int order;
    Node_adaptive *left, *right, *parent;
};

#define NYT 257
#define SYMBOL_COUNT 256

#endif //HUFFMAN_HUFFMAN_ADAPTIVE_H
