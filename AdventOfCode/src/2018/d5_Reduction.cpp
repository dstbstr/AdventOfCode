#include "Common.h"

SOLUTION(2018, 5) {
    constexpr auto pairs = { "aA", "bB", "cC", "dD", "eE", "fF", "gG", "hH", "iI", "jJ", "kK", "lL", "mM", "nN", "oO", "pP", "qQ", "rR",
    "sS", "tT", "uU", "vV", "wW", "xX", "yY", "zZ" };

    constexpr std::string Reduce(std::string_view input) {
        Constexpr::Stack<char> stack;
        for (auto c : input) {
            if (stack.is_empty() || (stack.top() ^ c) != 32) stack.push(c);
            else stack.pop();
        }

        std::string result;
        while (!stack.is_empty()) {
            result.push_back(stack.top());
            stack.pop();
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    PART(1) {
        return Reduce(lines[0]).size();
    }

    PART(2) {
        size_t best = 9999;
        
        for (const auto& pair : pairs) {
            auto test = Constexpr::RemoveAllOfCopy(lines[0], pair);
            best = std::min(best, Reduce(test).size());
        }
                
        return best;
    }

    static_assert(Reduce("aA") == "");
    static_assert(Reduce("aa") == "aa");
    static_assert(Reduce("dabAcCaCBAcCcaDA") == "dabCBAcaDA");
}