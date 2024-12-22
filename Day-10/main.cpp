#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <memory>
using namespace std;

class Node {
public:
    int x;
    int y;
    int value;
    set<Node*> found{};

    Node(int x, int y, int value) : x(x), y(y), value(value) {}
};

class Grid {
    vector<vector<shared_ptr<Node>>> grid{};
    vector<Node*> starts{};
public:

    Grid(vector<string> grid) {
        for (int y = 0; y < grid.size(); y++) {
            this->grid.push_back({});
            for (int x = 0; x < grid[y].size(); x++) {
                int value = grid[y][x] == '.' ? -1 : grid[y][x] - '0';
                shared_ptr<Node> n = make_shared<Node>(x, y, value);
                this->grid[y].push_back(n);

                if (value == 0) {
                    starts.push_back(n.get());
                }
            }
        }
    }

    vector<Node*> getStarts() {
        return starts;
    }

    vector<Node*> getValidNeighbors(Node* node) {
        vector<Node*> neighbors{};
        int lookForValue = node->value + 1;

        if (node->x > 0 && grid[node->y][node->x - 1]->value == lookForValue) {
            neighbors.push_back(grid[node->y][node->x - 1].get());
        }
        if (node->x < grid[node->y].size() - 1 && grid[node->y][node->x + 1]->value == lookForValue) {
            neighbors.push_back(grid[node->y][node->x + 1].get());
        }
        if (node->y > 0 && grid[node->y - 1][node->x]->value == lookForValue) {
            neighbors.push_back(grid[node->y - 1][node->x].get());
        }
        if (node->y < grid.size() - 1 && grid[node->y + 1][node->x]->value == lookForValue) {
            neighbors.push_back(grid[node->y + 1][node->x].get());
        }

        return neighbors;
    }

    set<Node*> findPaths(Node* node) {
        // found target
        if (grid[node->y][node->x]->value == 9) {
            return { node };
        }

        // already found paths, (memorized result)
        if (!node->found.empty()) {
            return node->found;
        }

        vector<Node*> neighbors = getValidNeighbors(node);
        // check next nodes
        for (auto& neighbor : neighbors) {
            set<Node*> found = findPaths(neighbor);
            node->found.insert(found.begin(), found.end());
        }

        return node->found;
    }
};

vector<string> readGrid(string filename) {
    fstream fh;
    fh.open(filename);
    vector<string> grid;
    string line;
    while (getline(fh, line)) {
        grid.push_back(line);
    }
    fh.close();

    return grid;
}

int partOne(Grid& g) {
    int paths = 0;
    auto starts = g.getStarts();
    for (auto& start : starts) {
        g.findPaths(start);
        paths += start->found.size();
    }
    
    return paths;
}

int main() {
    string filename = "input.txt";
    vector<string> grid = readGrid(filename);
    Grid g(grid);
    cout << "Part 1: "<< partOne(g) << endl;

    return 0;
}