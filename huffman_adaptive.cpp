/**
 * Author: Katarina Gresova, xgreso00
 * Datum: 12.04.2019
 * Name: huffman_adaptive.cpp
 * Description: Adaptive Huffman coding.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <stack>
#include <iostream>
#include "huffman_adaptive.hpp"
#include "file_manipulation.hpp"
#include "model.hpp"

/**
 * Creates initial tree containing only root
 * @return  pointer to the root
 */
Node* createTree() {
    Node *tree = new Node();
    tree->is_NYT = 1;
    tree->is_leaf = 1;

    tree->parent = nullptr;
    tree->left_child = nullptr;
    tree->right_child = nullptr;

    tree->weight = 0;
    tree->order = SYMBOLS_COUNT * 2;
    return tree;
}

/**
 * Traverses tree from given node to root and stores code
 * @param node  tree node to start from
 * @return      stack of 0s a 1s corresponding to the code
 */
stack<u_int8_t > codeOfNode(Node *node) {
    Node *current = node;

    stack<u_int8_t> code;
    while (current->parent != nullptr) {
        Node *parent = current->parent;
        code.push((parent->left_child == current) ? u_int8_t (0) : u_int8_t (1));
        current = current->parent;
    }
    return code;
}

/**
 *
 * @param parent    parent of new node
 * @param is_NYT    flag, if new node is NYT node
 * @param symbol    symbol of new node
 * @param weight    weight of new node
 * @param order     order of new node
 * @return          pointer to new node
 */
Node* addChild(Node *parent, bool is_NYT, u_int8_t symbol, int weight, int order) {
    Node *node = new Node();
    node->is_NYT = is_NYT;
    node->is_leaf = true;
    node->parent = parent;
    node->left_child = nullptr;
    node->right_child = nullptr;
    node->symbol = symbol;
    node->weight = weight;
    node->order = order;
    return node;
}

/**
 * Adds new symbol node too tree and adds it to symbols map
 * @param symbol        new symbol
 * @param NYT_node      reference to NYT_node
 * @param symbols       map of symbols
 * @return              parent of node with new symbol
 */
Node* addSymbol(u_int8_t symbol, Node **NYT_node, map<u_int8_t, Node*> &symbols) {
    Node *leftNode = addChild(*NYT_node, true, 0, 0, (*NYT_node)->order - 2);
    Node *rightNode = addChild(*NYT_node, false, symbol, 1, (*NYT_node)->order - 1);

    Node *previous_NYT_node = *NYT_node;
    (*NYT_node)->is_NYT = 0;
    (*NYT_node)->is_leaf = 0;
    (*NYT_node)->left_child = leftNode;
    (*NYT_node)->right_child = rightNode;

    symbols[symbol] = rightNode;
    *NYT_node = leftNode;

    return previous_NYT_node;
}

/**
 * Searches for the node that has the same weight as input node (with a maximum order value).
 * @param currMax   node with highest weight so far
 * @param root      root of tree
 * @return          node with the same weight as input node, or null
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

/**
 * Swaps two nodes
 * @param n1    first node
 * @param n2    second node
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

/**
 * Updates tree based on initial node
 * @param currNode  starting node that will be updated
 * @param root      root of tree
 */
void updateTree(Node *currNode, Node *root) {
    while (currNode->parent != nullptr) {
        Node *replaceNode = findReplaceNode(currNode, root);

        if (replaceNode && currNode->parent != replaceNode) {
            swapNodes(currNode, replaceNode);
        }

        (currNode->weight)++;
        currNode = currNode->parent;
    }

    (currNode->weight)++;
}

/**
 * Adaptive Huffman encoder
 * @param ifile     path to input file
 * @param ofile     path to output file
 * @param model     flag, if model will be used
 */
void encoder_adaptive(string ifile, string ofile, bool model) {

    FILE *fp_in, *fp_out;
    check_files(ifile, ofile, &fp_in, &fp_out);

    Node *root = createTree();
    Node *zeroNode = root;
    map<u_int8_t , Node*> symbols;

    u_int8_t currByte;
    while (fread(&currByte, sizeof(u_int8_t), 1, fp_in) > 0) {
        if (model) {
            currByte = modeling(currByte);
        }
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

    write_bit(EOF_VAL, fp_out);

    fclose(fp_in);
    fclose(fp_out);
}

/**
 * Adaptive Huffman decoder
 * @param ifile     path to input file
 * @param ofile     path to output file
 * @param model     flag, if model will be used
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

        if (model) {
            c = demodeling(c);
        }
        fwrite(&c, sizeof(unsigned char), 1, fp_out);
        updateTree(currNode, root);
    }

    fclose(fp_in);
    fclose(fp_out);
}