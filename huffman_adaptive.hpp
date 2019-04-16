/**
 * Author: Katarina Gresova, xgreso00
 * Datum: 12.04.2019
 * Name: huffman_adaptive.hpp
 * Description: Adaptive Huffman coding.
 */

#ifndef HUFFMAN_HUFFMAN_ADAPTIVE_H
#define HUFFMAN_HUFFMAN_ADAPTIVE_H

#include <string>
using namespace std;

void encoder_adaptive(string ifile, string ofile, bool model);
void decoder_adaptive(string ifile, string ofile, bool model);

struct node {
    struct node *parent;
    struct node *left_child;
    struct node *right_child;

    u_int8_t symbol;
    int weight;
    int order;
};
typedef struct node Node;

#define SYMBOLS_COUNT 256

#endif //HUFFMAN_HUFFMAN_ADAPTIVE_H
