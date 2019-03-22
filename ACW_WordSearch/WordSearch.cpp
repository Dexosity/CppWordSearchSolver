#include "WordSearch.h"
#include "AdvDictionary.h"
#include <fstream>
#include <ostream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
// WordSearch Constructor
WordSearch::WordSearch(const char * const filename) : m_output(filename), m_Grid_Size(0), m_Adv_Dictionary_Size(0), m_Grid(nullptr), m_Cells_Visited(0), m_Dictionary_Searches(0) {

}
// WordSearch Copy Constructor
WordSearch::WordSearch(const WordSearch &obj) : m_output(nullptr), m_Grid_Size(0), m_Adv_Dictionary_Size(0), m_Grid(nullptr), m_Cells_Visited(0), m_Dictionary_Searches(0) {
	m_Grid = new char*;
	*m_Grid = *obj.m_Grid;
}
// WordSearch Deconstrutor
WordSearch::~WordSearch() {
	// Deletes grid allocation
	for (int i = 0; i < m_Grid_Size; i++) {
		delete[] m_Grid[i];
	}
	delete[] m_Grid;
}
// Stores the grid values for the simple solver
void WordSearch::ReadSimplePuzzle() {
	ifstream file(puzzleName);
	char c = NULL;
	string size;
	if (file.is_open()) {
		while(c != '\n'){
			file.get(c);
			// Returns an ascii value -48 to make it equal its true value
			size += c;
		}
		m_Grid_Size = stoi(size);
		
		// Gen and Intialise arrays to form 2D array
		int i, j;
		m_Grid = new char*[m_Grid_Size];
		for (i = 0; i < m_Grid_Size; i++) {
			// For every item in the array, create a new array inside of the same size
			m_Grid[i] = new char[m_Grid_Size];
		}
		// Sets the values inside the 2D array equal to chars from the file of equal position
		// i = y & j = x
		for (i = 0; i < m_Grid_Size; i++) {
			for (j = 0; j < m_Grid_Size; j++) {
				// Reads the next char and sets c to it
				file >> c;
				// Sets the value of that position in the 2D array to c
				m_Grid[j][i] = c;
			}
		}
	}
	else { cout << "Unable to open file!"; }
}
// Stores the dictionary for the simple solver
void WordSearch::ReadSimpleDictionary() {
	ifstream file(dictionaryName);
	string line;
	// Loops through file (dictionary) and stores current line in string 'line'
	while (getline(file, line)) {
		// Use push_back to add the line to the dictionary vector
		m_Dictionary.push_back(line);

	}
}
// Stores the grid values for the advanced solver
void WordSearch::ReadAdvancedPuzzle() {
	// Called ReadSimplePuzzle as advanced method chosen was advanced dictionary
	ReadSimplePuzzle();
}
// Stores the dictionary for the advanced solver
void WordSearch::ReadAdvancedDictionary() {
	ifstream file(dictionaryName);
	string line;
	// Loops through file (dictionary) and stores current line in string 'line'
	while (getline(file, line)) {
		m_Adv_Dictionary.addWord(line);
	}
}
// Solves the puzzle using simple data structures
void WordSearch::SolvePuzzleSimple() {
	int i, j;
	for (i = 0; i < m_Grid_Size; i++) {
		for (j = 0; j < m_Grid_Size; j++) {
			const char c = m_Grid[j][i];
			// Updates Statisics
			m_Cells_Visited++;
			SimpleSearch(&c, &j, &i);
		}
	}
}
// Solves the puzzle using advanced data structures
void WordSearch::SolvePuzzleAdvanced() {
	int i, j;
	
	for (i = 0; i < m_Grid_Size; i++) {
		for (j = 0; j < m_Grid_Size; j++) {
			char c = m_Grid[j][i];			
			// Updates Statisics
			m_Cells_Visited++;
			AdvancedSearch(&c, &j, &i);
		}		
	}
}
// Writes the the statistics to the files
void WordSearch::WriteResults(const double loadTime, const double solveTime){
	if (m_Dictionary.size() == 0) {
		AdvancedWrite(loadTime, solveTime);
	}
	else {
		SimpleWrite(loadTime, solveTime);
	}
}
// Searches the grid of m_Grid using an algorithm which searches all directions
void WordSearch::SimpleSearch(const char* const letter, const int* const xPos, const int* const yPos) {

	int i;
	string curWord;
	// Loops through every word in the dictionary
	for (i = 0; i < static_cast<int>(m_Dictionary.size()); i++) {
		// Updates statistics
		m_Dictionary_Searches++;
		// Checks current words 1st letter against cell letter
		if (m_Dictionary[i][0] == *letter) {
			curWord = *letter;
			// If matched then processed to check surrounding letters
			// Sets directions to check, these are in pairs going left to right
			// e.g. TL, ML, BL -> MT, MB -> TR, MR, BR (8 combo's)
			int x[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
			int y[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
			int j;
			for (j = 0; j < 8; j++) {
				// Adds each pair of directions to current x,y position
				int newX = *xPos + x[j];
				int newY = *yPos + y[j];

				const int length = static_cast<int>(m_Dictionary[i].length());
				int len;
				for (len = 1; len < length; len++) {
					// Checks for out of bounds
					if (newX < 0 || newX >= m_Grid_Size || newY < 0 || newY >= m_Grid_Size) { break; }
					// Updates Statistics
					m_Cells_Visited++;
					// Checks if the next letter cell matches next letter in word
					if (m_Grid[newX][newY] == m_Dictionary[i][len]) {
						curWord += m_Dictionary[i][len];
						if (curWord == m_Dictionary[i]) {
							const string Position = to_string(*xPos) + " " + to_string(*yPos) + " ";
							m_Words_Found.push_back(m_Dictionary[i]);
							m_Words_Found_Pos.push_back(Position);
						}
						else {
							// Full word not found yet so increment new coords again
							newX += x[j];
							newY += y[j];
						}
					}
					else { len = length + 1; }
				}
				curWord = *letter;
			}
		}
	}
}
// Searches the grid of m_Grid using an algorithm which searches all directions
void WordSearch::AdvancedSearch(char* const letter, const int* const xPos, const int* const yPos) {
	Dictionary_Node* node = &m_Adv_Dictionary.getRoot();
	Dictionary_Node* prevNode;
	string curWord;
	int i;
	// Loops through every word in the dictionary
	const int size = static_cast<int>(m_Adv_Dictionary.getRoot().getChildren().size());
	for (i = 0; i < size; i++) {
		// Updates statistics
		m_Dictionary_Searches++;
		// Checks if the returned value from getDictionaryWord is the same as sent or child found
		if (addressof(m_Adv_Dictionary.getDictionaryWord(*node, *letter)) != addressof(*node)) {
			curWord = *letter;
			node = &m_Adv_Dictionary.getDictionaryWord(*node, *letter);
			// Sets previous node to current node as needed to bring loop back to bottom of tree
			prevNode = node;
			// If matched then processed to check surrounding letters
			// Sets directions to check, these are in pairs going left to right
			// e.g. TL, ML, BL -> MT, MB -> TR, MR, BR (8 combo's)
			int x[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
			int y[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
			int j;
			for (j = 0; j < 8; j++) {
				bool found = false;
				// Adds each pair of directions to current x,y position
				int newX = *xPos + x[j];
				int newY = *yPos + y[j];
				char* newLettter = &m_Grid[newX][newY];
				do {
					// Checks for out of bounds
					if (newX < 0 || newX >= m_Grid_Size || newY < 0 || newY >= m_Grid_Size) { break; }
					// Checks if the next letter cell matches next letter in word
					// Updates Statistics
					m_Cells_Visited++;
					int len;
					for (len = 0; len < static_cast<int>(node->getChildren().size()); len++) {
						if (m_Grid[newX][newY] == node->getChild(len).getLetter()) {
							// Updates string holding word its looking for
							curWord += node->getChild(len).getLetter();
							// Sets node to either the same node or the matching child to newLetter
							node = &m_Adv_Dictionary.getDictionaryWord(*node, *newLettter);
							// Set len so that it resets to 0 for the next node
							len = -1;
							
							// Increment newX & newY each loop to find next letter in that direction
							newX += x[j];
							newY += y[j];
							// Stops it breaking for words that end at an edge
							if (!node->getIsEnd()) {
								if (newX < 0 || newX >= m_Grid_Size || newY < 0 || newY >= m_Grid_Size) { break; }
								newLettter = &m_Grid[newX][newY];
							}
							// Updates Statistics
							m_Cells_Visited++;
							const bool hasChild = addressof(m_Adv_Dictionary.getDictionaryWord(*node, *newLettter)) != addressof(*node);
							if (!hasChild && node->getIsEnd()) {
								// Updates variables for statistics
								const string Position = to_string(*xPos) + " " + to_string(*yPos) + " ";
								m_Words_Found.push_back(curWord);
								m_Words_Found_Pos.push_back(Position);
								found = true;
								break;
							}
						}
					}
				} while (addressof(m_Adv_Dictionary.getDictionaryWord(*node, *newLettter)) != addressof(*node));
				if (!found) {
					curWord = *letter;
					node = prevNode;
				}
			}
		}
	}
}
// Writes the statistics for the simple functions into a txt file
void WordSearch::SimpleWrite(const double& loadTime, const double& solveTime) const {

	const string line1 = "NUMBER_OF_WORDS_MATCHED ", line2 = "WORDS_MATCHED_IN_GRID", line3 = "WORDS_UNMATCHED_IN_GIRD",
		line4 = "NUMBER_OF_GRID_CELLS_VISITED ", line5 = "NUMBER_OF_DICTIONARY_ENTRIES_VISITED ",
		line6 = "TIME_TO_POPULATE_GRID_STRUCTURE ", line7 = "TIME_TO_SOLVE_PUZZLE ";

	ofstream file(m_output);
	if (file.is_open()) {
		// Line 1
		file << line1 << to_string(static_cast<int>(m_Words_Found.size())) << "\n" << endl;
		// Line 2
		file << line2 << endl;
		for (int i = 0; i < static_cast<int>(m_Words_Found.size()); i++) {
			file << m_Words_Found_Pos[i] << m_Words_Found[i] << endl;
		}
		file << "" << endl;
		// Line 3
		file << line3 << endl;
		int i;
		for (i = 0; i < static_cast<int>(m_Dictionary.size()); i++) {
			if (find(m_Words_Found.begin(), m_Words_Found.end(), m_Dictionary[i]) == m_Words_Found.end()) {
				file << m_Dictionary[i] << to_string(i) << endl;
			}
		}
		file << "" << endl;
		// Line 4

		file << line4 << to_string(static_cast<int>(m_Cells_Visited)) << "\n" << endl;
		file << line5 << to_string(static_cast<int>(m_Dictionary_Searches)) << "\n" << endl;
		file << line6 << to_string(loadTime) << "\n" << endl;
		file << line7 << to_string(solveTime) << "\n" << endl;

	}
	file.close();
}
// Writes the statistics for the advanced functions into a txt file
void WordSearch::AdvancedWrite(const double& loadTime, const double& solveTime) {

	const string line1 = "NUMBER_OF_WORDS_MATCHED ", line2 = "WORDS_MATCHED_IN_GRID", line3 = "WORDS_UNMATCHED_IN_GIRD",
		line4 = "NUMBER_OF_GRID_CELLS_VISITED ", line5 = "NUMBER_OF_DICTIONARY_ENTRIES_VISITED ",
		line6 = "TIME_TO_POPULATE_GRID_STRUCTURE ", line7 = "TIME_TO_SOLVE_PUZZLE ";

	ofstream file(m_output);
	if (file.is_open()) {
		// Line 1
		file << line1 << to_string(static_cast<int>(m_Words_Found.size())) << "\n" << endl;
		// Line 2
		file << line2 << endl;
		for (int i = 0; i < static_cast<int>(m_Words_Found.size()); i++) {
			file << m_Words_Found_Pos[i] << m_Words_Found[i] << endl;
		}
		file << "" << endl;
		// Line 3
		file << line3 << endl;
		ReadSimpleDictionary();
		int i;
		for (i = 0; i < static_cast<int>(m_Dictionary.size()); i++) {
			if (find(m_Words_Found.begin(), m_Words_Found.end(), m_Dictionary[i]) == m_Words_Found.end()) {
				file << m_Dictionary[i] << to_string(i) << endl;
			}
		}
		file << "" << endl;
		// Line 4

		file << line4 << to_string(static_cast<int>(m_Cells_Visited)) << "\n" << endl;
		file << line5 << to_string(static_cast<int>(m_Dictionary_Searches)) << "\n" << endl;
		file << line6 << to_string(loadTime) << "\n" << endl;
		file << line7 << to_string(solveTime) << "\n" << endl;

	}
	file.close();
}
