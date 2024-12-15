#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<string> getNums(string line) {
    int last = -1;
    size_t sep;
    vector<string> nums{};
    while ((sep = line.find(",", last + 1)) != string::npos) {
        nums.push_back(line.substr(last + 1, sep - last - 1));
        last = sep;
    }
    nums.push_back(line.substr(last + 1));
    return nums;
}

bool isLineValid(vector<string>& nums, map<string, set<string>>& order) {
    for (int start = 0; start < nums.size(); start++) {
        auto currNext = order[nums[start]];

        for (int next = start + 1; next < nums.size(); next++) {
            if (currNext.find(nums[next]) == currNext.end()) {
                return false;
            }
        }
    }

    return true;
}

void reorderNums(vector<string>& nums, map<string, set<string>>& order) {
    set<string> currNums(nums.begin(), nums.end());
    vector<string> ordered(nums.size());
    for (auto num : nums) {
        vector<string> intersections{};
        set_intersection(currNums.begin(), currNums.end(), order[num].begin(), order[num].end(), back_inserter(intersections));

        ordered[nums.size() - intersections.size() - 1] = num;
    }

    // assign ordered strings to nums
    for (int i = 0; i < nums.size(); i++) {
        nums[i] = ordered[i];
    }
}

int partTwo(vector<string>& lines, map<string, set<string>>& order) {
    int validMiddleSum = 0;
    for (auto line : lines) { 
        auto nums = getNums(line);
    
        if (!isLineValid(nums, order)) {
            // reorder
            reorderNums(nums, order);
            validMiddleSum += stoi(nums[int(nums.size() / 2)]);
        }
    }

    return validMiddleSum;
}
int partOne(vector<string>& lines, map<string, set<string>>& order) {
    int validMiddleSum = 0;
    for (auto line : lines) { 
        auto nums = getNums(line);
    
        if (isLineValid(nums, order)) {
            validMiddleSum += stoi(nums[int(nums.size() / 2)]);
        }
    }

    return validMiddleSum;
}

vector<string> readLines(fstream& fh) { 
    string line;
    vector<string> lines{};
    while (getline(fh, line)) {
        lines.push_back(line);
    }

    return lines;
}

map<string, set<string>> readOrder(fstream& fh) {
    string line;

    map<string, set<string>> order{};  
    while (getline(fh, line)) {
        if (line == "") {
            break;
        }
        int sep = line.find('|');
        string b = line.substr(0, sep), a = line.substr(sep + 1);
        if (order.find(b) == order.end()) {
            order[b] = {};
        }
        order[b].insert(a);
    }

    return order;
}

int main() {

    fstream fh;
    fh.open("input.txt");
    auto order = readOrder(fh);
    auto lines = readLines(fh);
    cout << "Part 1: " << partOne(lines, order) << endl;
    cout << "Part 2: " << partTwo(lines, order) << endl;
    fh.close();
    return 0;
}