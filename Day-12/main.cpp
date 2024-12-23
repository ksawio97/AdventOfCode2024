#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <map>
#include <tuple>
using namespace std;


class Plot {
public:
    bool isVisited;
    int r, c;
    char value;
    int gardenIndex;

    Plot(int r, int c, char value) : r(r), c(c), isVisited(false), gardenIndex(-1), value(value) {}

    bool hasGarden() {
        return this->gardenIndex != -1;
    }
};

class Garden {
public:
    char value;
    int index;
    int perimeter;
    int area;
    Garden(char value, int index) : value(value), area(0), perimeter(0), index(index) {}

    tuple<char, int> getKey() {
        return make_tuple(this->value, this->index);
    }
} ;

class Grid {
private:
    vector<vector<shared_ptr<Plot>>> grid{};
    // used for assigning garden index
    map<char, int> gardensCount{};
    // store gardens perimeter and area
    map<tuple<char, int>, shared_ptr<Garden>> gardens{};
public:
    Grid(vector<string> grid) {
        for (int r = 0; r < grid.size(); r++) {
            this->grid.push_back(vector<shared_ptr<Plot>>());
            for (int c = 0; c < grid[r].size(); c++) {
                auto plot = make_shared<Plot>(r, c, grid[r][c]);
                this->grid[r].push_back(plot);
            }
        }
    }

    Plot* getPlot(int r, int c) {
        if (r < 0 || r >= this->grid.size() || c < 0 || c >= this->grid[0].size()) {
            return nullptr;
        }
        return this->grid[r][c].get();
    }

    vector<Plot*> countGardenNeighborsAndExtendGarden(Plot* plot) 
    {
        int r = plot->r, c = plot->c;
        vector<Plot*> neighbors{};

        for (auto neighbour : {getPlot(r - 1, c), getPlot(r + 1, c), getPlot(r, c - 1), getPlot(r, c + 1)}) {
            // count as plot
            if (neighbour == nullptr || neighbour->value != plot->value) {
                continue;
            // extend garden
            } else {
                neighbour->gardenIndex = plot->gardenIndex;
                neighbors.push_back(neighbour);
            }
        }

        return neighbors;
    }

    void visitPlot(int r, int c) {
        Plot* plot = getPlot(r, c);
        if (plot == nullptr || plot->isVisited) {
            return;
        }
        plot->isVisited = true;
        if (!plot->hasGarden()) {
            // add plot to new garden
            plot->gardenIndex = getNewGardenIndex(plot->value);
        }
        auto gardenKey = make_tuple(plot->value, plot->gardenIndex);

        auto gardenNeighbors = countGardenNeighborsAndExtendGarden(plot);
        // add perimeter to garden
        gardens[gardenKey]->perimeter += 4 - gardenNeighbors.size();
        // extend garden area
        gardens[gardenKey]->area++;

        for (auto gardenNeighbor : gardenNeighbors) {
            visitPlot(gardenNeighbor->r, gardenNeighbor->c);
        }
    }

    int getNewGardenIndex(char value) {
        if (this->gardensCount.find(value) == this->gardensCount.end()) {
            this->gardensCount[value] = 0;
        }
        int index = this->gardensCount[value];
        this->gardens[make_tuple(value, index)] = make_shared<Garden>(value, index);
        return this->gardensCount[value]++;
    }

    int getGardensPrices() {
        int price = 0;
        for (auto garden : this->gardens) {
            price += garden.second->area * garden.second->perimeter;
        }
        return price;
    }
};

vector<string> readGrid(string filename) {
    vector<string> grid{};
    fstream fh(filename);
    string line;
    while (getline(fh, line)) {
        grid.push_back(line);
    }
    fh.close();

    return grid;
}

int partOne(vector<string> grid) {
    Grid gardenGrid(grid);
    for (int r = 0; r < grid.size(); r++) {
        for (int c = 0; c < grid[r].size(); c++) {
            gardenGrid.visitPlot(r, c);
        }
    }
    return gardenGrid.getGardensPrices();
}

int main() {
    string filename = "input.txt";
    vector<string> grid = readGrid(filename);
    cout << "Part 1: " << partOne(grid) << endl;

    return 0;
}