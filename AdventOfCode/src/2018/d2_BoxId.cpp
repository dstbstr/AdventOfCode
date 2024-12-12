#include "Common.h"

SOLUTION(2018, 2) {
    PART(1) {
        u32 twos = 0;
        u32 threes = 0;
        std::array<u8, 26> counts{};
        for (const auto& line : lines) {
            std::fill(counts.begin(), counts.end(), u8(0));
            for (const auto& c : line) {
                counts[c - 'a']++;
            }
            for (auto count : counts) {
                twos += count == 2;
                threes += count == 3;
            }
        }
        return twos * threes;
    }

    constexpr bool OffByOne(std::string_view lhs, std::string_view rhs) {       
        u32 deltas = 0;
        for (size_t i = 0; i < lhs.size(); i++) {
            deltas += lhs[i] != rhs[i];
        }
        return deltas == 1;
    }

    PART(2) {
        auto copy = lines;
        std::sort(copy.begin(), copy.end());
        
        for (size_t i = 0u; i < copy.size() - 1; i++) {
            if (OffByOne(copy[i], copy[i + 1])) {
                return "\n" + std::string(copy[i]) + '\n' + std::string(copy[i + 1]);
            }
        }

        return ""s;
    }

    static_assert(OffByOne("abc", "adc"));
    static_assert(!OffByOne("abcd", "efgh"));
}