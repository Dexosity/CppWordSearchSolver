#include "Dictionary_Node.h"
#include <vector>
#include <iostream>

using namespace std;

Dictionary_Node::Dictionary_Node() : m_isEndNode(false)
{
	setLetter(NULL);
}
Dictionary_Node::Dictionary_Node(const char& letter) : m_letter(letter), m_isEndNode(false) {
}

Dictionary_Node::~Dictionary_Node()
{
}
// Setter for node letter value
void Dictionary_Node::setLetter(const char& letter) {
	m_letter = letter;
}
// Getter for node letter value
char Dictionary_Node::getLetter() const {
	return m_letter;
}
// Adds new child to node
void Dictionary_Node::addChild(const Dictionary_Node& child) {
	m_children.push_back(child);
}
// Gets child of node at index by reference
Dictionary_Node& Dictionary_Node::getChild(const int& index){
	return m_children[index];
}
// Gets all children of node by reference
const vector<Dictionary_Node>& Dictionary_Node::getChildren() const{
	return m_children;
}
// Setter for is end of word node
void Dictionary_Node::setIsEnd(const bool& isEnd) {
	m_isEndNode = isEnd;
}
// Getter for is end of word node
bool Dictionary_Node::getIsEnd() const {
	return m_isEndNode;
}

