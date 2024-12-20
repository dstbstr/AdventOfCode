#include "Common.h"

SOLUTION(2018, 1) {

    PART(1) {
        auto deltas = ParseLinesAsNumbers<s32>(lines);
        return std::accumulate(deltas.begin(), deltas.end(), 0);
    }

    PART(2) {
        auto deltas = ParseLinesAsNumbers<s32>(lines);
        std::vector<s32> frequencies {0};
        for (size_t i = 0u; i < deltas.size(); i++) {
            frequencies.push_back(frequencies.back() + deltas[i]);
        }

        auto shift = frequencies.back();
        Constexpr::SmallMap<s32, std::vector<s32>> mods;
        //the last value in frequencies is the first value in the second row
        for (size_t i = 0; i < frequencies.size() - 1; i++) {
            auto v = frequencies[i];
            mods[v % shift].push_back(v);
        }

        s32 bestDelta = std::numeric_limits<s32>::max();
        s64 bestKey = 0;

        for (auto& [key, values] : mods) {
            std::sort(values.begin(), values.end());
            for (size_t i = 0; i < values.size() - 1; i++) {
                auto delta = values[i + 1] - values[i];
                if (delta < bestDelta) {
                    bestDelta = delta;
                    bestKey = values[i + 1];
                }
            }
        }

        return bestKey;
    }

    //static_assert(PartTwo({ "+10000000", "-9999999" }) == "10000000");
}