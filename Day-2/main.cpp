#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <cctype>
#include <optional>
using namespace std;

int readNextNum(string line, int& start) {
    if (start == line.length()) {
        return -1;
    } 

    int numEnd = line.find_first_of(' ', start) + 1;
    // no more nums to read
    if (numEnd < start) {
        int startCopy = start;
        start = line.length();
        return stoi(line.substr(startCopy));
    }
    int num = stoi(line.substr(start, numEnd - start));

    start = numEnd;
    return num;
}

int partOne(fstream& fh) { 
    string line;
    int safeCount = 0;
    while (getline(fh, line)) {
        bool safe = true;
        int numStart = 0;
        int lastNum = readNextNum(line, numStart), num = readNextNum(line, numStart);
        int difference = num - lastNum;
        lastNum = num;
        if (abs(difference) < 1 || abs(difference) > 3) {
            continue;
        }
        bool increasing = difference > 0;

        while ((num = readNextNum(line, numStart)) != -1) {
            difference = num - lastNum;
            // check if its increasing and check if difference is from 1 to 3 
            if (((difference > 0) != increasing) || (abs(difference) < 1 || abs(difference) > 3)) {
                safe = false;
                break;
            }
            lastNum = num;
        }

        if (safe) {
            safeCount++;
        }
    }

    return safeCount;
}

bool isSafe(int prev, int num) {
    int diff = abs(num - prev);
    return 1 <= diff && diff <= 3;
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

int main()
{
    string path = "input.txt";
    fstream fh;
    fh.open(path);

    if (!fh.is_open()) {
        cerr << "Failed to open file " << endl;
        return 1;
    }

    
    cout << "Part 1: " << partOne(fh) << endl;
    fh.close();
    // safeCount = 0;
    fh.open(path);

    // cout << "Part 2: " << partTwo(fh) << endl;
    fh.close();
    return 0;
}