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

int partOne(fstream& fh, map<string, set<string>>& order) {
    int validMiddleSum = 0;
    string line;
    while (getline(fh, line)) { 
        auto nums = getNums(line);
        bool count = true;
        for (int start = 0; start < nums.size(); start++) {
            auto currNext = order[nums[start]];

            for (int next = start + 1; next < nums.size(); next++) {
                if (currNext.find(nums[next]) == currNext.end()) {
                    // invalidate this line
                    start = nums.size();
                    count = false;
                    break;
                }
            }
        }

        if (count) {
            validMiddleSum += stoi(nums[int(nums.size() / 2)]);
        }
    }

    return validMiddleSum;
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
    cout << partOne(fh, order) << endl;
    fh.close();
    return 0;
}