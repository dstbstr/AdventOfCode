#include "Common.h"

SOLUTION(2023, 1) {

    constexpr u32 FindFirstNumber(std::string_view sv) {
        return sv[sv.find_first_of("0123456789")] - '0';
    }

    constexpr u32 FindLastNumber(std::string_view sv) {
        return sv[sv.find_last_of("0123456789")] - '0';
    }

    constexpr u32 SumDigits(const std::vector < std::string> lines) {
        u32 result = 0;
        for (auto line : lines) {
            auto val = FindFirstNumber(line) * 10;
            val += FindLastNumber(line);
            result += val;
        }
        return result;
    }

    PART(1) {
         return SumDigits(lines);
    }

    constexpr std::vector<std::string> ReplaceWords(const auto & lines) {
        std::vector<std::string> transformed;
        std::transform(lines.begin(), lines.end(), std::back_inserter(transformed), [](std::string_view sv) {
            std::string running(sv);
            Constexpr::ReplaceAll(running, "zero", "0");
            Constexpr::ReplaceAll(running, "0ne", "01");
            Constexpr::ReplaceAll(running, "one", "1");
            Constexpr::ReplaceAll(running, "zer1", "01");

            Constexpr::ReplaceAll(running, "two", "2");
            Constexpr::ReplaceAll(running, "tw1", "21");
            
            Constexpr::ReplaceAll(running, "three", "3");
            
            Constexpr::ReplaceAll(running, "four", "4");
            
            Constexpr::ReplaceAll(running, "five", "5");
            
            Constexpr::ReplaceAll(running, "six", "6");
            
            Constexpr::ReplaceAll(running, "seven", "7");
            
            Constexpr::ReplaceAll(running, "eight", "8");
            Constexpr::ReplaceAll(running, "1ight", "18");
            Constexpr::ReplaceAll(running, "3ight", "38");
            Constexpr::ReplaceAll(running, "5ight", "58");
            Constexpr::ReplaceAll(running, "eigh2", "82");
            Constexpr::ReplaceAll(running, "eigh3", "83");
            
            Constexpr::ReplaceAll(running, "nine", "9");
            Constexpr::ReplaceAll(running, "7ine", "79");
            Constexpr::ReplaceAll(running, "nin8", "98");
            return running;
            });

        return transformed;
    }
    PART(2) {
        auto transformed = ReplaceWords(lines);
        return SumDigits(transformed);
    }

    static_assert(FindFirstNumber("1abc2") == 1);
    static_assert(FindLastNumber("1abc2") == 2);
    static_assert(ReplaceWords(std::vector<std::string>{ "eightwo" })[0] == "82");

    TEST(1) {
        std::vector<std::string> example = {
            "two1nine", //29
            "eighttwothree", //83
            "abcone2threexyz", //13
            "xtwone3four", //24
            "4nineeightseven2", //42
            "zoneight234" //14
        };

        return SumDigits(ReplaceWords(example)) == 205;
    }
}