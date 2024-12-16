#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <cctype>
#include <optional>
using namespace std;

bool isSafe(int prev, int num) {
    int diff = abs(num - prev);
    return 1 <= diff && diff <= 3;
}


int lineSafeFailIndex(vector<int>& nums) {
    int incrCount = 0;
    int lastIncr[2] = { -1, -1 };

    for (int i = 0; i < nums.size() - 1; i++) {
        if(!isSafe(nums[i], nums[i + 1])) {
            return i;
        }

        bool incr = nums[i] < nums[i + 1];
        incrCount += incr;
        lastIncr[incr] = i;
    }

    if (incrCount != 0 && incrCount != nums.size() - 1) {
        return lastIncr[incrCount < (nums.size() - incrCount - 1)];
    }

    return -1;
}

bool lineSafe(vector<int>& nums) {
    int incrCount = 0;
    for (int i = 0; i < nums.size() - 1; i++) {
        if(!isSafe(nums[i], nums[i + 1])) {
            return false;
        }

        incrCount += int(nums[i] < nums[i + 1]);
    }

    return incrCount == 0 || incrCount == nums.size() - 1;
}

int partOne(vector<vector<int>>& lines) {
    int safeCount = 0;
    for (auto nums : lines) {
        if (lineSafe(nums)) {
            safeCount += 1;
        }
    }

    return safeCount;
}

int partTwo(vector<vector<int>>& lines) {
    int safeCount = 0;
    for (auto nums : lines) {
        int failIndex;
        if ((failIndex = lineSafeFailIndex(nums)) == -1) {
            safeCount += 1;
        } else {
            vector<int> numsCopy1(nums.begin(), nums.end());
            vector<int> numsCopy2(nums.begin(), nums.end());

            numsCopy1.erase(numsCopy1.begin() + failIndex);
            numsCopy2.erase(numsCopy2.begin() + failIndex + 1);

            // check if removing one will change anything
            if (lineSafe(numsCopy1) || lineSafe(numsCopy2)) {
                safeCount += 1;
            }
        }
    }

    return safeCount;
}

// refactor part 1 so we can use this function for both parts
vector<int> loadLineAsList(string line) {
    vector<int> nums = {};

    int numStart = 0;
    int numEnd = line.find_first_of(' ');

    while (numEnd != -1) {
        string num = line.substr(numStart, numEnd - numStart);
        nums.push_back(stoi(num));

        numStart = numEnd + 1;
        numEnd = line.find(' ', numStart);
    }
    nums.push_back(stoi(line.substr(numStart, line.size() - numStart)));
    return nums;
}

vector<vector<int>> loadLinesAsLists(fstream& fh) {
    string line;
    vector<vector<int>> lines{};
    while (getline(fh, line)) {
        lines.push_back(loadLineAsList(line));
    }

    return lines;
}

int main()
{
    string path = "input.txt";
    fstream fh;
    fh.open(path);

    if (!fh.is_open()) {
        cerr << "Failed to open file " << endl;
        return 1;
    }

    auto lines = loadLinesAsLists(fh);
    fh.close();
    cout << "Part 1: " << partOne(lines) << endl;
    cout << "Part 2: " << partTwo(lines) << endl;

    return 0;
}