#include "Common.h"
#include "Core/Algorithms/FloydCycle.h"

SOLUTION(2017, 6) {
    constexpr std::vector<u32> Next(std::vector<u32> in) {
        auto max = std::max_element(in.begin(), in.end());
        for (u32 iters{ std::exchange(*max, 0) }; iters--; ++(*max)) {
            if (++max == in.end()) {
                max = in.begin();
            }
        }
        return in;
    }

    PART(1) {
        auto banks = ParseLineAsNumbers<u32>(lines[0], "\t");
        u32 cycleStart, cycleLength;
        FloydCycle::FindCycle(banks, Next, cycleLength, cycleStart);
        return cycleLength + cycleStart;
    }

    PART(2) {
        auto banks = ParseLineAsNumbers<u32>(lines[0], "\t");
        u32 cycleStart, cycleLength;
        FloydCycle::FindCycle(banks, Next, cycleLength, cycleStart);
        return cycleLength;
    }

    //static_assert(PartOne({ "0\t2\t7\t0" }) == "5");
    //static_assert(PartTwo({ "0\t2\t7\t0" }) == "4");
}