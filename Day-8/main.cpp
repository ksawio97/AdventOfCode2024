#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <set>
#include <iostream>
using namespace std;

class Pos {
public:
    int x, y;
    Pos(int x, int y) {
        this->x = x;
        this->y = y;
    }

    Pos getDoubleDifferencePos(Pos* other) {
        return Pos(x - (other->x - x), y - (other->y - y));
    }

    bool inRange(int rows, int cols) {
        return x >= 0 && x < cols && y >= 0 && y < rows;
    }
    
    string tokenize() {
        return to_string(x) + "," + to_string(y);
    }
};

class Size {
public:
    int width, height;

    Size(int width, int height) {
        this->width = width;
        this->height = height;
    }

    virtual bool posInRange(Pos& pos) {
        return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
    }
};

class Frequency {
private:
    static map<char, shared_ptr<Frequency>> frequencies;
    Frequency() {}
    Frequency(const Frequency&) = default;
    Frequency& operator=(const Frequency&) = default;

    vector<shared_ptr<Pos>> positions;
public:
    vector<Pos*> getPositions() {
        vector<Pos*> pos;
        for (auto& p : positions) {
            pos.push_back(p.get());
        }
        return pos;
    }

    void addPosition(int x, int y) {
        auto position = shared_ptr<Pos>(new Pos(x, y));
        positions.emplace_back(position);
    }

    static Frequency* getFrequency(char key) {
        if (frequencies.find(key) == frequencies.end()) {
            auto frequency = shared_ptr<Frequency>(new Frequency());
            frequencies.emplace(key, frequency);
        }

        return frequencies[key].get();
    }

    static vector<char> getFrequencyKeys() {
        vector<char> keys;
        for (auto it = frequencies.begin(); it != frequencies.end(); it++) {
            keys.push_back(it->first);
        }
        return keys;
    }
};
map<char, shared_ptr<Frequency>> Frequency::frequencies{};

void lookForAntiNodes(Pos* pos1, Pos* pos2, Size& size, set<string>& antinodes) {
    Pos newPos = pos1->getDoubleDifferencePos(pos2);
    if (size.posInRange(newPos)) {
        antinodes.insert(newPos.tokenize());
    }
    newPos = pos2->getDoubleDifferencePos(pos1);
    if (size.posInRange(newPos)) {
        antinodes.insert(newPos.tokenize());
    }
}

// pos 2 should always be in range, we don't need to check it
void lookForAntiNodesRecursive(Pos* pos1, Pos* pos2, Size& size, set<string>& antinodes) {
    if (!size.posInRange(*pos1)) {
        return;
    }
    antinodes.insert(pos1->tokenize());
    Pos newPos = pos1->getDoubleDifferencePos(pos2);
    lookForAntiNodesRecursive(&newPos, pos1, size, antinodes);
}

void lookForAntiNodesRecursiveStart(Pos* pos1, Pos* pos2, Size& size, set<string>& antinodes) {
    lookForAntiNodesRecursive(pos1, pos2, size, antinodes);
    // check opposite direction
    lookForAntiNodesRecursive(pos2, pos1, size, antinodes);
}

int checkAllCombinations(Size& size, void (getAntiNodes)(Pos*, Pos*, Size&, set<string>&)) {
    vector<char> keys = Frequency::getFrequencyKeys();
    set<string> antinodes{};
    for (char& key : keys) {
        auto positions = Frequency::getFrequency(key)->getPositions();
        for (int i = 0; i < positions.size(); i++) {
            for (int j = i + 1; j < positions.size(); j++) {
                getAntiNodes(positions[i], positions[j], size, antinodes);
            }
        }
    }

    return antinodes.size();
}

int partOne(Size& size) {
    return checkAllCombinations(size, lookForAntiNodes);
}

int partTwo(Size& size) {
    return checkAllCombinations(size, lookForAntiNodesRecursiveStart);
}

Size loadFrequencies(fstream &fh) {
    string line;
    int row = 0;
    int cols = 0;
    while (getline(fh, line)) {        
        int col = 0;
        for (; col < line.size(); col++) {
            if (line[col] != '.') {
                Frequency::getFrequency(line[col])->addPosition(col, row);
            }
        }
        cols = col;
        row++;
    }
    return Size(cols, row);
}


int main() {
    fstream fh;
    fh.open("input.txt");
    auto size = loadFrequencies(fh);
    cout << "Part 1: " << partOne(size) << endl;
    cout << "Part 2: " << partTwo(size) << endl;
    fh.close();
    return 0;
}