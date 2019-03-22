#pragma once
#include <vector>
#include <string>
#include "AdvDictionary.h"
using namespace std;
class WordSearch {
	const char* puzzleName = "wordsearch_grid.txt";
	const char* dictionaryName = "dictionary.txt";
public:
	explicit WordSearch(const char * const filename);
	WordSearch& operator=(const WordSearch &obj) = default;
	WordSearch(const WordSearch &obj);
	~WordSearch();

	void ReadSimplePuzzle();
	void ReadSimpleDictionary();
	void ReadAdvancedPuzzle();
	void ReadAdvancedDictionary();
	void SolvePuzzleSimple();
	void SolvePuzzleAdvanced();
	void WriteResults(const	double loadTime, const double solveTime);

private:
	const char* m_output;
	int m_Grid_Size;
	int m_Adv_Dictionary_Size;
	char** m_Grid;
	vector<string> m_Dictionary;
	vector<string> m_Words_Found;
	vector<string> m_Words_Found_Pos;

	AdvDictionary m_Adv_Dictionary;
	// Statistics
	int m_Cells_Visited, m_Dictionary_Searches;

	void SimpleSearch(const char* const letter, const int* const xPos, const int* const yPos);
	void AdvancedSearch(char* const letter, const int* const xPos, const int* const yPos);
	void SimpleWrite(const double& loadTime, const double& solveTime) const;
	void AdvancedWrite(const double& loadTime, const double& solveTime);
};



