//#include <iostream>
//#include <fstream>
//#include <string>
//#include <unitypes.h>
//#include <map>
//#include <stack>
//#include "huffman_adaptive.hpp"
//
//void createRootNode(Node_adaptive** node, map<u_int16_t, Node_adaptive*>& symbol_array) {
//    *node = new Node_adaptive();
//
//    (*node)->ch = NYT;
//    (*node)->weight = 0;
//    (*node)->order = SYMBOL_COUNT * 2;
//    (*node)->left = nullptr;
//    (*node)->right = nullptr;
//    (*node)->parent = nullptr;
//
//    //symbol_array[NYT] = *node;
//}
//
////Node_adaptive* createNewNode(map<u_int16_t, Node_adaptive*>& symbol_array, u_int8_t symbol) {
////
////    Node_adaptive* NYT_node = symbol_array[NYT];
////
////    Node_adaptive* left = new Node_adaptive();
////    left->parent = NYT_node;
////    left->left = nullptr;
////    left->right = nullptr;
////    left->ch = NYT;
////    left->weight = 0;
////    left->order = NYT_node->order - 2;
////
////    Node_adaptive* right = new Node_adaptive();
////    right->parent = NYT_node;
////    right->left = nullptr;
////    right->right = nullptr;
////    right->ch = symbol;
////    right->weight = 1;
////    right->order = NYT_node->order - 1;
////
////    NYT_node->left = left;
////    NYT_node->right = right;
////   // NYT_node->ch = 256;
////
////    symbol_array[NYT] = left;
////    symbol_array[symbol] = right;
////    symbol_array[SYMBOL_COUNT + NYT_node->order/2] = NYT_node;
////
////    return NYT_node;
////}
//
///* Adds a new symbol in the tree and in the array of symbols
// * symbol: the symbol that will be inserted
// * zeroNode: reference to where the node ZERO points to.
// * symbols: the array of symbols
// * returns: the leaf node's (that contains the added symbol) parent.
// */
//Node_adaptive* createNewNode(map<u_int16_t, Node_adaptive*>& symbol_array, u_int8_t symbol, Node_adaptive** NYT_node) {
//
//    Node_adaptive* left = new Node_adaptive();
//    left->parent = *NYT_node;
//    left->left = nullptr;
//    left->right = nullptr;
//    left->ch = NYT;
//    left->weight = 0;
//    left->order = (*NYT_node)->order - 2;
//
//    Node_adaptive* right = new Node_adaptive();
//    right->parent = *NYT_node;
//    right->left = nullptr;
//    right->right = nullptr;
//    right->ch = symbol;
//    right->weight = 1;
//    right->order = (*NYT_node)->order - 1;
//
//    Node_adaptive *previous_NYT_node = *NYT_node;
//
//    (*NYT_node)->left = left;
//    (*NYT_node)->right = right;
//
//    symbol_array[symbol] = right;
//
//    *NYT_node = left;
//
//    return previous_NYT_node;
//}
//
//void huf_write2(u_int8_t i, ofstream& outfile) {
//    static int bit_pos = 0; //0 to 7 (left to right) on the byte block
//    static u_int8_t c = '\0'; //byte block to write
//
//    if (i != 2) {
//        if (i == 1) {
//            c = c | (i << (7 - bit_pos)); //add a 1 to the byte
//        } else {
//            c = c & static_cast<unsigned char>(255 - (1 << (7 - bit_pos))); //add a 0
//        }
//        ++bit_pos;
//        bit_pos %= 8;
//        if (bit_pos == 0) {
//            outfile.put(c);
//            c = '\0';
//        }
//    } else {
//        outfile.put(c);
//    }
//}
//
///**
// * Reads input stream and returns it bit by bit
// * @param infile    input file stream
// * @return          read bit of EOF
// */
//u_int8_t huf_read2(ifstream & infile) {
//    static int bit_pos = 0; //0 to 7 (left to right) on the byte block
//    static u_int8_t c = infile.get();
//
//    u_int8_t i;
//
//    i = (c>>(7-bit_pos))%2; //get the bit from the byte
//    ++bit_pos;
//    bit_pos %= 8;
//    if (bit_pos == 0) {
//        if (!infile.eof()) {
//            c = infile.get();
//        } else {
//            i = 2;
//        }
//    }
//
//    return i;
//}
//
//void encode_character(Node_adaptive* symbol_tree, ofstream& outfile) {
//    stack<u_int8_t> code;
//    Node_adaptive* node = symbol_tree;
//
//    while (node->parent != nullptr) {
//        if (node == node->parent->right) {
//            code.push(1);
//        } else {
//            code.push(0);
//        }
//        node = node->parent;
//    }
//
//    while (!code.empty()) {
//        huf_write2(code.top(), outfile);
//        code.pop();
//    }
//}
//
///* Searches for the node that has the same value as the input node (with a maximum order value).
// * currMax: the input node.
// * root: the tree's root.
// * returns: the node that has the same value as the input node (with a maximum order value). if it doesn't exist, NULL is returned.
// */
//Node_adaptive* findReplaceNode(Node_adaptive *currMax, Node_adaptive *root) {
//    Node_adaptive *result = currMax;
//
//    // weight and is not leaf
//    if (root->weight > result->weight && !(root->left == nullptr && root->right == nullptr) ) {
//        Node_adaptive *greatestLeft = findReplaceNode(result, root->left);
//        if (greatestLeft) {
//            result = greatestLeft;
//        }
//
//        Node_adaptive *greatestRight = findReplaceNode(result, root->right);
//        if (greatestRight) {
//            result = greatestRight;
//        }
//    } else if (root->weight == result->weight && root->order > result->order) {
//        result = root;
//    }
//
//    return (result != currMax) ? result : nullptr;
//}
//
///* Swap nodes.
// * Note: the orders are not swapped, since they are independent from the node's content.
// */
//void swapNodes(Node_adaptive *n1, Node_adaptive *n2) {
//    unsigned int tempOrder = n1->order;
//    n1->order = n2->order;
//    n2->order = tempOrder;
//
//    if (n1->parent->left == n1) {
//        n1->parent->left = n2;
//    } else {
//        n1->parent->right = n2;
//    }
//
//    if (n2->parent->left == n2) {
//        n2->parent->left = n1;
//    } else {
//        n2->parent->right = n1;
//    }
//
//    Node_adaptive *temp = n1->parent;
//    n1->parent = n2->parent;
//    n2->parent = temp;
//}
//
//void updateTree(Node_adaptive *currNode, Node_adaptive *root) {
//    while (currNode->parent != nullptr) {
//        Node_adaptive *replaceNode = findReplaceNode(currNode, root);
//
//        if (replaceNode && currNode->parent != replaceNode) {
//            swapNodes(currNode, replaceNode);
//        }
//
//        (currNode->weight)++;
//        currNode = currNode->parent;
//    }
//
//    (currNode->weight)++;
//}
//
//void encoder_adaptive(string ifile, string ofile, bool model) {
//    ifstream infile(ifile.c_str(), ios::in|ios::binary);
//    if(!infile) {
//        cerr << ifile << " could not be opened!" << endl;
//        exit(1);
//    }
//
//    if(ifstream(ofile.c_str())) {
//        cerr << ofile << " already exists!" << endl;
//        exit(1);
//    }
//
//    //open the output file
//    ofstream outfile(ofile.c_str(), ios::out|ios::binary);
//    if(!outfile) {
//        cerr << ofile << " could not be opened!" << endl;
//        exit(1);
//    }
//
//    //input file was empty - do empty output and finish
//    if (infile.peek() == ifstream::traits_type::eof()) {
//        infile.close();
//        outfile.close();
//        return;
//    }
//
//    Node_adaptive* tree;
//    map<u_int16_t, Node_adaptive*> symbol_array;
//    createRootNode(&tree, symbol_array);
//    Node_adaptive* NYT_node = tree;
//
//    u_int8_t value;
//    char buf[sizeof(u_int8_t)];
//    while (infile.read(buf, sizeof(buf))) {
//        memcpy(&value, buf, sizeof(value));
//
//        Node_adaptive* symbolTree = symbol_array[value];
//
//        if(symbolTree == nullptr) { // symbol is not in tree yet
//            encode_character(NYT_node, outfile);
//            //for (int i = 0; i < sizeof(u_int8_t); i++) {
//            //    huf_write2(buf[i], outfile);
//            //}
//            //outfile.write(buf, sizeof(buf));
//            for (int i = 0; i < 8; i++) {
//                huf_write2((value >> (7 - i)) & 1, outfile);
//                //ch = ch << (7-i);
//                //result |= ch;
//            }
//
//            Node_adaptive *newNode = createNewNode(symbol_array, value, &NYT_node);
//            updateTree(newNode, tree);
//        } else {
//            encode_character(symbolTree, outfile);
//            updateTree(symbolTree, tree);
//        }
//    }
//    // EOF
//    huf_write2(2, outfile);
//
//    infile.close();
//    outfile.close();
//}
//void decoder_adaptive(string ifile, string ofile, bool model) {
//    ifstream infile(ifile.c_str(), ios::in|ios::binary);
//    if (!infile) {
//        cerr << ifile << " could not be opened!" << endl;
//        exit(1);
//    }
//
//    if(ifstream(ofile.c_str())) {
//        cerr << ofile << " already exists!" << endl;
//        exit(1);
//    }
//
//    ofstream outfile(ofile.c_str(), ios::out|ios::binary);
//    if(!outfile) {
//        cerr << ofile << " could not be opened!" << endl;
//        exit(1);
//    }
//
//    //input file was empty - do empty output and finish
//    if (infile.peek() == ifstream::traits_type::eof()) {
//        infile.close();
//        outfile.close();
//        return;
//    }
//
//    Node_adaptive* tree;
//    map<u_int16_t, Node_adaptive*> symbol_array;
//    createRootNode(&tree, symbol_array);
//    Node_adaptive* NYT_node = tree;
//
//    bool nyt = true;
//    Node_adaptive* node = tree;
//    //u_int8_t value;
//    //char buf[sizeof(u_int8_t)];
//    u_int8_t ch = huf_read2(infile);
//    while (ch != 2) {
//
//        if (nyt) {
//
//            huf_write2(ch, outfile);
//            u_int8_t result = 0;
//            ch = ch << 7;
//            result |= ch;
//            for (int i = 1; i < 8; i++) {
//                ch = huf_read2(infile);
//                huf_write2(ch, outfile);
//                ch = ch << (7-i);
//                result |= ch;
//            }
//
//            Node_adaptive *newNode = createNewNode(symbol_array, result, &NYT_node);
//            updateTree(newNode, tree);
//
//            nyt = false;
//        } else {
//            if (ch == 1) {
//                node = node->right;
//            } else {
//                node = node->left;
//            }
//
//            if (node->ch == NYT) {
//                nyt = true;
//                node = tree;
//            } else {//if(node->ch < NYT) {
//                for (int i = 0; i < 8; i++) {
//                    huf_write2((node->ch >> (7 - i)) & 1, outfile);
//                    //ch = ch << (7-i);
//                    //result |= ch;
//                }
//                updateTree(node, tree);
//                node = tree;
//            }
//        }
//        ch = huf_read2(infile);
//    }
//
//    infile.close();
//    outfile.close();
//}

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <stack>
#include "huffman_adaptive.hpp"

