#pragma once
#include <string>
#include <vector>

using namespace std;

class Dictionary_Node
{
public:
	Dictionary_Node();
	explicit Dictionary_Node(const char& letter);
	~Dictionary_Node();

	void setLetter(const char& letter);
	char getLetter() const;
	void addChild(const Dictionary_Node& child);
	Dictionary_Node& getChild(const int& index);
	const vector<Dictionary_Node>& getChildren() const;
	void setIsEnd(const bool& isEnd);
	bool getIsEnd() const;
private:
	char m_letter;
	vector<Dictionary_Node> m_children;
	bool m_isEndNode;
};


