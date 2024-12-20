#include "Common.h"

SOLUTION(2019, 1) {
    constexpr u64 Calculate(u64 mass) {
        auto quotient = mass / 3;
        if (quotient < 2) return 0;
        return quotient - 2;
    }

    constexpr u64 CalculateWithFuel(u64 mass) {
        u64 result = 0;
        auto delta = Calculate(mass);
        while (delta > 0) {
            result += delta;
            delta = Calculate(delta);
        }

        return result;
    }

    PART(1) {
        auto masses = ParseLinesAsNumbers<size_t>(lines);
        return std::accumulate(masses.begin(), masses.end(), 0ull, [](size_t previous, size_t mass) { 
            return previous + Calculate(mass); 
        });
    }

    PART(2) {
        auto masses = ParseLinesAsNumbers<size_t>(lines);
        return std::accumulate(masses.begin(), masses.end(), 0ull, [](size_t previous, size_t mass) { 
            return previous + CalculateWithFuel(mass); 
        });
    }

    static_assert(Calculate(12) == 2);
    static_assert(Calculate(14) == 2);
    static_assert(Calculate(1969) == 654);
    static_assert(Calculate(100756) == 33583);

    static_assert(CalculateWithFuel(1969) == 966);
    static_assert(CalculateWithFuel(100756) == 50346);
}