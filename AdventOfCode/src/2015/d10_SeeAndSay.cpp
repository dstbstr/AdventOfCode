#include "Common.h"

SOLUTION(2015, 10) {
    constexpr void AppendNext(char lastCharacter, int count, std::string & result) {
        switch (count) {
        case 1: result += "1"; break;
        case 2: result += "2"; break;
        case 3: result += "3"; break;
        }
        result += lastCharacter;
    }

    constexpr std::string Next(const std::string & input) {
        std::string result;
        char lastCharacter = input[0];
        int count = 0;
        for (const auto& c : input) {
            if (c == lastCharacter) {
                count++;
            }
            else {
                AppendNext(lastCharacter, count, result);
                lastCharacter = c;
                count = 1;
            }
        }
        AppendNext(lastCharacter, count, result);

        return result;
    }

    constexpr std::string ApplyTimes(std::string_view input, size_t times) {
        std::string running = std::string(input);

        for (size_t i = 0u; i < times; i++) {
            running = Next(running);
        }

        return running;
    }


    PART(1) {
        return ApplyTimes(lines[0], 40).size();
    }
    PART(2) {
        return ApplyTimes(lines[0], 50).size();
    }

    static_assert(Next("1") == "11");
    static_assert(Next("11") == "21");
    static_assert(Next("21") == "1211");
    static_assert(Next("1211") == "111221");
    static_assert(Next("111221") == "312211");
    //static_assert(ApplyTimes("1113222113", 40).size() == 252594);
}