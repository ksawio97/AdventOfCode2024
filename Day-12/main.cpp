#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <map>
#include <tuple>
#include <set>
using namespace std;

enum Sides {
    TOP = 0,
    BOTTOM = 1,
    LEFT = 2,
    RIGHT = 3
};

class Plot {
public:
    set<Sides> perimeters{};
    bool isVisited;
    int r, c;
    char value;
    int gardenIndex;

    Plot(int r, int c, char value) : r(r), c(c), isVisited(false), gardenIndex(-1), value(value) {}

    bool hasGarden() {
        return this->gardenIndex != -1;
    }

    void addPerimeters(vector<Plot*>& gardenNeighbors) {
        set<Sides> sides = {TOP, BOTTOM, LEFT, RIGHT};
        for (auto& gardenNeighbor : gardenNeighbors) {
            // check where should be perimeter 
            if (gardenNeighbor->r < this->r) {
                sides.erase(TOP);
            } else if (gardenNeighbor->r > this->r) {
                sides.erase(BOTTOM);
            } else if (gardenNeighbor->c < this->c) {
                sides.erase(LEFT);
            } else if (gardenNeighbor->c > this->c) {
                sides.erase(RIGHT);
            }
        }

        this->perimeters.insert(sides.begin(), sides.end());
    }
};

class Garden {
public:
    char value;
    int index;
    int perimeter;
    int area;
    int sides;
    Garden(char value, int index) : value(value), area(0), perimeter(0), sides(0), index(index) {}

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

        plot->addPerimeters(gardenNeighbors);
        // add perimeter to garden
        gardens[gardenKey]->perimeter += plot->perimeters.size();
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

    int getGardensPrices1() {
        int price = 0;
        for (auto garden : this->gardens) {
            price += garden.second->area * garden.second->perimeter;
        }
        return price;
    }

    void deleteSideFromPlots(Plot* plot, set<Sides> sides, Sides goTo) {
        int rIncr = goTo == TOP ? -1: goTo == BOTTOM ? 1 : 0, cIncr = goTo == LEFT ? -1 : goTo == RIGHT ? 1 : 0;
        
        Plot* delFrom = getPlot(plot->r, plot->c);
        while ((delFrom = getPlot(delFrom->r + rIncr, delFrom->c + cIncr)) != nullptr && sides.size() > 0 && delFrom->value == plot->value) {
            set<Sides> nextSides{};
            for (auto side : sides) {
                int count = delFrom->perimeters.erase(side);
                if (count > 0) {
                    nextSides.insert(side);
                }
            }
            // we replace them with next Sides bcs side might end earlier
            sides = nextSides;
        }
    }

    void countSides(Plot* plot) {
        if (plot->perimeters.size() == 0) {
            return;
        }
        tuple<char, int> gardenKey = make_tuple(plot->value, plot->gardenIndex);
        gardens[gardenKey]->sides += plot->perimeters.size();

        for (auto sideToDel : {TOP, BOTTOM, LEFT, RIGHT}) {
            // if its not in perimeters then it might be in the same garden, delete
            if (plot->perimeters.find(sideToDel) == plot->perimeters.end()) {
                deleteSideFromPlots(plot, plot->perimeters, sideToDel);
            }
        }

        plot->perimeters.clear();
    }

    int getGardensPrices2() {

        for (int r = 0; r < this->grid.size(); r++) {
            for (int c = 0; c < this->grid[r].size(); c++) {
                Plot* plot = getPlot(r, c);
                countSides(plot);
            }
        }

        int price = 0;
        for (auto garden : this->gardens) {
            price += garden.second->area * garden.second->sides;
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

Grid core(vector<string> grid) {
    Grid gardenGrid(grid);
    for (int r = 0; r < grid.size(); r++) {
        for (int c = 0; c < grid[r].size(); c++) {
            gardenGrid.visitPlot(r, c);
        }
    }
    return gardenGrid;
}

int main() {
    string filename = "input.txt";
    vector<string> grid = readGrid(filename);
    Grid gardenGrid = core(grid);
    cout << "Part 1: " << gardenGrid.getGardensPrices1() << endl;
    cout << "Part 2: " << gardenGrid.getGardensPrices2() << endl;

    return 0;
}