#define INVALID -1 /* setted as the symbol of the non-leaf nodes */
#define HOW_MANY_POSSIBLE_SYMBOLS 256 /* how many possible symbols */

enum { ENCODE, DECODE }; /* program options */

/* Describes each tree node */
struct node {
    int isZero;
    int isRoot;
    int isLeaf;

    struct node *parent;
    struct node *leftChild;
    struct node *rightChild;

    unsigned char symbol;
    int value;
    int order;
};
typedef struct node Node;

/* Describes each symbol, containing a pointer to its node in the tree */
struct symbol {
    char symbol;
    Node *tree;
};
typedef struct symbol Symbol;

/* Creates the initial tree just containing the root node.
 * returns: a pointer to the root.
 */
Node* createTree() {
    Node *tree = new Node();
    tree->isZero = 1;
    tree->isRoot = 1;
    tree->isLeaf = 1;

    tree->parent = NULL;
    tree->leftChild = NULL;
    tree->rightChild = NULL;

    tree->symbol = INVALID;
    tree->value = 0;
    tree->order = HOW_MANY_POSSIBLE_SYMBOLS * 2; /* generally 512 (considering char = 8 bits) */

    return tree;
}

/* Obtains the tree node based on its symbol.
 * symbol: the desired symbol.
 * symbols: the array of all symbols.
 * returns: a pointer to the tree node corresponding to the passed symbol. If the node does not exist, a NULL pointer is returned.
 */
