#include "huffman_static.h"
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <map>
using namespace std;

int errCode;

// A Tree node
struct Node {
    char ch;
    int freq;
    Node *left, *right;
};

// Function to allocate a new tree node
Node* getNode(char ch, int freq, Node* left, Node* right) {
    Node* node = new Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

// Comparison object to be used to order the heap
struct comp {
    bool operator()(Node* l, Node* r)
    {
        // highest priority item has lowest frequency
        return l->freq > r->freq;
    }
};

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void storeHuffmanCodes(Node* root, string str, map<u_int8_t, string> &huffmanCode) {
    if (root == nullptr)
        return;

    // found a leaf node
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

// traverse the Huffman Tree and decode the encoded string
void decode(Node* root, int &index, string str) {
    if (root == nullptr) {
        return;
    }

    // found a leaf node
    if (!root->left && !root->right)
    {
        cout << root->ch;
        return;
    }

    index++;

    if (str[index] =='0')
        decode(root->left, index, str);
    else
        decode(root->right, index, str);
}


int count_frequency(map<u_int8_t, int> *freq, string inputFileName) {

    ifstream inputFileStream;
    inputFileStream.open(inputFileName.c_str(), ios::binary);
    if (inputFileStream.fail()) {
        cerr << "Unable to open input file." << endl;
        return 1;
    }

    u_int8_t value;
    char buf[sizeof(u_int8_t)];
    while (inputFileStream.read(buf, sizeof(buf))) {
        memcpy(&value, buf, sizeof(value));
        (*freq)[value]++;
    }

    inputFileStream.close();
    return 0;
}

priority_queue<Node*, vector<Node*>, comp>* createHuffmanTree(map<u_int8_t, int> *freq) {
    // Create a priority queue to store live nodes of
    // Huffman tree;
    priority_queue<Node*, vector<Node*>, comp> *pq = new priority_queue<Node*, vector<Node*>, comp>();

    // Create a leaf node for each character and add it
    // to the priority queue.
    for (auto pair: *freq) {
        pq->push(getNode(pair.first, pair.second, nullptr, nullptr));
    }

    // do till there is more than one node in the queue
    while (pq->size() != 1) {
        // Remove the two nodes of highest priority
        // (lowest frequency) from the queue
        Node *left = pq->top(); pq->pop();
        Node *right = pq->top(); pq->pop();

        // Create a new internal node with these two nodes
        // as children and with frequency equal to the sum
        // of the two nodes' frequencies. Add the new node
        // to the priority queue.
        int sum = left->freq + right->freq;
        pq->push(getNode('\0', sum, left, right));
    }

    // root stores pointer to root of Huffman Tree
    return pq;
}

string createHeader(map<u_int8_t, int> *freq) {
    


}

int compressStatic(string inputFileName, string outputFileName, bool model) {

    // count frequency of appearance of each character
    // and store it in a map
    map<u_int8_t, int> freq;
    errCode = count_frequency(&freq, inputFileName);
    if (errCode != 0) {
        return errCode;
    }

    Node* root = createHuffmanTree(&freq)->top();

    // traverse the Huffman Tree and store Huffman Codes
    // in a map. Also prints them
    map<u_int8_t, string> huffmanCode;
    storeHuffmanCodes(root, "", huffmanCode);

    string header = createHeader(&freq);

    return 0;
}

int decompressStatic(string inputFileName, string outputFileName, bool model) {

}

