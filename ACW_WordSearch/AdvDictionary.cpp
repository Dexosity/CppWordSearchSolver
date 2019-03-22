#include "AdvDictionary.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

AdvDictionary::AdvDictionary()
{
}

AdvDictionary::~AdvDictionary()
{
}
// Adds a new word to the dictionary tree
void AdvDictionary::addWord(const string& word) {
	
	// If its the first entry into root then adds straight away as nothing to compare to
	if (static_cast<int>(m_root.getChildren().size()) == 0) {
		m_root.addChild(Dictionary_Node(word[0]));
	}
	else {
		int i;
		// Checks if word is already listed and breaks if true
		for (i = 0; i < static_cast<int>(m_root.getChildren().size()); i++) {
			if (m_root.getChild(i).getLetter() == word[0]) {
				break;
			}
		}
		// Onlys adds new letter if exit loop so use i value to check
		if (i == static_cast<int>(m_root.getChildren().size())) {
			m_root.addChild(Dictionary_Node(word[0]));
		}
	}
	Dictionary_Node *current;
	int count = 0;
	do {
		// Checks if the child of root at index of count == first letter in the word
		if (m_root.getChild(count).getLetter() == word[0]) {
			// Sets current node to the child of root that matches
			current = &m_root.getChild(count);
			int i, j;
			// First loop handles the search for each letter in the word
			for (i = 1; i < static_cast<int>(word.length()); i++) {
				// This loop handles searching for above letter in current node
				bool isFound = false;
				for (j = 0; j < static_cast<int>(current->getChildren().size()); j++) {
					// If found set current node to that node
					if (word[i] == current->getChild(j).getLetter()) {
						current = &current->getChild(j);
						isFound = true;
					}
				}
				// If not found then add as new child and set as current
				if (!isFound && i < static_cast<int>(word.length())) {
					current->addChild(Dictionary_Node(word[i]));
					const int index = static_cast<int>(current->getChildren().size()) - 1;
					current = &current->getChild(index);
				}
				// Will loop into next letter of word to repeat process
			}
			// Once at the end of the word sets last current IsEnd to true and breaks out
			const bool end = true;
			current->setIsEnd(end);
			break;
		}
		count++;
	} while (count < static_cast<int>(m_root.getChildren().size()));

}
// Searches the dictionary for word specified
bool AdvDictionary::searchDictionary(const string& word){
	int count = 0;
	Dictionary_Node* current;
	do {
		// Checks if the child of root at index of count == first letter in the word
		if (m_root.getChild(count).getLetter() == word[0]) {
			// Sets current node to the child of root that matches
			current = &m_root.getChild(count);
			int i, j;
			// First loop handles the search for each letter in the word
			for (i = 0; i < static_cast<int>(word.length()); i++) {
				// This loop handles searching for above letter in current node
				for (j = 0; j < static_cast<int>(current->getChildren().size()); j++) {
					// If found set current node to that node
					if (word[i] == current->getChild(j).getLetter()) {
						current = &current->getChild(j);	
					}
					const int length = static_cast<int>(word.length()) - 1;
					if (i == (length) && current->getIsEnd()) {
						return true;
					}
				}
			}
			break;
		}
		count++;
	} while (count < static_cast<int>(m_root.getChildren().size()));
	return false;
}
// Searches for node of letter specificed, returns new node or node specified
Dictionary_Node& AdvDictionary::getDictionaryWord(Dictionary_Node& nodeLevel, const char& letter) const{
	int i;
	if (letter != NULL) {
		for (i = 0; i < static_cast<int>(nodeLevel.getChildren().size()); i++) {
			if (nodeLevel.getChild(i).getLetter() == letter) {
				return nodeLevel.getChild(i);
			}
		}
	}
	return nodeLevel;
}
// Getter for root node by reference
Dictionary_Node& AdvDictionary::getRoot() {
	return m_root;
}



