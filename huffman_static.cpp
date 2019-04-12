#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <iomanip>
#include <cctype>
#include "huffman_static.hpp"
#include "file_manipulation.hpp"
#include "model.hpp"

#define EOF_VAL 2

using namespace std;

/**
 * Creating new tree node
 * @param ch        character of current node
 * @param freq      frequency of character
 * @param left      pointer to Node on left
 * @param right     pointer to Node on right
 * @return
 */
Node_static* createNode(u_int8_t ch, unsigned int freq, Node_static *left, Node_static *right) {
    Node_static* node = new Node_static();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

/**
 * Comparison object to be used to order the heap
 */
struct comp {
    bool operator()(Node_static* l, Node_static* r) {
        // highest priority item has lowest frequency
        return l->freq > r->freq;
    }
};

/**
 * Finds character corresponding to given Huffman code
 * @param root  root of Huffman tree
 * @param s     Huffman code of character
 * @param c     reference where found character is copied
 * @return      true if a char is found, false otherwise
 */
bool get_huf_char(Node_static* root, string s, u_int8_t& c) {

    //we have only root
    if (!root->left && !root->right) {
        c = root->ch;
        return true;
    }

    Node_static* curr = root;
    for (char i : s) {
        //go to left in the H. tree
        if (i == '0') {
            curr = curr->left;
        }
        //go to right in the H. tree
        if (i == '1') {
            curr = curr->right;
        }
    }

    bool found = false;

    //if it is a leaf node
    if (!curr->left && !curr->right) {
        found = true;
        c = curr->ch;
    }

    return found;
}

/**
 * Creates a priority queue to store live nodes of Huffman tree
 * @param freq      array of frequencies for each 8b character
 * @return          priority queue
 */
priority_queue<Node_static*, vector<Node_static*>, comp>* createHuffmanTree(unsigned int* freq) {
    priority_queue<Node_static*, vector<Node_static*>, comp> *pq = new priority_queue<Node_static*, vector<Node_static*>, comp>();

    // Create a leaf node for each character and add it to the priority queue.
    for (unsigned int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            pq->push(createNode(i, freq[(u_int8_t) i], nullptr, nullptr));
        }
    }

    // do till there is more than one node in the queue
    while (pq->size() != 1) {
        // Remove the two nodes of highest priority lowest frequency) from the queue
        Node_static *left = pq->top(); pq->pop();
        Node_static *right = pq->top(); pq->pop();

        // Create a new internal node with these two nodes
        // as children and with frequency equal to the sum
        // of the two nodes' frequencies. Add the new node
        // to the priority queue.
        unsigned int sum = left->freq + right->freq;
        pq->push(createNode('\0', sum, left, right));
    }

    return pq;
}

/**
 * Traverse the Huffman tree and store Huffman codes in a map.
 * @param root          root of Huffman tree
 * @param str           current prefix of Huffman code
 * @param huffmanCode   map to store Huffman code for each character
 */
void storeHuffmanCodes(Node_static* root, string str, map<u_int8_t, string> &huffmanCode) {
    if (root == nullptr)
        return;

    // found a leaf node
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    storeHuffmanCodes(root->left, str + "0", huffmanCode);
    storeHuffmanCodes(root->right, str + "1", huffmanCode);
}

/**
 * Static Huffman encoder
 * @param ifile     path to input file
 * @param ofile     path to output file
 * @param model     flag, if model will be used
 */
void encoder_static(string ifile, string ofile, bool model) {
    FILE *fp_in, *fp_out;
    check_files(ifile, ofile, &fp_in, &fp_out);

    //array to hold frequency table for all ASCII characters in the file
    unsigned int f[256];
    for (int i = 0; i < 256; ++i) {
        f[i] = 0;
    }

    //read the whole file and count the ASCII char table frequencies
    u_int8_t value;
    while (fread(&value, sizeof(u_int8_t), 1, fp_in) > 0) {
        if (model) {
            value = modeling(value);
        }
        ++f[value];
    }
    rewind(fp_in);
    if (model) {
        modeling(0);
    }

    Node_static* root = createHuffmanTree(f)->top();

    map<u_int8_t , string> H_table;
    storeHuffmanCodes(root, "", H_table);

    for (int i = 0; i < 256; ++i) {
        //output char freq table to the output file
        //divide 32 bit u. int values into 4 bytes
        write_byte(static_cast<u_int8_t>(f[i]>>24), fp_out);
        write_byte(static_cast<u_int8_t>((f[i]>>16)%256), fp_out);
        write_byte(static_cast<u_int8_t>((f[i]>>8)%256), fp_out);
        write_byte(static_cast<u_int8_t>(f[i]%256), fp_out);
    }

    //output Huffman coded chars into the output file
    u_int8_t ch;
    while (fread(&value, sizeof(u_int8_t), 1, fp_in) > 0) {
        if (model) {
            value = modeling(value);
        }
        for (char i : H_table[value]) {
            if (i == '0') {
                ch = 0;
            }
            if (i == '1') {
                ch = 1;
            }
            write_bit(ch, fp_out);
        }
    }
    write_bit(EOF_VAL, fp_out);

    fclose(fp_in);
    fclose(fp_out);

}

/**
 * Static Huffman dencoder
 * @param ifile     path to input file
 * @param ofile     path to output file
 * @param model     flag, if model will be used
 */
void decoder_static(string ifile, string ofile, bool model) {
    FILE *fp_in, *fp_out;
    check_files(ifile, ofile, &fp_in, &fp_out);

    //read frequency table from the input file
    unsigned int f[256];
    u_int8_t ch;
    unsigned int j=1;
    for (int i = 0; i < 256; ++i) {
        //read 4 bytes and combine them into one 32 bit u. int value
        f[i]=0;
        for (int k = 3; k >= 0; --k) {
            ch = readByte(fp_in);
            f[i] += ch*(j<<(8*k));
        }
    }

    Node_static* root = createHuffmanTree(f)->top();

    //read Huffman strings from the input file
    //find out the chars and write into the output file
    string st;
    u_int8_t ch2;
    unsigned int total_chars=(*root).freq;
    while (total_chars > 0) { //continue until no char left to decode

        st=""; //current Huffman string
        do {
            //read H. strings bit by bit
            ch = readBit(fp_in);
            if (ch == 0)
                st += '0';
            if (ch == 1)
                st += '1';
        } //search the H. tree
        while(!get_huf_char(root, st, ch2)); //continue until a char is found

        //output the char to the output file
        if (model) {
            ch2 = demodeling(ch2);
        }
        write_byte(ch2, fp_out);
        --total_chars;
    }

    fclose(fp_in);
    fclose(fp_out);
}