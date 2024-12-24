#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <bits/stdc++.h>
using namespace std;

class Button {
public:
    int xMove, yMove;
    Button(int xMove, int yMove) : xMove(xMove), yMove(yMove) {}
};

class Prize {
    public:
    int x, y;
    Prize(int x, int y) : x(x), y(y) {}
};

class ClawMachine {
    Button a, b;
    Prize prize;
public:
    ClawMachine(Button a, Button b, Prize prize) : a(a), b(b), prize(prize) {}

    tuple<int, int> getPossibleButtonPresses() { 
        int up = a.xMove * prize.y - prize.x * a.yMove;
        int down = a.xMove * b.yMove - b.xMove * a.yMove;
        if (up % down != 0) {
            return { -1, -1 };
        }
        float clicksB = up / down;
        float clicksA = (prize.y - b.yMove * clicksB) / a.yMove;

        return { int(clicksA), int(clicksB) };
    }

    bool isPossible(int clicksA, int clicksB) {
        return 0 <= clicksA && clicksA <= 100 && 0 <= clicksB && clicksB <= 100;
    }
};

void readNextNums(const string& line, int* data, int& currDataIndex) {
    regex re(R"(\d+)");
    auto numsBegin = sregex_iterator(line.begin(), line.end(), re);
    auto numsEnd = sregex_iterator();

    for (auto i = numsBegin; i != numsEnd; ++i) {
        smatch match = *i;
        data[currDataIndex++] = stoi(match.str());
    }
}


vector<ClawMachine> readMachinesData(string filepath) {
    vector<ClawMachine> machines{};
    fstream fh(filepath);
    string line;
    
    int currDataIndex = 0;
    int data[6]{};
    while (getline(fh, line)) {
        // add machine
        if (line == "") {
            machines.emplace_back(Button(data[0], data[1]), Button(data[2], data[3]), Prize(data[4], data[5]));
            currDataIndex = 0;
            continue;
        }

        readNextNums(line, data, currDataIndex);
    }
    machines.emplace_back(Button(data[0], data[1]), Button(data[2], data[3]), Prize(data[4], data[5]));
    fh.close();
    return machines;
}

int partOne(vector<ClawMachine>& machines) {
    int tokens = 0;
    for (auto& machine : machines) {
        auto clicks = machine.getPossibleButtonPresses();
        if (machine.isPossible(get<0>(clicks), get<1>(clicks))) {
            tokens += get<0>(clicks) * 3 + get<1>(clicks);
        }
    }
    return tokens;
}
int main() {
    auto machines = readMachinesData("input.txt");
    cout << "Part 1: " << partOne(machines) << endl;
    return 0;
}