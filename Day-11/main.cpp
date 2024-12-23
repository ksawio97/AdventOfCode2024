#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <map>

using namespace std;

vector<long long> readStones(string filename) {
    vector<long long> stones;
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

tuple<long long, long long> splitNumberInTwo(long long num, int digits) {
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

int countDigits(long long num) {
    int count = 0;
    while (num > 0) {
        num /= 10;
        count++;
    }
    return count;
}

void blinkAtStones(vector<long long>& stones, int& i) {
    if (stones[i] == 0) {
        stones[i] = 1;
        return;
    }

    int digits = countDigits(stones[i]);
    if (digits % 2 == 1) {
        stones[i] *= 2024;
        return;
    }

    long long num1, num2;
    tie(num1, num2) = splitNumberInTwo(stones[i], digits);
    stones[i] = num1;
    stones.insert(stones.begin() + ++i /* ++i bcs its still part of this number */, num2);
}

int partOne(vector<long long> stones, int blinks) {
    int i = 0;
    while (blinks > 0) {
        blinkAtStones(stones, i);

        i = (i + 1) % stones.size();
        // blink handled
        if (i == 0) {
            blinks--;
        }
    }


    return stones.size();
}

long long blinkAtStoneMemo(long long stone, int blinks) {
    static map<tuple<long long, long long>, long long> memo;
    // one bcs we count initial stone
    if (blinks == 0) {
        return 1;
    }

    auto key = make_tuple(stone, blinks);

    if (memo.find(key) != memo.end()) {
        return memo[key];
    }
    long long result = 0;
    if (stone == 0) {
        result = blinkAtStoneMemo(1, blinks - 1);
        memo[key] = result;
        return result;
    }
    int digits = countDigits(stone);
    if (digits % 2 == 1) {
        result = blinkAtStoneMemo(stone * 2024, blinks - 1);
        memo[key] = result;
        return result;
    }

    long long num1, num2;
    tie(num1, num2) = splitNumberInTwo(stone, digits);
    result = blinkAtStoneMemo(num1, blinks - 1) + blinkAtStoneMemo(num2, blinks - 1);
    memo[key] = result;
    // added new one while splitting number
    return result;
}


long long partTwo(vector<long long> stones, int blinks) {
    long long result = 0;

    for (auto& stone : stones) {
        result += blinkAtStoneMemo(stone, blinks);
    }

    return result;
}

int main() {
    string filename = "input.txt";
    fstream fh(filename);
    vector<long long> stones = readStones(filename);
    fh.close();
    cout << "Part 1: " << partOne(stones, 25) << endl;
    cout << "Part 2: " << partTwo(stones, 75) << endl;
    return 0;
}