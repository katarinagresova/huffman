#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <stack>
#include <iostream>
#include "huffman_adaptive.hpp"
#include "file_manipulation.hpp"

/* Creates the initial tree just containing the root node.
 * returns: a pointer to the root.
 */
Node* createTree() {
    Node *tree = new Node();
    tree->is_NYT = 1;
    tree->is_leaf = 1;

    tree->parent = nullptr;
    tree->left_child = nullptr;
    tree->right_child = nullptr;

    tree->weight = 0;
    tree->order = SYMBOLS_COUNT * 2; /* generally 512 (considering char = 8 bits) */

    return tree;
}

/* Returns an array of integers (i.e. 0 and 1s) containing the code corresponding to the given node.
 * node: the tree node.
 * n: the number of digits in the array that will be returned.
 * returns an array of integers containing the code.
 */
stack<u_int8_t > codeOfNode(Node *node) {
    Node *current = node;

    stack<u_int8_t> code;
    while (current->parent != nullptr) {
        Node *parent = current->parent;
        code.push((parent->left_child == current) ? 0 : 1);
        current = current->parent;
    }
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
Node* addChild(Node *parent, int isZero, u_int8_t symbol, int value, int order) {
    Node *node = new Node();
    node->is_NYT = isZero;
    node->is_leaf = 1;
    node->parent = parent;
    node->left_child = nullptr;
    node->right_child = nullptr;
    node->symbol = symbol;
    node->weight = value;
    node->order = order;
    return node;
}

/* Adds a new symbol in the tree and in the array of symbols
 * symbol: the symbol that will be inserted
 * zeroNode: reference to where the node ZERO points to.
 * symbols: the array of symbols
 * returns: the leaf node's (that contains the added symbol) parent.
 */
Node* addSymbol(unsigned char symbol, Node **zeroNode, map<u_int8_t, Node*> &symbols) {
    Node *leftNode = addChild(*zeroNode, 1, 0, 0, (*zeroNode)->order - 2);
    Node *rightNode = addChild(*zeroNode, 0, symbol, 1, (*zeroNode)->order - 1);

    Node *previousZeroNode = *zeroNode;
    (*zeroNode)->is_NYT = 0;
    (*zeroNode)->is_leaf = 0;
    (*zeroNode)->left_child = leftNode;
    (*zeroNode)->right_child = rightNode;

    symbols[symbol] = rightNode;
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

    if (root->weight > result->weight && !root->is_leaf) {
        Node *greatestLeft = findReplaceNode(result, root->left_child);
        if (greatestLeft) result = greatestLeft;

        Node *greatestRight = findReplaceNode(result, root->right_child);
        if (greatestRight) result = greatestRight;
    } else if (root->weight == result->weight && root->order > result->order) {
        result = root;
    }

    return (result != currMax) ? result : nullptr;
}

/* Swap nodes.
 * Note: the orders are not swapped, since they are independent from the node's content.
 */
void swapNodes(Node *n1, Node *n2) {
    int tempOrder = n1->order;
    n1->order = n2->order;
    n2->order = tempOrder;

    if (n1->parent->left_child == n1) {
        n1->parent->left_child = n2;
    } else {
        n1->parent->right_child = n2;
    }

    if (n2->parent->left_child == n2) {
        n2->parent->left_child = n1;
    } else {
        n2->parent->right_child = n1;
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
    while (currNode->parent != nullptr) {
        Node *replaceNode = findReplaceNode(currNode, root);

        /* se replaceNode for o pai de currentNode, eles nao podem ser trocados */
        if (replaceNode && currNode->parent != replaceNode) {
            swapNodes(currNode, replaceNode);
        }

        (currNode->weight)++;
        currNode = currNode->parent;
    }

    (currNode->weight)++;
}

/* Encodes the file.
 * fp_in: the input file (the one that will be encoded).
 * fp_out: the output file.
 */
void encoder_adaptive(string ifile, string ofile, bool model) {

    FILE *fp_in, *fp_out;
    check_files(ifile, ofile, &fp_in, &fp_out);

    Node *root = createTree();
    Node *zeroNode = root;
    map<u_int8_t , Node*> symbols;

    u_int8_t currByte;
    while (fread(&currByte, sizeof(u_int8_t), 1, fp_in) > 0) {
        Node *symbolTree = symbols[currByte];

        if (symbolTree) {
            stack<u_int8_t> symbolCode = codeOfNode(symbolTree);
            while(!symbolCode.empty()) {
                write_bit(symbolCode.top(), fp_out);
                symbolCode.pop();
            }

            updateTree(symbolTree, root);
        } else {
            stack<u_int8_t> NYT_code = codeOfNode(zeroNode);
            while(!NYT_code.empty()) {
                write_bit(NYT_code.top(), fp_out);
                NYT_code.pop();
            }
            write_byte(currByte, fp_out);

            Node *newNode = addSymbol(currByte, &zeroNode, symbols);
            updateTree(newNode, root);
        }
    }

    write_bit(2, fp_out);

    fclose(fp_in);
    fclose(fp_out);
}

/* Decodes the file
 * fp_in: input file (the one that will be decoded)
 * fp_out: the output file
 */
void decoder_adaptive(string ifile, string ofile, bool model) {

    FILE *fp_in, *fp_out;
    check_files(ifile, ofile, &fp_in, &fp_out);

    Node *root = createTree();
    Node *zeroNode = root;
    map<u_int8_t , Node*> symbols;

    while (!feof(fp_in)) {
        Node *currNode = root;

        int endOfFile = 0;
        while (!currNode->is_leaf && !endOfFile) {
            int bit = readBit(fp_in);
            if (bit == 0) {
                currNode = currNode->left_child;
            } else if (bit == 1) {
                currNode = currNode->right_child;
            } else {
                endOfFile = 1;
            }
        }

        if (endOfFile) break;

        u_int8_t c;
        if (currNode->is_NYT) {
            c = readByte(fp_in);
            currNode = addSymbol(c, &zeroNode, symbols);
        } else {
            c = currNode->symbol;
        }

        fwrite(&c, sizeof(unsigned char), 1, fp_out);
        updateTree(currNode, root);
    }

    fclose(fp_in);
    fclose(fp_out);
}