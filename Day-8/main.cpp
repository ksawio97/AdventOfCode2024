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



int partOne(Size& size) {
    vector<char> keys = Frequency::getFrequencyKeys();
    set<string> antinodes{};
    for (char& key : keys) {
        auto positions = Frequency::getFrequency(key)->getPositions();
        // check all combinations of positions
        for (int i = 0; i < positions.size(); i++) {
            for (int j = i + 1; j < positions.size(); j++) {
                // look for the antinode
                Pos newPos = positions[i]->getDoubleDifferencePos(positions[j]);
                if (size.posInRange(newPos))
                    antinodes.insert(newPos.tokenize());
                newPos = positions[j]->getDoubleDifferencePos(positions[i]);
                if (size.posInRange(newPos))
                    antinodes.insert(newPos.tokenize());
            }
        }
    }

    return antinodes.size();
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
    cout << partOne(size) << endl;
    fh.close();
    return 0;
}