Node* getTreeFromSymbol(unsigned char symbol, map<char, Node*> symbols) {
    //Symbol *symbolPtr = symbols[(unsigned int)symbol];

    if (!symbols[symbol]) {
        return NULL;
    }

    return symbols[symbol];
}

/* Reverse an array of integers
 * code: the array that will be reserved;
 * codeSize: how many elements are contained in the array.
 */
//void reverseCode(map<int, int>& code, int codeSize) {
//    if (code == NULL) {
//        printf("reverseCode: tried to reverse a NULL code.\n");
//        return;
//    }
//
//    int start = code[0];
//    int end = code+(codeSize-1);
//
//    while (start < end) {
//        int temp = start;
//        start = end;
//        end = temp;
//        start++;
//        end--;
//    }
//}

/* Returns an array of integers (i.e. 0 and 1s) containing the code corresponding to the given node.
 * node: the tree node.
 * n: the number of digits in the array that will be returned.
 * returns an array of integers containing the code.
 */
stack<int> codeOfNode(Node *node, int *n) {
    Node *current = node;
    //map<int, int> code;

    stack<int> code;
    while (!current->isRoot) {
        Node *parent = current->parent;
        code.push((parent->leftChild == current) ? 0 : 1);
        current = current->parent;
    }
    //reverseCode(code, i);

    //*n = i;
    return code;
}

