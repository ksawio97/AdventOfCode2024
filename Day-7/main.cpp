#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

class Equation {
public: 
    unsigned long target;
    vector<unsigned long> nums;

    Equation(unsigned long target, vector<unsigned long>& nums) {
        this->target = target;
        this->nums = move(nums);
    }
};

class Possibility {
protected:
    unsigned long result;
public:
    Possibility(unsigned long result) {
        this->result = result;
    }

    unsigned long get_result() {
        return this->result;
    }

    void add(unsigned long num) {
        this->result += num;
    }

    void multiply(unsigned long num) {
        this->result *= num;
    }

    void concat(unsigned long num) {
        int ten_powered = 1;
        while (ten_powered <= num) {
            ten_powered *= 10;
        }

        this->result = (this->result * ten_powered) + num;
    }
};


vector<unsigned long> readNums(string line) {
    vector<unsigned long> nums{};
    string curr = "";
    for (unsigned long i = 0; i < line.size(); i++) {
        if (isdigit(line[i])) {
            curr += line[i];
        } else {
            nums.push_back(stoul(curr));
            curr = "";
        }
    }

    if (curr != "") {
        nums.push_back(stoul(curr));
    }

    return nums;
}

vector<Equation> loadEquations(fstream& fh) {
    vector<Equation> equations{};
    string line;
    while (getline(fh, line))
    {
        int target_end = line.find(':');
        string target = line.substr(0, target_end);
        auto nums = readNums(line.substr(target_end + 2));
        equations.emplace_back(stoul(target), nums);
    }

    return equations;
}

void checkCombinations(int index, vector<Possibility>& possibilities, Equation& equation, bool withConcat = false) {
    int n = possibilities.size();
    for (int i = 0; i < n; i++) {
        // check multiplication
        auto temp = possibilities[i].get_result();
        possibilities.emplace_back(temp);
        possibilities.back().multiply(equation.nums[index]);
        // won't ever be a target
        if (possibilities.back().get_result() > equation.target) {
            possibilities.pop_back();
        }

        if (withConcat) {
            possibilities.emplace_back(temp);
            possibilities.back().concat(equation.nums[index]);
            // won't ever be a target
            if (possibilities.back().get_result() > equation.target) {
                possibilities.pop_back();
            }        
        }
        // check addition
        possibilities[i].add(equation.nums[index]);
        // won't ever be a target
        if (possibilities[i].get_result() > equation.target) {
            possibilities.erase(possibilities.begin() + i);
            n -= 1;
            i -= 1;
        }
    }
}

// the same as partOne but withConcat is true
unsigned long partTwo(vector<Equation>& equations) {
    unsigned long long num_sum = 0;
    vector<Possibility> possibilities{};
    possibilities.reserve(2048);

    for (auto& equation : equations) {
        possibilities.emplace_back(*(equation.nums.begin()));

        for (int i = 1; i < equation.nums.size(); i++) {
            checkCombinations(i, possibilities, equation, true);
        }

        for (auto& possibility : possibilities) {
            if (possibility.get_result() == equation.target) {
                num_sum += equation.target;
                break;
            }
        }
        possibilities.clear();
    }


    return num_sum;
}

unsigned long partOne(vector<Equation>& equations) {
    unsigned long long num_sum = 0;
    vector<Possibility> possibilities{};
    possibilities.reserve(2048);

    for (auto& equation : equations) {
        possibilities.emplace_back(*(equation.nums.begin()));

        for (int i = 1; i < equation.nums.size(); i++) {
            checkCombinations(i, possibilities, equation);
        }

        for (auto& possibility : possibilities) {
            if (possibility.get_result() == equation.target) {
                num_sum += equation.target;
                break;
            }
        }
        possibilities.clear();
    }


    return num_sum;
}

int main() {
    fstream fh;
    fh.open("input.txt");
    auto equations = loadEquations(fh);
    cout << "Part 1: " << partOne(equations) << endl;
    cout << "Part 2: " << partTwo(equations) << endl;
    fh.close();


    return 0;
}