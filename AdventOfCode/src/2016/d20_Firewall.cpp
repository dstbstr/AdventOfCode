#include "Common.h"

SOLUTION(2016, 20) {
    constexpr u32 MaxIp = 4294967295;

    constexpr std::vector<UCoord> GetRanges(const auto& lines) {
        std::vector<UCoord> ranges;
        ranges.reserve(lines.size());
        for (const auto& line : lines) {
            ranges.emplace_back(UCoord(line, "-"));
        }

        return ranges;
    }

    constexpr u64 FindLowest(const auto& lines) {
        auto ranges = GetRanges(lines);
        std::ranges::sort(ranges, {}, &UCoord::X);

        auto bottomRange = ranges[0];
        for (size_t i = 1; i < ranges.size(); i++) {
            auto next = ranges[i];
            if (bottomRange.Y >= next.X - 1) {
                bottomRange.Y = std::max(bottomRange.Y, next.Y);
            }
            else {
                return bottomRange.Y + 1;
            }
        }

        return 0;
    }

    constexpr u64 CountAvailable(const auto& lines) {
        auto ranges = GetRanges(lines);
        std::ranges::sort(ranges, {}, &UCoord::X);

        u64 result = 0;
        auto current = ranges[0];
        for (size_t i = 1; i < ranges.size(); i++) {
            auto next = ranges[i];
            if (current.Y >= next.X - 1) {
                current.Y = std::max(current.Y, next.Y);
            }
            else {
                result += next.X - current.Y - 1;
                current.Y = next.Y;
            }
        }

        if (current.Y < MaxIp) {
            result += MaxIp - current.Y;
        }

        return result;
    }

    PART(1) {
        return FindLowest(lines);
    }
    PART(2) {
        return CountAvailable(lines);
    }

    TEST(1) {
        std::vector<std::string> lines = {
            "5-8",
            "0-2",
            "4-7"
        };

        return FindLowest(lines) == 3;
    }
}

