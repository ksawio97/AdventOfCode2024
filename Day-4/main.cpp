#include <fstream>
#include <vector>
#include <string>
#include <iostream>
 
using namespace std;
 
class FindWord {
private:
    vector<string> grid;
public:
    FindWord(vector<string>&& grid) {
        this->grid = move(grid);
    }
 
    bool matches(int row, int col, string word, int iterRow, int iterCol) {
        for (int i = 0; i < word.size(); i++) {
            int currRow = row + i * iterRow;
            int currCol = col + i * iterCol;
 
            if (word[i] != grid[currRow][currCol]) {
                return false;
            }
        }
        return true;
    }
 
    int checkWordStart(int row, int col, string word) {
        int wordCount = 0;

        int wordSize = word.size();
        bool left = (col - wordSize) >= -1, top = (row - wordSize) >= -1, right = (col + wordSize) <= this->grid.size(), bottom = (row + wordSize) <= this->grid.size();
        if (left) {
            wordCount += this->matches(row, col, word, 0, -1);
        }
 
        if (top && left) {
            wordCount += this->matches(row, col, word, -1, -1);
        }
 
        if (top) {
            wordCount += this->matches(row, col, word, -1, 0);
        }
 
        if (top && right) {
            wordCount += this->matches(row, col, word, -1, 1);
        }
 
        if (right) {
            wordCount += this->matches(row, col, word, 0, 1);
        }
 
        if (bottom && right) {
            wordCount += this->matches(row, col, word, 1, 1);
        }
 
        if (bottom) {
            wordCount += this->matches(row, col, word, 1, 0);
        }
 
        if (bottom && left) {
            wordCount += this->matches(row, col, word, 1, -1);
        }
 
        return wordCount;
    }
};
 
vector<string> loadGrid(fstream& fh) {
    string line;
    vector<string> grid = {};
    while (getline(fh, line)) {
        grid.push_back(line);
    }
 
    return grid;
}
 
int partOne(vector<string> grid, string word) {
    int wordCount = 0;
    int rows = grid.size(), cols = grid[0].size();
    FindWord* findWord = new FindWord(move(grid));
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int count = findWord->checkWordStart(r, c, word);
            wordCount += count;
        }
    }

    return wordCount;
}
 
int main()
{
    fstream fh;
    fh.open("input.txt");
    vector<string> grid = loadGrid(fh);
    cout << "Part 1: " << partOne(grid, "XMAS") << endl;
    fh.close();
    return 0;
}