/* Creates a new child.
 * parent: the parent node that will be created.
 * isZero: 1 if the node is a ZERO node, or 0 otherwise.
 * isRoot: 1 if the created node is a root node, or 0 otherwise.
 * symbol: the symbol attributed to the node.
 * value: the value attributed to the node.
 * order: the order attributed to the node.
 * returns: the created node.
 */
Node* addChild(Node *parent, int isZero, int isRoot, unsigned char symbol, int value, int order) {
    Node *node = new Node();
    node->isZero = isZero;
    node->isRoot = isRoot;
    node->isLeaf = 1;
    node->parent = parent;
    node->leftChild = NULL;
    node->rightChild = NULL;
    node->symbol = symbol;
    node->value = value;
    node->order = order;
    return node;
}

/* Adds a new symbol in the tree and in the array of symbols
 * symbol: the symbol that will be inserted
 * zeroNode: reference to where the node ZERO points to.
 * symbols: the array of symbols
 * returns: the leaf node's (that contains the added symbol) parent.
 */
Node* addSymbol(unsigned char symbol, Node **zeroNode, map<char, Node*> &symbols) {
    Node *leftNode = addChild(*zeroNode, 1, 0, INVALID, 0, (*zeroNode)->order - 2);
    Node *rightNode = addChild(*zeroNode, 0, 0, symbol, 1, (*zeroNode)->order - 1);

    Node *previousZeroNode = *zeroNode;
    (*zeroNode)->isZero = 0;
    (*zeroNode)->isLeaf = 0;
    (*zeroNode)->leftChild = leftNode;
    (*zeroNode)->rightChild = rightNode;

    //unsigned int symbolIndex = (unsigned int)symbol;
    //symbols[symbolIndex] = new Symbol();
    symbols[symbol] = rightNode;
    //symbols[symbolIndex]->tree = rightNode;

    *zeroNode = leftNode;

    return previousZeroNode;
}

