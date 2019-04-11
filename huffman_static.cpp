#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <iomanip>
#include <cctype>
#include "huffman_static.hpp"

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
 * Adds bit tu 8b buffer and writes it to output file
 * @param i         bit to be written, or EOF
 * @param outfile   output file stream
 */
void huf_write(u_int8_t i, ofstream& outfile) {
    static int bit_pos = 0; //0 to 7 (left to right) on the byte block
    static u_int8_t c = '\0'; //byte block to write

    if (i != EOF_VAL) {
        if (i == 1) {
            c = c | (i << (7 - bit_pos)); //add a 1 to the byte
        } else {
            c = c & static_cast<unsigned char>(255 - (1 << (7 - bit_pos))); //add a 0
        }
        ++bit_pos;
        bit_pos %= 8;
        if (bit_pos == 0) {
            outfile.put(c);
            c = '\0';
        }
    } else {
        outfile.put(c);
    }
}

/**
 * Reads input stream and returns it bit by bit
 * @param infile    input file stream
 * @return          read bit of EOF
 */
u_int8_t huf_read(ifstream & infile) {
    static int bit_pos = 0; //0 to 7 (left to right) on the byte block
    static u_int8_t c = infile.get();

    u_int8_t i;

    i = (c>>(7-bit_pos))%2; //get the bit from the byte
    ++bit_pos;
    bit_pos %= 8;
    if (bit_pos == 0) {
        if (!infile.eof()) {
            c = infile.get();
        } else {
            i = EOF_VAL;
        }
    }

    return i;
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
    ifstream infile(ifile.c_str(), ios::in|ios::binary);
    if(!infile) {
        cerr << ifile << " could not be opened!" << endl;
        exit(1);
    }

    if(ifstream(ofile.c_str())) {
        cerr << ofile << " already exists!" << endl;
        exit(1);
    }

    //open the output file
    ofstream outfile(ofile.c_str(), ios::out|ios::binary);
    if(!outfile) {
        cerr << ofile << " could not be opened!" << endl;
        exit(1);
    }

    //input file was empty - do empty output and finish
    if (infile.peek() == ifstream::traits_type::eof()) {
        infile.close();
        outfile.close();
        return;
    }

    //array to hold frequency table for all ASCII characters in the file
    unsigned int f[256];
    for (int i = 0; i < 256; ++i) {
        f[i] = 0;
    }

    //read the whole file and count the ASCII char table frequencies
    u_int8_t value;
    char buf[sizeof(u_int8_t)];
    while (infile.read(buf, sizeof(buf))) {
        memcpy(&value, buf, sizeof(value));
        ++f[value];
    }

    infile.clear(); //clear EOF flag
    infile.seekg(0); //reset get() pointer to beginning

    Node_static* root = createHuffmanTree(f)->top();

    map<u_int8_t , string> H_table;
    storeHuffmanCodes(root, "", H_table);

    for (int i = 0; i < 256; ++i) {
        //output char freq table to the output file
        //divide 32 bit u. int values into 4 bytes
        outfile.put(static_cast<u_int8_t>(f[i]>>24));
        outfile.put(static_cast<u_int8_t>((f[i]>>16)%256));
        outfile.put(static_cast<u_int8_t>((f[i]>>8)%256));
        outfile.put(static_cast<u_int8_t>(f[i]%256));
    }

    //output Huffman coded chars into the output file
    u_int8_t ch;
    while (infile.read(buf, sizeof(buf))) {
        memcpy(&value, buf, sizeof(value));
        for (char i : H_table[value]) {
            if (i == '0') {
                ch = 0;
            }
            if (i == '1') {
                ch = 1;
            }
            huf_write(ch, outfile);
        }
    }
    huf_write(EOF_VAL, outfile);

    infile.close();
    outfile.close();

}

/**
 * Static Huffman dencoder
 * @param ifile     path to input file
 * @param ofile     path to output file
 * @param model     flag, if model will be used
 */
void decoder_static(string ifile, string ofile, bool model) {
    ifstream infile(ifile.c_str(), ios::in|ios::binary);
    if (!infile) {
        cerr << ifile << " could not be opened!" << endl;
        exit(1);
    }

    if(ifstream(ofile.c_str())) {
        cerr << ofile << " already exists!" << endl;
        exit(1);
    }

    ofstream outfile(ofile.c_str(), ios::out|ios::binary);
    if(!outfile) {
        cerr << ofile << " could not be opened!" << endl;
        exit(1);
    }

    //input file was empty - do empty output and finish
    if (infile.peek() == ifstream::traits_type::eof()) {
        infile.close();
        outfile.close();
        return;
    }

    //read frequency table from the input file
    unsigned int f[256];
    char c;
    u_int8_t ch;
    unsigned int j=1;
    for (int i = 0; i < 256; ++i) {
        //read 4 bytes and combine them into one 32 bit u. int value
        f[i]=0;
        for(int k=3;k>=0;--k)
        {
            infile.get(c);
            ch=c;
            f[i]+=ch*(j<<(8*k));
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
            ch = huf_read(infile);
            if (ch == 0)
                st += '0';
            if (ch == 1)
                st += '1';
        } //search the H. tree
        while(!get_huf_char(root, st, ch2)); //continue until a char is found

        //output the char to the output file
        outfile.put(static_cast<char>(ch2));
        --total_chars;
    }

    infile.close();
    outfile.close();

}