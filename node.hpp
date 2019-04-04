///**
// * Autor: Katarina Gresova, xgreso00
// * Datum: 01.04.2019
// * Name: node.hpp
// * Description: Header file for Node object for constructing Tree in Adaptive Huffman algorithm.
// */
//
//#ifndef node_hpp
//#define node_hpp
//
//#include <memory>
//#include <iostream>
//
//class Node {
//  friend class Tree; ///< The tree structure manages and manipulates its nodes
//
//  int weight;  ///< Num occurrences for the symbol (or sum of children for internal)
//  char symbol; ///< The symbol represented by this leaf (unused for internals)
//
//  Node* parent;      ///< Pointer to the node's parent
//  Node* group_next;  ///< Pointer to the next node in weight group
//  Node* group_prev;  ///< Pointer to the prev node in weight group
//
//  unique_ptr<Node> left, right;  ///< Node owns its children
//
//  Node();              ///< Constructor for NYT Node
//  Node(char);          ///< Constructor for leaf Node
//  Node(Node*, Node*);  ///< Constructor for branch Node
//
//  void transmit_path(OutputBuffer&); ///< Pushes the path to this node to the output buffer (left = 0, right = 1)
//
//  void set_left(Node*);   ///< Releases the left node and takes ownership of the given node
//  void set_right(Node*);  ///< Releases the right node and takes ownership of the given node
//
//
//public:
//  bool is_leaf();                 ///< Checks if the current node has any children
//  Node* get_child(bool right);  ///< Gets the node in the given direction (left = 0, right = 1)
//
//  int get_weight();   ///< Get the Node's associated weight
//  char get_symbol();  ///< Get the Node's associated symbol
//};
//
//#endif