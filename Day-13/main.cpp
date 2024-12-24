#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <bits/stdc++.h>
using namespace std;

class Button {
public:
    long long xMove, yMove;
    Button(long long xMove, long long yMove) : xMove(xMove), yMove(yMove) {}
};

class Prize {
    public:
    long long x, y;
    Prize(long long x, long long y) : x(x), y(y) {}
};

class ClawMachine {
    Button a, b;
public:
    Prize prize;
    ClawMachine(Button a, Button b, Prize prize) : a(a), b(b), prize(prize) {}

    tuple<long long, long long> getPossibleButtonPresses() { 
        double up = static_cast<double>(a.xMove) * prize.y - prize.x * a.yMove;
        double down = static_cast<double>(a.xMove) * b.yMove - b.xMove * a.yMove;
        if (floor(up / down) != up / down) {
            return { 0, 0 };
        }
        long long clicksB = up / down;
        
        double numerator = static_cast<double>(prize.y) - b.yMove * clicksB;
        if (floor(numerator / a.yMove) != numerator / a.yMove) {
            return { 0, 0 };
        }
        long long clicksA = numerator / a.yMove;

        return { clicksA, clicksB };
    }

    bool isInLimit(long long clicksA, long long clicksB) {
        return 0 <= clicksA && clicksA <= 100 && 0 <= clicksB && clicksB <= 100;
    }
};

void readNextNums(const string& line, long long* data, long long& currDataIndex) {
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
    
    long long currDataIndex = 0;
    long long data[6]{};
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

long long core(vector<ClawMachine>& machines, bool checkLimit) {
    long long tokens = 0;
    for (auto& machine : machines) {
        auto clicks = machine.getPossibleButtonPresses();
        if (!checkLimit || machine.isInLimit(get<0>(clicks), get<1>(clicks))) {
            tokens += get<0>(clicks) * 3 + get<1>(clicks);
        }
    }
    return tokens;
}

int main() {
    auto machines = readMachinesData("input.txt");
    cout << "Part 1: " << core(machines, true) << endl;
    for (auto& machine : machines) {
        machine.prize.x += 10000000000000;
        machine.prize.y += 10000000000000;
    }
    cout << "Part 2: " << core(machines, false) << endl;
    return 0;
}