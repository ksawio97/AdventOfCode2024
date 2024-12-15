#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

void turnRight(int& xIncr, int& yIncr) {
    if (xIncr == 0) {
        xIncr = -yIncr;
        yIncr = 0;
    } else {
        yIncr = xIncr;
        xIncr = 0;
    }
}

string getVisitedKey(int& row, int& col) {
    return to_string(row) + "-" + to_string(col);
}

bool travel(vector<string>& grid, int& col, int& row, int& xIncr, int& yIncr, char target, set<string>& visited) {
    while (row + yIncr < grid.size() && row + yIncr >= 0 && col + xIncr < grid[row + yIncr].size() && col + xIncr >= 0) {        
        if (grid[row + yIncr][col + xIncr] == target) {
            return true;
        }

        col += xIncr;
        row += yIncr;

        visited.insert(getVisitedKey(row, col));
    }

    return false;
}

tuple<int, int> findGuard(vector<string>& grid) {
    for (int r = 0; r < grid.size(); r++) {
        for (int c = 0; c < grid[r].size(); c++) {
            if (grid[r][c] == '^') {
                return { c, r };
            }
        }
    }

    return { -1, -1 };
}

int partOne(vector<string>& grid) {
    int col, row;
    tie(col, row) = findGuard(grid);

    set<string> visited{ getVisitedKey(row, col) };
    int xIncr = 0, yIncr = -1;
    while (travel(grid, col, row, xIncr, yIncr, '#', visited)) {
        turnRight(xIncr, yIncr);
    }

    return visited.size();
}

vector<string> readGrid(fstream& fh) {
    vector<string> grid{};
    string line;
    while (getline(fh, line)) {
        grid.push_back(line);
    }

    return grid;
}

int main() {
    fstream fh;
    fh.open("input.txt");
    auto grid = readGrid(fh);
    cout << "Part 1: " << partOne(grid) << endl;
    
    fh.close();

    
    return 0;
}