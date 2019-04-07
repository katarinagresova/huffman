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
void storeHuffmanCodes(Node* root, string str, map<unsigned int, string> &huffmanCode) {
    if (root == nullptr)
        return;

    // found a leaf node
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    storeHuffmanCodes(root->left, str + "0", huffmanCode);
    storeHuffmanCodes(root->right, str + "1", huffmanCode);
}

//// traverse the Huffman Tree and decode the encoded string
//void decode(Node* root, int &index, string str) {
//    if (root == nullptr) {
//        return;
//    }
//
//    // found a leaf node
//    if (!root->left && !root->right)
//    {
//        cout << root->ch;
//        return;
//    }
//
//    index++;
//
//    if (str[index] =='0')
//        decode(root->left, index, str);
//    else
//        decode(root->right, index, str);
//}

//char finder with given Huffman string
//input : a Huffman string to traverse on the H. tree and
//        a u. char by ref. to copy the char found
//return: true if a char is found else false
void decode(Node* curr, string s, unsigned char & c) {
    for(unsigned int i=0;i<s.size();++i)
    {
        if(s[i]=='0') //go to left in the H. tree
            curr=curr->left;
        if(s[i]=='1') //go to right in the H. tree
            curr=curr->right;
    }

    //bool found=false;

    if(!curr->left && !curr->right) //if it is a leaf node
    {
        //found=true;
        c=curr->ch;
    }

    //return found;
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

//string createHeader(map<u_int8_t, int> *freq) {
//    for (int i=0; i<256; ++i) {
//        //output char freq table to the output file
//        //divide 32 bit u. int values into 4 bytes
//        outfile.put(static_cast<unsigned char>(f[i]>>24));
//        outfile.put(static_cast<unsigned char>((f[i]>>16)%256));
//        outfile.put(static_cast<unsigned char>((f[i]>>8)%256));
//        outfile.put(static_cast<unsigned char>(f[i]%256));
//    }
//

//}

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

    //open the output file
    ofstream outfile(outputFileName.c_str(), ios::out|ios::binary);
    if(!outfile) {
        cerr<<outputFileName<<" could not be opened!"<<endl;
        exit(1);
    }

    for (int i=0; i<256; ++i) {
        //output char freq table to the output file
        //divide 32 bit u. int values into 4 bytes
        outfile.put(static_cast<unsigned char>(freq[i]>>24));
        outfile.put(static_cast<unsigned char>((freq[i]>>16)%256));
        outfile.put(static_cast<unsigned char>((freq[i]>>8)%256));
        outfile.put(static_cast<unsigned char>(freq[i]%256));
    }

    unsigned int total_chars=(*tp).get_freq();
    cout<<"total chars to encode:"<<total_chars<<endl;


    ifstream infile(inputFileName.c_str(), ios::in|ios::binary);
    if(!infile)
    {
        cerr<<inputFileName<<" could not be opened!"<<endl;
        exit(1);
    }
    //output Huffman coded chars into the output file
    unsigned char ch2;
    while(infile.get(c))
    {
        ch=c;

        for () {
            static unsigned char c;
            if(i==1)
                c=c | (i<<(7-bit_pos)); //add a 1 to the byte
            else //i==0
                c=c & static_cast<unsigned char>(255-(1<<(7-bit_pos))); //add a 0
            ++bit_pos;
            bit_pos%=8;
            if(bit_pos==0)
            {
                outfile.put(c);
                c='\0';
            }
        }


//        //send the Huffman string to output file bit by bit
//        for(unsigned int i=0;i<H_table[ch].size();++i)
//        {
//            if(H_table[ch].at(i)=='0')
//                ch2=0;
//            if(H_table[ch].at(i)=='1')
//                ch2=1;
//            huf_write(ch2, outfile);
//        }
    }

    //send EOF
    outfile.put('\0');


    outfile.close();

    //string header = createHeader(&freq);

    return 0;
}

int decompressStatic(string inputFileName, string outputFileName, bool model) {
    ifstream infile(inputFileName.c_str(), ios::in|ios::binary);
    if(!infile)
    {
        cerr<<inputFileName<<" could not be opened!"<<endl;
        exit(1);
    }

    if(ifstream(outputFileName.c_str()))
    {
        cerr<<ofioutputFileNamele<<" already exists!"<<endl;
        exit(1);
    }

    //open the output file
    ofstream outfile(outputFileName.c_str(), ios::out|ios::binary);
    if(!outfile)
    {
        cerr<<outputFileName<<" could not be opened!"<<endl;
        exit(1);
    }


    //read frequency table from the input file
    map<u_int8_t, int> freq;
    char c;
    unsigned char ch;
    unsigned int j=1;
    for (int i = 0; i < 256; i++) {
        //read 4 bytes and combine them into one 32 bit u. int value
        freq[i]=0;
        for (int k=3; k >= 0; k--) {
            infile.get(c);
            ch=c;
            freq[i]+=ch*(j<<(8*k));
        }
    }

    Node* root = createHuffmanTree(&freq)->top();

    //read Huffman strings from the input file
    //find out the chars and write into the output file
    string st;
    unsigned char ch2;
    unsigned int total_chars=(*root).freq;
    cout<<"total chars to decode:"<<total_chars<<endl;
    while(total_chars>0) //continue until no char left to decode
    {
        st=""; //current Huffman string
        do
        {
            //read H. strings bit by bit
            ch=huf_read(infile);
            if(ch==0)
                st=st+'0';
            if(ch==1)
                st=st+'1';
        } //search the H. tree
        while(!(*tp).decode(st, ch2)); //continue until a char is found

        //output the char to the output file
        outfile.put(static_cast<char>(ch2));
        --total_chars;
    }

    infile.close();
    outfile.close();
}

