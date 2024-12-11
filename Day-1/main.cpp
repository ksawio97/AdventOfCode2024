#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
using namespace std;

class SortedArray {
private:
    vector<int> ids = {};
    // no need to delete ids from vector
    int first = 0;
public:
    int getFirstNumber() {
        return first < ids.size() ? ids[first++] : -1;
    }

    void addId(int id) {
        // binary search find index for id
        const int n = ids.size();
        int first = 0, last = n - 1;

        while (first <= last) {
            const int mid = (first + last) / 2;
            if (ids[mid] < id) {
                first = mid + 1;
            } else if (ids[mid] > id) {
                last = mid - 1;
            } else {
                first = mid;
                break;
            }
        }

        ids.insert(ids.begin() + first, id);
    }

    void resetFirstNumber() {
        first = 0;
    }
};


tuple<int, int> getIDs(string line) {
    int firstEnd = line.find_first_of(' ');
    int lastStart = line.find_last_of(' ');

    return { stoi(line.substr(0, firstEnd)), stoi(line.substr(lastStart + 1, line.length() - lastStart)) };
}

int main()
{
    string path = "input.txt";
    ifstream fh;
    fh.open(path);


    if (!fh.is_open()) {
        std::cerr << "Failed to open file " << path << std::endl;
        return 1; 
    }


    SortedArray list1 = SortedArray(), list2 = SortedArray();

    // read input and insert it to lists
    string line;
    int first, second;
    int linesCount = 0;
    while (getline(fh, line)) {
        tie(first, second) = getIDs(line);

        list1.addId(first); list2.addId(second);

        linesCount++;
    }
    
    fh.close();

    int difference = 0;
    while (linesCount-- > 0) {
        difference += abs(list1.getFirstNumber() - list2.getFirstNumber());
    }
    cout << "Part 1: " << difference << endl;

    list1.resetFirstNumber(); list2.resetFirstNumber();

    int similarity = 0;
    int id1 = list1.getFirstNumber(), id2 = list2.getFirstNumber();
    int lastId1 = -1, lastCount = 0;

    int iter = 1;
    while (id1 != -1 && id2 != -1) {
        // id1 is read again, so add same score
        if (id1 == lastId1) {
            similarity += lastId1 * lastCount;
            id1 = list1.getFirstNumber();
            continue;
        }

        // find when id1 appears in list2
        while (id2 != -1 && id1 > id2) {
            id2 = list2.getFirstNumber();
        }

        // count id1 occurrences
        int count = 0;
        while (id1 == id2) {
            count++;
            id2 = list2.getFirstNumber();
        }

        similarity += id1 * count;
        // iterate
        lastId1 = id1;
        lastCount = count;

        id1 = list1.getFirstNumber();
    }

    cout << "Part 2: " << similarity << endl;

    return 0;
}