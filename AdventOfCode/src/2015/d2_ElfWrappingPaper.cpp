#include "Common.h"

SOLUTION(2015, 2) {
    constexpr u64 CalculateWrappingPaper(const Vec3<u32>& dims) {
        auto d1 = dims.X * dims.Y;
        auto d2 = dims.X * dims.Z;
        auto d3 = dims.Y * dims.Z;
        auto min = Constexpr::Min(d1, d2, d3);
        return 2ull * (d1 + d2 + d3) + min;
    }

    constexpr u64 CalculateRibbon(const Vec3<u32>& dims) {
        u64 area = dims.X * dims.Y * dims.Z;
		auto max = Constexpr::Max(dims.X, dims.Y, dims.Z);
        return area + 2ull * (dims.X + dims.Y + dims.Z - max);
    }

    constexpr u64 Solve(const auto& lines, auto func) {
        auto packages = ParseLines(lines, [](std::string_view line) {
            return Vec3<u32>(line, "x");
		});
        return std::accumulate(packages.begin(), packages.end(), 0ull, [&](u64 result, const Vec3<u32>& dims) {
            return result + func(dims);
        });
    }
    PART(1) {
        return Solve(lines, CalculateWrappingPaper);
    }

    PART(2) {
        return Solve(lines, CalculateRibbon);
    }

    static_assert(CalculateWrappingPaper({2, 3, 4}) == 58);
    static_assert(CalculateWrappingPaper({1, 1, 10}) == 43);

    static_assert(CalculateRibbon({2, 3, 4}) == 34);
    static_assert(CalculateRibbon({1, 1, 10}) == 14);
    static_assert(CalculateRibbon({2, 2, 2}) == 16);
}