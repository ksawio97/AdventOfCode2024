#include <tuple>
#include <string>
#include <fstream>
#include <regex>
#include <iostream>
using namespace std;

vector<tuple<int, int>> cutNumbers(string line) {
    regex mul_regex("mul\\((\\d{1,3},\\d{1,3})\\)");

    auto words_begin = sregex_iterator(line.begin(), line.end(), mul_regex);
    auto words_end = sregex_iterator();
 
    vector<tuple<int, int>> found{};
    for (sregex_iterator i = words_begin; i != words_end; ++i)
    {
        smatch match = *i;
        string match_str = match.str();

        string numsAndComma = match_str.substr(4, match_str.size() - 5);
        int commaIndex = numsAndComma.find(',');

        tuple<int, int> nums = { stoi(numsAndComma.substr(0, commaIndex)), stoi(numsAndComma.substr(commaIndex + 1)) };
        found.push_back(nums);
    }
    return found;
}


int partOne(fstream& fh) {
    int numsSum = 0;
    string line;
    while (getline(fh, line)) {
        auto numsPairs = cutNumbers(line);
        for (auto current = numsPairs.begin(); current != numsPairs.end(); current++) {
            numsSum += get<0>(*current) * get<1>(*current);
        }
    }

    return numsSum;
}

int main() {
    fstream fh;
    fh.open("input.txt");

    cout << "Part 1: " << partOne(fh) << endl;

    fh.close();

    return 0;
}