/* Searches for the node that has the same value as the input node (with a maximum order value).
 * currMax: the input node.
 * root: the tree's root.
 * returns: the node that has the same value as the input node (with a maximum order value). if it doesn't exist, NULL is returned.
 */
Node* findReplaceNode(Node *currMax, Node *root) {
    Node *result = currMax;

    if (root->value > result->value && !root->isLeaf) {
        Node *greatestLeft = findReplaceNode(result, root->leftChild);
        if (greatestLeft) result = greatestLeft;

        Node *greatestRight = findReplaceNode(result, root->rightChild);
        if (greatestRight) result = greatestRight;
    } else if (root->value == result->value && root->order > result->order) {
        result = root;
    }

    return (result != currMax) ? result : NULL;
}

/* Swap nodes.
 * Note: the orders are not swapped, since they are independent from the node's content.
 */
void swapNodes(Node *n1, Node *n2) {
    /* mudando as ordens porque elas nao se trocam */
    int tempOrder = n1->order;
    n1->order = n2->order;
    n2->order = tempOrder;

    if (n1->parent->leftChild == n1) {
        n1->parent->leftChild = n2;
    } else {
        n1->parent->rightChild = n2;
    }

    if (n2->parent->leftChild == n2) {
        n2->parent->leftChild = n1;
    } else {
        n2->parent->rightChild = n1;
    }

    Node *temp = n1->parent;
    n1->parent = n2->parent;
    n2->parent = temp;
}

/* Updates the tree based on an initial node.
 * currNode: the starting node that will be updated.
 * root: the tree's root.
 */
void updateTree(Node *currNode, Node *root) {
    while (!currNode->isRoot) {
        Node *replaceNode = findReplaceNode(currNode, root);

        /* se replaceNode for o pai de currentNode, eles nao podem ser trocados */
        if (replaceNode && currNode->parent != replaceNode) {
            swapNodes(currNode, replaceNode);
        }

        (currNode->value)++;
        currNode = currNode->parent;
    }

    (currNode->value)++;
}

/* Adds a code (of 0 and 1s) to the output buffer. When the buffer gets full, it will be written in the file.
 * code: the code that will be added into the output buffer. WHen the buffer gets full, it will be written in the file.
 * codeSize: how many bits are contained in code.
 * fp: the output file.
 * buffer: the input buffer.
 * bufferSize: how many bits in buffer are waiting to be written.
 * returns: the new buffer.
 */
unsigned char addCodeToBuffer(stack<int> &code, int codeSize, FILE *fp, unsigned char buffer, int *bufferSize) {
    unsigned char currBuffer = buffer;

    //int i;
    //for (i = 0; i < codeSize; i++) {
    while (!code.empty()) {
        unsigned char bit = ((unsigned char) code.top()) << (*bufferSize - 1);
        currBuffer = currBuffer | bit;

        (*bufferSize)--;

        if (*bufferSize == 0) {
            fwrite(&currBuffer, sizeof(unsigned char), 1, fp);
            currBuffer = 0;
            *bufferSize = 8;
        }
        code.pop();
    }

    return currBuffer;
}

/* Adds a byte into the output buffer. If the buffer is not previously empty, a byte will be written in the output file.
 * byte: the byte that will be written in the buffer.
 * fp: The output file.
 * buffer: the input buffer.
 * bufferSize: how many bits in the buffer are waiting to be written.
 * returns: the new buffer.
 */
