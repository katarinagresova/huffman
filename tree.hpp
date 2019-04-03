/**
 * Autor: Katarina Gresova, xgreso00
 * Datum: 01.04.2019
 * Name: tree.hpp
 * Description: Header file for Tree obejct.
 */

#ifndef tree_hpp
#define tree_hpp

#include <iostream>
#include <unordered_map>
#include <memory>
#include <stdint.h>

using namespace std;

class Tree{
  	Node* nyt;      ///< Maintained pointer to the NYT node

  	unique_ptr<Node> root;   ///< Tree owns root to provide auto cleanup
  	unordered_map<int, Node*> groups;  ///< Mapping from weight to heighest node of weight
  	unordered_map<int64_t, Node*> leaves; ///< Mapping from symbol to representing leaf

  	void update_weight(Node*);      ///< Increments leaf weight or recalculates branch weight
  	void change_weight(Node*, int); ///< Changes node weight, while maintaining groups
  	void perform_swap(Node*);       ///< Swaps the node with the heighest weighted in group

  	// Getters
  	Node* get_root();                    ///< Provides a ptr to the root node
  	Node* get_weight_group(int weight);  ///< Provides a ptr to the head of the weight group

  	// Setters
  	void set_root(Node* root);  ///< Takes ownership of the new node (releases old)

public:
  	Tree();  ///< Standard constructor

  	bool contains(int64_t symbol);  ///< Checks the tree for a given symbol

  	void process_symbol(int64_t symbol, vector<int64_t> *output);  ///< Encodes a single symbol and update structure

  	void encode(int64_t symbol, std::vector<int64_t> *output);  ///< Begins encoding of the input stream
  	void decode(FILE *inputFile, FILE *outputFile);  ///< Begins decoding of the input stream
};


#endif