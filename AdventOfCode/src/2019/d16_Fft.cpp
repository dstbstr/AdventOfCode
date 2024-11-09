#include "Common.h"

SOLUTION(2019, 16) {
    constexpr std::vector<s32> GetMultipliers(size_t inputSize, size_t row) {
        //0 1 0 -1
        std::vector<s32> multipliers = { 0, 1, 0, -1 };
        std::vector<s32> result;
        result.reserve(inputSize);
        while (result.size() < inputSize) {
            for (auto m : multipliers) {
                for (size_t i = 0u; i < row; i++) {
                    result.push_back(m);
                }
            }
        }

        std::rotate(result.begin(), result.begin() + 1, result.end());
        result.erase(result.begin() + inputSize, result.end());
        return result;
    }

    constexpr std::string RunPhase(const std::string & str) {
        auto inputSize = str.size();
        std::string result;
        result.reserve(inputSize);
        std::vector<u32> numbers;
        numbers.reserve(str.size());
        for (auto c : str) {
            numbers.push_back(static_cast<u32>(c - '0'));
        }

        for (size_t row = 0; row < str.size(); row++) {
            s32 val = 0;
            size_t col = row;
            size_t groupSize = row + 1;
            while (col < inputSize) {
                auto limit = std::min(groupSize, inputSize - col);
                for (size_t i = 0u; i < limit; i++) {
                    val += numbers[col++];
                }
                col += groupSize; //skip zeros
                if (col >= inputSize) break;
                limit = std::min(groupSize, inputSize - col);
                for (size_t i = 0u; i < limit; i++) {
                    val -= numbers[col++];
                }
                col += groupSize; //skip zeros
            }
            if (val < 0) val = -val;
            result.push_back((val % 10) + '0');
        }

        return result;
    }

    PART(1) {
        std::string running = std::string(lines[0]);
        for (auto i = 0; i < 100; i++) {
            running = RunPhase(running);
        }

        return running.substr(0, 8);
    }

    PART(2) {
        auto line = std::string(lines[0]);
        std::string running = "";
        running.reserve(line.size() * 10'000);
        size_t offset;
        Constexpr::ParseNumber(line.substr(0, 7), offset);

        for (auto i = 0; i < 10'000; i++) {
            running += line;
        }

        running = running.substr(offset);
        std::reverse(running.begin(), running.end());
        for (auto i = 0; i < 100; i++) {
            for (auto c = 1ull; c < running.size(); c++) {
                char val = (running[c - 1] - '0') + (running[c] - '0');
                val %= 10;
                running[c] = val + '0';
            }
        }

        std::reverse(running.begin(), running.end());
        return running.substr(0, 8);
    }

    static_assert(GetMultipliers(8, 1) == std::vector<s32>{1, 0, -1, 0, 1, 0, -1, 0});
    static_assert(GetMultipliers(8, 2) == std::vector<s32>{0, 1, 1, 0, 0, -1, -1, 0});
    static_assert(GetMultipliers(8, 3) == std::vector<s32>{0, 0, 1, 1, 1, 0, 0, 0});
    static_assert(GetMultipliers(8, 4) == std::vector<s32>{0, 0, 0, 1, 1, 1, 1, 0});
    static_assert(GetMultipliers(8, 5) == std::vector<s32>{0, 0, 0, 0, 1, 1, 1, 1});

    static_assert(RunPhase("12345678") == "48226158");

    TEST(1) {
        return RunPhase("12345678") == "48226158";
    }
    /*
    TEST(2) {
        return Part1({"80871224585914546619083218645595"}) == "24176176";
    }
    TEST(3) {
        return Part2({ "03036732577212944063491565474664" }) == "84462026";
    }
    */
}