unsigned char addByteToBuffer(char byte, FILE *fp, unsigned char buffer, int *bufferSize) {
    unsigned char currBuffer = buffer;

    int howManyBitsWillBeWritten = *bufferSize;
    int shiftSize = 8 - howManyBitsWillBeWritten;
    unsigned char tempByte = ((unsigned char) byte) >> shiftSize;
    currBuffer = currBuffer | tempByte;

    fwrite(&currBuffer, sizeof(unsigned char), 1, fp);

    currBuffer = byte << howManyBitsWillBeWritten;

    return (*bufferSize == 8) ? 0 : currBuffer;
}

/* Writes the buffer in the output file.
 * Also writes the last byte containing the amount of bits that the decoder will read in the next to last byte.
 *
 * fp: the output file.
 * buffer: the input buffer.
 * bufferSize: how many bits should be written in the buffer.
 */
void writeRemainingBits(FILE *fp, unsigned char buffer, int bufferSize) {
    if (bufferSize < 8) {
        fwrite(&buffer, sizeof(unsigned char), 1, fp);
    }

    /* Writes in the last byte how many bits should be read by the decoded in the next to last byte. */
    buffer = (bufferSize == 8) ? 8 : 8 - bufferSize;
    fwrite(&buffer, sizeof(unsigned char), 1, fp);
}

/* Encodes the file.
 * fp_in: the input file (the one that will be encoded).
 * fp_out: the output file.
 */
void encoder_adaptive(FILE *fp_in, FILE *fp_out) {
    Node *root = createTree();
    Node *zeroNode = root;

    unsigned char buffer = 0;
    int bufferSize = 8;

    map<char, Node*> symbols;

    unsigned char currByte;
    while (fread(&currByte, sizeof(unsigned char), 1, fp_in) > 0) {
        Node *symbolTree = getTreeFromSymbol(currByte, symbols);

        if (symbolTree) {
            int codeSize;
            stack<int> symbolCode = codeOfNode(symbolTree, &codeSize);
            buffer = addCodeToBuffer(symbolCode, codeSize, fp_out, buffer, &bufferSize);

            updateTree(symbolTree, root);
            //free(symbolCode);
        } else {
            int codeSize;

            stack<int> zeroCode = codeOfNode(zeroNode, &codeSize);
            buffer = addCodeToBuffer(zeroCode, codeSize, fp_out, buffer, &bufferSize);
            buffer = addByteToBuffer(currByte, fp_out, buffer, &bufferSize);

            Node *newNode = addSymbol(currByte, &zeroNode, symbols);
            updateTree(newNode, root);
            //free(zeroCode);
        }
    }

    writeRemainingBits(fp_out, buffer, bufferSize);
}

/* Read the next bit to be read in the biffer and returns it.
 * fp: input file (that will be read in case the buffer limit is reached).
 * buffer: the input buffer, that can be updated in case a read operation if performed in the input file.
 * bufferSize: how many bits there are left to be read in the buffer.
 * fileSize: the filesize (until its next to last byte).
 * readHowManyBitsAtTheEnd: how many bits should be read in the next to last byte.
 * returns: the bit (0 or 1).
 */
int readBit(FILE *fp, unsigned char *buffer, int *bufferSize, long int fileSize, int readHowManyBitsAtTheEnd) {
    if (*bufferSize == 0) {
        *bufferSize = 8;
        fread(buffer, sizeof(unsigned char), 1, fp);
    }

    if (readHowManyBitsAtTheEnd != 8) {
        if (ftell(fp) == fileSize && readHowManyBitsAtTheEnd <= (8 - *bufferSize)) return -1;
    }

    if (ftell(fp) > fileSize || feof(fp)) return -1;

    (*bufferSize)--;

    return (*buffer >> *bufferSize) & 1;
}

/* Reads the next 8 bits to be read and return it as a char.
 * fp: input file (that will be read in case the buffer limit is reached).
 * buffer: the input buffer, that can be updated in case a read operation if performed in the input file.
 * bufferSize: how many bits there are left to be read in the buffer.
 * fileSize: the filesize (until its next to last byte).
 * readHowManyBitsAtTheEnd: how many bits should be read in the next to last byte.
 * returns: the read byte.
 */
