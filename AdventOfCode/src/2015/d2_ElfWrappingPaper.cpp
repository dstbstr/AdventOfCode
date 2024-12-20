#include "Common.h"

SOLUTION(2015, 2) {
    constexpr u64 CalculateWrappingPaper(u32 length, u32 width, u32 height)
    {
        auto d1 = height * width;
        auto d2 = height * length;
        auto d3 = width * length;

        if (d1 <= d2 && d1 <= d3) {
            return (d1 * 3) + (d2 * 2) + (d3 * 2);
        }
        else if (d2 <= d1 && d2 <= d3) {
            return (d1 * 2) + (d2 * 3) + (d3 * 2);
        }
        else {
            return (d1 * 2) + (d2 * 2) + (d3 * 3);
        }
    }

    constexpr u64 CalculateRibbon(u32 length, u32 width, u32 height)
    {
        u64 area = height * width * length;
        if (height >= width && height >= length) {
            return area + width + width + length + length;
        }
        else if (width >= height && width >= length) {
            return area + height + height + length + length;
        }
        else if (length >= width && length >= height) {
            return area + width + width + height + height;
        }
        return 0;
    }

    constexpr void ParseLine(std::string_view line, u32& outLength, u32& outWidth, u32& outHeight) {
        auto s = Constexpr::Split(line, "x");
        Constexpr::ParseNumber(s[0], outLength);
        Constexpr::ParseNumber(s[1], outWidth);
        Constexpr::ParseNumber(s[2], outHeight);
    }

    constexpr u64 Solve(const auto& lines, auto func) {
        u64 result = 0;
        for (const auto& line : lines) {
            u32 length, width, height;
            ParseLine(line, length, width, height);
            result += func(length, width, height);
        }

        return result;
    }
    PART(1) {
        return Solve(lines, CalculateWrappingPaper);
    }

    PART(2) {
        return Solve(lines, CalculateRibbon);
    }

    static_assert(CalculateWrappingPaper(2, 3, 4) == 58);
    static_assert(CalculateWrappingPaper(1, 1, 10) == 43);

    static_assert(CalculateRibbon(2, 3, 4) == 34);
    static_assert(CalculateRibbon(1, 1, 10) == 14);
    static_assert(CalculateRibbon(2, 2, 2) == 16);
}