#pragma once
#include "Dictionary_Node.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class AdvDictionary
{
public:
	AdvDictionary();
	~AdvDictionary();

	void addWord(const string& word);
	bool searchDictionary(const string& word);
	Dictionary_Node& getDictionaryWord(Dictionary_Node& nodeLevel, const char& letter) const;
	Dictionary_Node& getRoot();

private:
	Dictionary_Node m_root;
};

