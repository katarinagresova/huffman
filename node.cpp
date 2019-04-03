/**
 * Autor: Katarina Gresova, xgreso00
 * Datum: 01.04.2019
 * Name: node.cpp
 * Description: Tree object used for adaptive huffman algorithm.
 */

#include "node.hpp"
#include <stack>

using namespace std;

/* Constructors */
Node::Node() : left(nullptr), right(nullptr) {
  weight = 0;
  parent = nullptr;

  group_next = this;
  group_prev = this;
}

Node::Node(Node* nyt, Node* leaf) {
  weight = 0;
  parent = nullptr;
  left = unique_ptr<Node>(nyt);
  right = unique_ptr<Node>(leaf);

  left->parent = this;
  right->parent = this;

  group_next = this;
  group_prev = this;
}

Node::Node(char symbol) {
  weight = 0;
  parent = nullptr;
  this->symbol = symbol;
  group_next = this;
  group_prev = this;
}

/* Setters */
void Node::set_left(Node* left) {
  this->left.release(); 
  this->left = std::unique_ptr<Node>(left);
  this->left->parent = this;
}
void Node::set_right(Node* right) {
  this->right.release(); 
  this->right = std::unique_ptr<Node>(right);
  this->right->parent = this;
}

/* Getters */
char Node::get_symbol() { return symbol; }
int Node::get_weight() { return weight; }
Node* Node::get_child(bool get_right) { return get_right ? right.get() : left.get(); }

/* Methods */
bool Node::is_leaf() { return left == nullptr && right == nullptr; }