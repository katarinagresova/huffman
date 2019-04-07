#include<iostream>
#include<fstream>
#include<string>
#include <queue>
#include <map>
#include<iomanip> //for width()
#include<cctype> //for sprintf()

#define HELP_ERROR 99
#define width_unit 5

using namespace std;

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

//char finder with given Huffman string
//input : a Huffman string to traverse on the H. tree and
//        a u. char by ref. to copy the char found
//return: true if a char is found else false
bool get_huf_char(Node* root, string s, unsigned char & c)
{
    if(!root->left && !root->right) //we have only root
    {
        c=root->ch;
        return true;
    }

    Node * curr=root;
    for(unsigned int i=0;i<s.size();++i)
    {
        if(s[i]=='0') //go to left in the H. tree
            curr=curr->left;
        if(s[i]=='1') //go to right in the H. tree
            curr=curr->right;
    }

    bool found=false;

    if(!curr->left && !curr->right) //if it is a leaf node
    {
        found=true;
        c=curr->ch;
    }

    return found;
}

//the given bit will be written to the output file stream
//input : unsigned char i(:0 or 1 : bit to write ; 2:EOF)
void huf_write(unsigned char i, ofstream & outfile)
{
    static int bit_pos=0; //0 to 7 (left to right) on the byte block
    static unsigned char c='\0'; //byte block to write

    if(i<2) //if not EOF
    {
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
    else
    {
        outfile.put(c);
    }
}

//input : a input file stream to read bits
//return: unsigned char (:0 or 1 as bit read or 2 as EOF)
unsigned char huf_read(ifstream & infile)
{
    static int bit_pos=0; //0 to 7 (left to right) on the byte block
    static unsigned char c=infile.get();

    unsigned char i;

    i=(c>>(7-bit_pos))%2; //get the bit from the byte
    ++bit_pos;
    bit_pos%=8;
    if(bit_pos==0) {
        if (!infile.eof()) {
            c = infile.get();
        } else {
            i = 2;
        }
    }

    return i;
}

priority_queue<Node*, vector<Node*>, comp>* createHuffmanTree(unsigned int* freq) {
    // Create a priority queue to store live nodes of
    // Huffman tree;
    priority_queue<Node*, vector<Node*>, comp> *pq = new priority_queue<Node*, vector<Node*>, comp>();

    // Create a leaf node for each character and add it
    // to the priority queue.
    for (unsigned int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            pq->push(getNode(i, freq[(unsigned char)i], nullptr, nullptr));
        }
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

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void storeHuffmanCodes(Node* root, string str, map<unsigned char, string> &huffmanCode) {
    if (root == nullptr)
        return;

    // found a leaf node
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    storeHuffmanCodes(root->left, str + "0", huffmanCode);
    storeHuffmanCodes(root->right, str + "1", huffmanCode);
}

//Huffman Encoder
void encoder(string ifile, string ofile, bool verbose)
{
    ifstream infile(ifile.c_str(), ios::in|ios::binary);
    if(!infile)
    {
        cerr<<ifile<<" could not be opened!"<<endl;
        exit(1);
    }

    if(ifstream(ofile.c_str()))
    {
        cerr<<ofile<<" already exists!"<<endl;
        exit(1);
    }

    //open the output file
    ofstream outfile(ofile.c_str(), ios::out|ios::binary);
    if(!outfile)
    {
        cerr<<ofile<<" could not be opened!"<<endl;
        exit(1);
    }

    if (infile.peek() == ifstream::traits_type::eof()) {
        cout << "empty input" << endl;
        infile.close();
        outfile.close();
        return;
    }

    //array to hold frequency table for all ASCII characters in the file
    unsigned int f[256];
    for(int i=0;i<256;++i)
        f[i]=0;

    //read the whole file and count the ASCII char table frequencies
    char c;
    unsigned char ch;
    while(infile.get(c))
    {
        ch=c;
        ++f[ch];
    }

    infile.clear(); //clear EOF flag
    infile.seekg(0); //reset get() pointer to beginning

    Node* root = createHuffmanTree(f)->top();

    map<unsigned char, string> H_table;
    storeHuffmanCodes(root, "", H_table);

    for(int i=0;i<256;++i)
    {
        //output char freq table to the output file
        //divide 32 bit u. int values into 4 bytes
        outfile.put(static_cast<unsigned char>(f[i]>>24));
        outfile.put(static_cast<unsigned char>((f[i]>>16)%256));
        outfile.put(static_cast<unsigned char>((f[i]>>8)%256));
        outfile.put(static_cast<unsigned char>(f[i]%256));
    }

    unsigned int total_chars=(*root).freq;

    //output Huffman coded chars into the output file
    unsigned char ch2;
    while(infile.get(c))
    {
        ch=c;
        //send the Huffman string to output file bit by bit
        for(unsigned int i=0;i<H_table[ch].size();++i)
        {
            if(H_table[ch].at(i)=='0')
                ch2=0;
            if(H_table[ch].at(i)=='1')
                ch2=1;
            huf_write(ch2, outfile);
        }
    }
    ch2=2; // send EOF
    huf_write(ch2, outfile);

    infile.close();
    outfile.close();

} //end of Huffman encoder

//Huffman Decoder
void decoder(string ifile, string ofile, bool verbose)
{
    ifstream infile(ifile.c_str(), ios::in|ios::binary);
    if(!infile)
    {
        cerr<<ifile<<" could not be opened!"<<endl;
        exit(1);
    }

    if(ifstream(ofile.c_str()))
    {
        cerr<<ofile<<" already exists!"<<endl;
        exit(1);
    }

    //open the output file
    ofstream outfile(ofile.c_str(), ios::out|ios::binary);
    if(!outfile)
    {
        cerr<<ofile<<" could not be opened!"<<endl;
        exit(1);
    }


    if (infile.peek() == ifstream::traits_type::eof()) {
        cout << "empty input" << endl;
        infile.close();
        outfile.close();
        return;
    }

    //read frequency table from the input file
    unsigned int f[256];
    char c;
    unsigned char ch;
    unsigned int j=1;
    for(int i=0;i<256;++i)
    {
        //read 4 bytes and combine them into one 32 bit u. int value
        f[i]=0;
        for(int k=3;k>=0;--k)
        {
            infile.get(c);
            ch=c;
            f[i]+=ch*(j<<(8*k));
        }
    }

    Node* root = createHuffmanTree(f)->top();

    //read Huffman strings from the input file
    //find out the chars and write into the output file
    string st;
    unsigned char ch2;
    unsigned int total_chars=(*root).freq;
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
        while(!get_huf_char(root, st, ch2)); //continue until a char is found

        //output the char to the output file
        outfile.put(static_cast<char>(ch2));
        --total_chars;
    }

    infile.close();
    outfile.close();

} //end of Huffman decoder
