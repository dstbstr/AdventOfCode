#include "Common.h"

SOLUTION(2023, 9) {

    constexpr std::vector<s64> Next(const std::vector<s64>&current) {
        std::vector<s64> result;
        for (size_t i = 1u; i < current.size(); i++) {
            result.push_back(current[i] - current[i - 1]);
        }
        return result;
    }

    constexpr Constexpr::Stack<std::vector<s64>> GetHistory(const std::string& line) {
        Constexpr::Stack<std::vector<s64>> stack;
        stack.push(ParseLineAsNumbers<s64>(line, " "));
        while (true) {
            auto top = stack.top();
            if (std::all_of(top.begin(), top.end(), [](s64 val) { return val == 0; })) break;
            stack.push(Next(top));
        }

        return stack;
    }

    constexpr s64 ExtractForward(Constexpr::Stack<std::vector<s64>>& stack) {
        while (stack.size() > 1ull) {
            auto next = stack.pop();
            stack.top().push_back(stack.top().back() + next.back());
        }

        return stack.top().back();
    }

    constexpr s64 ExtractBackward(Constexpr::Stack<std::vector<s64>>& stack) {
        while (stack.size() > 1ull) {
            auto next = stack.pop();
            stack.top().insert(stack.top().begin(), stack.top()[0] - next[0]);
        }

        return stack.top()[0];
    }

    constexpr s64 Solve(const std::vector<std::string>& lines, auto Extract) {
        auto result = std::accumulate(lines.begin(), lines.end(), 0ll, [&](s64 running, const std::string& line) {
            auto stack = GetHistory(line);
            return running + Extract(stack);
            });
        return result;
    }

    PART(1) {
        return Solve(lines, ExtractForward);
    }

    PART(2) {
        return Solve(lines, ExtractBackward);
    }

    TEST(1) {
        std::vector<std::string> example = {
            "0 3 6 9 12 15",
            "1 3 6 10 15 21",
            "10 13 16 21 30 45"
        };

        if (Solve(example, ExtractForward) != 114) return false;
		if (Solve(example, ExtractBackward) != 2) return false;
        return true;
    }
}