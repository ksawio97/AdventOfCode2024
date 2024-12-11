#include <tuple>
#include <string>
#include <fstream>
#include <regex>
#include <iostream>
#include <optional>
using namespace std;


class IncludeInfo {
private:
    ptrdiff_t position;
    bool include;
public:
    IncludeInfo(ptrdiff_t position, bool include) {
        this->position = position;
        this->include = include;
    }
    
    ptrdiff_t getPosition() {
        return this->position;
    }

    bool getInclude() {
        return this->include;
    }
};

vector<tuple<int, int>> cutNumbers(string line, vector<IncludeInfo> includeInfos = {}) {
    regex mul_regex("mul\\((\\d{1,3},\\d{1,3})\\)");

    auto words_begin = sregex_iterator(line.begin(), line.end(), mul_regex);
    auto words_end = sregex_iterator();
    
    bool include = true;
    ptrdiff_t includeBreakpoint;

    int currentIncludeIndex = 0;
    // set next breakpoint
    if (includeInfos.size() > 0) { 
        includeBreakpoint = includeInfos[0].getPosition();
    } else {
        includeBreakpoint = line.length();
    }

    vector<tuple<int, int>> found{};
    for (sregex_iterator i = words_begin; i != words_end; ++i)
    {
        smatch match = *i;
        // change include status
        if (match.position() > includeBreakpoint) {
            include = includeInfos[currentIncludeIndex].getInclude();
            // set next breakpoint
            if (includeInfos.size() > ++currentIncludeIndex) { 
                includeBreakpoint = includeInfos[currentIncludeIndex].getPosition();
            } else {
                includeBreakpoint = line.length();
            }
        }

        // skip
        if (!include) {
            continue;
        }

        string match_str = match.str();
        string numsAndComma = match_str.substr(4, match_str.size() - 5);
        int commaIndex = numsAndComma.find(',');

        tuple<int, int> nums = { stoi(numsAndComma.substr(0, commaIndex)), stoi(numsAndComma.substr(commaIndex + 1)) };
        found.push_back(nums);
    }
    return found;
}

int multiplyPairsAndSum(vector<tuple<int, int>> numsPairs) {
    int numsSum = 0;
    for (auto current = numsPairs.begin(); current != numsPairs.end(); current++) {
        numsSum += get<0>(*current) * get<1>(*current);
    }
    return numsSum;
}

int partOne(fstream& fh) {
    int numsSum = 0;
    string line;
    while (getline(fh, line)) {
        auto numsPairs = cutNumbers(line);
        numsSum += multiplyPairsAndSum(numsPairs);
    }

    return numsSum;
}

vector<IncludeInfo> findIncludeInfoStarts(string line) {
    regex pattern(R"(do\(\)|don't\(\))");

    auto start = sregex_iterator(line.begin(), line.end(), pattern);
    auto end = sregex_iterator();

    vector<IncludeInfo> matches{};
    for (; start != end; start++) {
        smatch match = *start;
        matches.emplace_back(match.position(), match.str() == "do()");
    }
    return matches;
}
int partTwo(fstream& fh) {
    int numsSum = 0;

    IncludeInfo lastIncludeInfo = *(new IncludeInfo(0, true));
    string line;
    while (getline(fh, line)) {
        auto includeInfos = findIncludeInfoStarts(line);
        includeInfos.insert(includeInfos.begin(), lastIncludeInfo);

        // remember last lines includeInfo
        lastIncludeInfo = *(new IncludeInfo(0, includeInfos.empty() ? true : includeInfos.back().getInclude()));

        auto numsPairs = cutNumbers(line, includeInfos);
        numsSum += multiplyPairsAndSum(numsPairs);
    }

    return numsSum;
}

int main() {
    string path = "input.txt";
    fstream fh;
    fh.open(path);

    cout << "Part 1: " << partOne(fh) << endl;
    fh.close();

    fh.open(path);
    cout << "Part 2: " << partTwo(fh) << endl;

    fh.close();
    return 0;
}