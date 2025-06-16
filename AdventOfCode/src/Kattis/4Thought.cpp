#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>
#include <stack>
#include <format>

constexpr int Eval(char op, int a, int b) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
	default: throw std::invalid_argument("Invalid operator");
    }
}
constexpr int Eval(const std::string& ops) {
    std::vector<int> nums = { 4, 4, 4, 4 };
    std::vector<char> op_stack = { ops[0], ops[1], ops[2] };
    for(size_t i = 0; i < op_stack.size();) {
        auto op = op_stack[i];
        if(op == '*' || op == '/') {
            nums[i] = Eval(op, nums[i], nums[i + 1]);
            nums.erase(nums.begin() + i + 1);
            op_stack.erase(op_stack.begin() + i);
        } else {
            i++;
        }
    }
    int result = nums[0];
    for(size_t i = 0; i < op_stack.size(); i++) {
        result = Eval(op_stack[i], result, nums[i + 1]);
    }
    return result;
}

std::string ToInfix(std::string ops, int ans) {
	return std::format("4 {} 4 {} 4 {} 4 = {}", ops[0], ops[1], ops[2], ans);
}

std::unordered_map<int, std::string> AllAnswers = []() {
    std::unordered_map<int, std::string> result{};
    // 4/4 + 4 + 4 == 4+4 + 4/4
    std::vector<std::string> current{ "+", "-", "/", "*" };
    auto next = std::vector<std::string>{};
    current.reserve(64);
    for (size_t round = 0; round < 2; round++) {
        for (const auto& exp : current) {
            next.emplace_back(exp + "+");
            next.emplace_back(exp + "-");
            next.emplace_back(exp + "*");
            next.emplace_back(exp + "/");
        }
        current = next;
        next.clear();
    }

    for (const auto& ops : current) {
        auto val = Eval(ops);
        result.emplace(val, ToInfix(ops, val));
    }
    return result;
    }();

void Run4Thought() {
    std::vector<int> vals{ 9, 0, 7, 11, 24 };
    for(auto val : vals) {
        if(AllAnswers.contains(val) ) {
            std::cout << AllAnswers.at(val) << "\n";
        } else {
            std::cout << "no solution\n";
		}
    }
}

int Run4ThoughtMain() {
    int count, val;
    std::cin >> count;
    for (auto i = 0; i < count; i++) {
        std::cin >> val;
        if (AllAnswers.contains(val)) {
            std::cout << AllAnswers.at(val) << "\n";
        }
        else {
            std::cout << "no solution\n";
        }
    }
    return 0;
}