#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

using namespace std;

vector<long> readStones(string filename) {
    vector<long> stones;
    fstream fh(filename);
    string line;
    getline(fh, line);
    fh.close();

    int last = 0;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ' ') {
            stones.push_back(stoi(line.substr(last, i - last)));
            last = i + 1;
        }
    }
    stones.push_back(stoi(line.substr(last)));

    return stones;
}

tuple<long, long> splitNumberInTwo(long num, int digits) {
    int breakPoint = digits / 2;
    long numberTwo = 0;

    int multiplier = 1;
    while (breakPoint > 0) {
        numberTwo += multiplier * (num % 10);
        multiplier *= 10;
        num /= 10;
        breakPoint--;
    }

    return make_tuple(num, numberTwo);
}

int countDigits(long num) {
    int count = 0;
    while (num > 0) {
        num /= 10;
        count++;
    }
    return count;
}

void blinkAtStone(vector<long>& stones, int& i) {
    if (stones[i] == 0) {
        stones[i] = 1;
        return;
    }

    int digits = countDigits(stones[i]);
    if (digits % 2 == 1) {
        stones[i] *= 2024;
        return;
    }

    int num1, num2;
    tie(num1, num2) = splitNumberInTwo(stones[i], digits);
    stones[i] = num1;
    stones.insert(stones.begin() + ++i /* ++i bcs its still part of this number */, num2);
}

int partOne(vector<long> stones, int blinks) {
    int i = 0;
    while (blinks > 0) {
        blinkAtStone(stones, i);

        i = (i + 1) % stones.size();
        // blink handled
        if (i == 0) {
            blinks--;
        }
    }


    return stones.size();
}

int partTwo() {
    // Implementation for part two
    return 0;
}

int main() {
    string filename = "input.txt";
    fstream fh(filename);
    vector<long> stones = readStones(filename);
    fh.close();
    cout << "Part 1: " << partOne(stones, 25) << endl;
    cout << "Part 2: " << partTwo() << endl;
    return 0;
}