#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <set>
using namespace std;

string readDiskMap(string filename) {
    fstream fh;
    fh.open(filename);
    string line;
    getline(fh, line);
    fh.close();

    return line;
}

vector<int> unwrapDiskMap(string diskMap) {
    vector<int> unwrappedDiskMap{};

    for (int i = 0; i < diskMap.size(); i++) {
        const int num = diskMap[i] - '0';

        int fillWith;
        // add id
        if (i % 2 == 0) {
            fillWith = i / 2;
        // representation of a gap
        } else {
            fillWith = -1;
        }

        // fill
        for (int j = 0; j < num; j++) {
            unwrappedDiskMap.push_back(fillWith);
        }
    }

    return unwrappedDiskMap;
}

void fillGaps1(vector<int>& unwrappedDiskMap) {
    int end = unwrappedDiskMap.size();
    for (int i = 0; i < end; i++) {
        if (unwrappedDiskMap[i] != -1) {
            continue;
        }

        while (unwrappedDiskMap[--end] == -1) {}
        if (end <= i) {
            break;
        }
        iter_swap(unwrappedDiskMap.begin() + i, unwrappedDiskMap.begin() + end);
    }
}

int countFileSize(vector<int>& unwrappedDiskMap, int start, bool right) {
    const int iter = right ? 1 : -1;
    int end = start + iter;
    while (end < unwrappedDiskMap.size() && end >= 0 && unwrappedDiskMap[end] == unwrappedDiskMap[start]) {
        end += iter;
    }

    return abs(end - start);
}

void fillGapWithFile(vector<int>& unwrappedDiskMap, int fileStart, int fileSize, int end) {
    for (int gapStart = 0; gapStart < end; gapStart++) {
        if (unwrappedDiskMap[gapStart] != -1) {
            continue;
        }

        int gapSize = countFileSize(unwrappedDiskMap, gapStart, true);
        // can't fit
        if (gapSize < fileSize) {
            continue;
        }
        // fill with file
        for (int i = 0; i < fileSize; i++) {
            iter_swap(unwrappedDiskMap.begin() + gapStart + i, unwrappedDiskMap.begin() + fileStart + i);
        }

        break;
    }
}


void fillGaps2(vector<int>& unwrappedDiskMap) {
    set<int> filesMoved{};
    // look for files
    for (int i = unwrappedDiskMap.size() - 1; i >= 0; i--) {
        if (unwrappedDiskMap[i] == -1) {
            continue;
        }

        int size = countFileSize(unwrappedDiskMap, i, false);

        if (filesMoved.find(unwrappedDiskMap[i]) == filesMoved.end()) {
            filesMoved.insert(unwrappedDiskMap[i]);
            fillGapWithFile(unwrappedDiskMap, i - size + 1, size, i - size + 1);
        }

        i -= size - 1;
    }
}


long long checkSum(vector<int>& filledDiskMap) {
    long long checksum = 0;
    for (int i = 0; i < filledDiskMap.size(); i++) {
        if (filledDiskMap[i] == -1) {
            continue;
        }

        checksum += i * filledDiskMap[i];
    }

    return checksum;
}

int main() {
    string diskMap = readDiskMap("input.txt");   
    vector<int> unwrappedDiskMap = unwrapDiskMap(diskMap);
    vector<int> unwrappedDiskMapCopy(unwrappedDiskMap.begin(), unwrappedDiskMap.end());

    fillGaps1(unwrappedDiskMap);
    cout << "Part 1: " << checkSum(unwrappedDiskMap) << endl;

    fillGaps2(unwrappedDiskMapCopy);
    cout << "Part 2: " << checkSum(unwrappedDiskMapCopy) << endl;
    return 0;
}