char readByte(FILE *fp, unsigned char *buffer, int *bufferSize, long int fileSize, int readHowManyBitsAtTheEnd) {
    char result = 0;

    int i, bit;
    for (i = 0; i < 8; i++) {
        bit = readBit(fp, buffer, bufferSize, fileSize, readHowManyBitsAtTheEnd);
        bit = bit << (7-i);
        result |= bit;
    }

    return result;
}

/* Decodes the file
 * fp_in: input file (the one that will be decoded)
 * fp_out: the output file
 */
void decoder_adaptive(FILE *fp_in, FILE *fp_out) {
    Node *root = createTree();
    Node *zeroNode = root;

    unsigned char buffer = 0;
    int bufferSize = 0;

    map<char, Node*> symbols;

    /* Obtains the file size (excluding the last byte). */
    fseek(fp_in, -1, SEEK_END);
    long int fileSize = ftell(fp_in);

    /* Reads the last byte, which contains the number of bits that should be read in the previous byte */
    unsigned char readHowManyBitsAtTheEnd;
    fread(&readHowManyBitsAtTheEnd, sizeof(unsigned char), 1, fp_in);
    rewind(fp_in);

    while (!feof(fp_in)) {
        Node *currNode = root;

        int endOfFile = 0;
        while (!currNode->isLeaf && !endOfFile) {
            int bit = readBit(fp_in, &buffer, &bufferSize, fileSize, readHowManyBitsAtTheEnd);
            if (bit == 0) {
                currNode = currNode->leftChild;
            } else if (bit == 1) {
                currNode = currNode->rightChild;
            } else {
                endOfFile = 1;
            }
        }

        if (endOfFile) break;

        unsigned char c;
        if (currNode->isZero) {
            c = readByte(fp_in, &buffer, &bufferSize, fileSize, readHowManyBitsAtTheEnd);
            currNode = addSymbol(c, &zeroNode, symbols);
        } else {
            c = currNode->symbol;
        }

        fwrite(&c, sizeof(unsigned char), 1, fp_out);
        updateTree(currNode, root);
    }
}

//int main(int argc, char *argv[]) {
//    if (argc != 4) {
//        printf("Usage:\n");
//        printf("\t./fgk input_file output_file -c (to encode)\n");
//        printf("\t./fgk input_file output_file -d (to decode)\n");
//        exit(1);
//    }
//
//    FILE *fp_in;
//    FILE *fp_out;
//    int option;
//
//    /* input file */
//    fp_in = fopen(argv[1], "rb");
//    while (fp_in == NULL) {
//        char in_file[100];
//        printf("The file %s could not be opened. Try again: ", argv[1]);
//        scanf("%s", in_file);
//        fp_in = fopen(in_file, "rb");
//    }
//
//    /* output file */
//    fp_out = fopen(argv[2], "wb");
//
//    /* option: encode or decode */
//    if (strcmp(argv[3], "-c") == 0 || strcmp(argv[3], "-C") == 0) {
//        option = ENCODE;
//    } else if (strcmp(argv[3], "-d") == 0 || strcmp(argv[3], "-D") == 0) {
//        option = DECODE;
//    } else {
//        char opt;
//        do {
//            printf("Invalid option, type 'c' to encode or 'd' to decode: ");
//            scanf("%c", &opt);
//            getchar();
//        } while (opt != 'c' && opt != 'C' && opt != 'd' && opt != 'D');
//        option = (opt == 'c' || opt == 'C') ? ENCODE : DECODE;
//    }
//
//    if (option == ENCODE) {
//        encode(fp_in, fp_out);
//        printf("The file was encoded successfully.\n");
//    } else {
//        decode(fp_in, fp_out);
//        printf("The file was decoded successfully\n");
//    }
//
//    fclose(fp_in);
//    fclose(fp_out);
//
//    return 0;
//}