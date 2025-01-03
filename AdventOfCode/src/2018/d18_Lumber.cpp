#include "Common.h"

SOLUTION(2018, 18) {

    /*
    ______
    _...._
    _...._
    _...._
    _...._
    ______
    
    _______....__....__....__...._______
    TL = C - 7
    T = C - 6
    TR = C - 5
    L = C - 1
    R = C + 1
    BL = C + 5
    B = C + 6
    BR = C + 7
    
    C = 7, TL = 0, T = 1, TR = 2 L = 6 R = 8 BL = 12 B = 13 BR = 14
    {{}, {}, {}, {}, {}, {}, {}, {0, 1, 2, 6, 8, 12, 13, 14}, {1,2,3,7,9,13,14,15}, {2,3,4,8,10,14,15,16}, {3,4,5,9,11,15,16,17}, {}, {}, 
    */

    template<size_t Rows, size_t Cols>
    constexpr std::array<std::array<size_t, 8>, Rows * Cols> BuildLookup() {
        std::array<std::array<size_t, 8>, Rows * Cols> result;
        for (size_t i = 0; i < Rows * Cols; i++) {
            result[i] = { i - Cols - 1, i - Cols, i - Cols + 1, i - 1, i + 1, i + Cols - 1, i + Cols, i + Cols + 1 };
        }
        return result;
    }

    constexpr auto LookupTableBig = BuildLookup<52, 52>();
    constexpr auto LookupTableSmall = BuildLookup<12, 12>();

    constexpr void Next(const std::string& src, std::string& dest, const std::array<size_t, 8>& indexes, size_t pos) {
        auto n = std::string({ src[indexes[0]], src[indexes[1]], src[indexes[2]], src[indexes[3]], src[indexes[4]], src[indexes[5]], src[indexes[6]], src[indexes[7]] });
        auto treeCount = std::count(n.begin(), n.end(), '|');
        auto yardCount = std::count(n.begin(), n.end(), '#');

        switch (src[pos]) {
        case '.': dest[pos] = treeCount > 2 ? '|' : '.'; break;
        case '|': dest[pos] = yardCount > 2 ? '#' : '|'; break;
        case '#': dest[pos] = (treeCount && yardCount) ? '#' : '.'; break;
        }
    }

    constexpr void NextBig(const std::string& src, std::string& dest) {
        for (size_t i = 1; i < 51; i++) {
            for (size_t j = 1; j < 51; j++) {
                auto pos = i * 52 + j;
                Next(src, dest, LookupTableBig[pos], pos);
            }
        }
    }

    constexpr void NextSmall(const std::string& src, std::string& dest) {
        for (size_t i = 1; i < 11; i++) {
            for (size_t j = 1; j < 11; j++) {
                auto pos = i * 12 + j;
                Next(src, dest, LookupTableSmall[pos], pos);
            }
        }
    }

    constexpr size_t GetScore(const std::string& src) {
        return std::count(src.begin(), src.end(), '|') * std::count(src.begin(), src.end(), '#');
    }

    constexpr std::string Flatten(const auto& lines) {
        std::string border(lines.size() + 2, '_');
        std::string result = border;
        result.reserve(52 * 52);
        for (const auto& line : lines) {
            result.push_back('_');
            result += line;
            result.push_back('_');
        }
        result += border;
        return result;
    }

    PART(1) {
        auto src = Flatten(lines);
        auto dest = src;
        for (auto i = 0; i < 10; i++) {
            NextBig(src, dest);
            std::swap(src, dest);
        }

        return GetScore(src);
    }

    PART(2) {
        auto src = Flatten(lines);
        auto dest = src;
        Constexpr::SmallSet<std::string> seen;
        size_t cycleStart = 0;
        while (true) {
            if (!seen.insert(src)) break;
            NextBig(src, dest);
            std::swap(src, dest);
            cycleStart++;
        }
        auto target = src;
        size_t cycleSize = 0;
        do {
            NextBig(src, dest);
            std::swap(src, dest);
            cycleSize++;
        } while (src != target);

        auto remaining = 1'000'000'000 - cycleStart;
        remaining %= cycleSize;
        for (auto i = 0u; i < remaining; i++) {
            NextBig(src, dest);
            std::swap(src, dest);
        }

        return GetScore(src);
    }

    TEST(1) {
        std::vector<std::string> lines = {
            ".#.#...|#.",
            ".....#|##|",
            ".|..|...#.",
            "..|#.....#",
            "#.#|||#|#|",
            "...#.||...",
            ".|....|...",
            "||...#|.#|",
            "|.||||..|.",
            "...#.|..|."
        };

        auto src = Flatten(lines);
        auto dest = src;
        for (auto i = 0; i < 10; i++) {
            NextSmall(src, dest);
            std::swap(src, dest);
        }
        
        if (GetScore(src) != 1147) return false;

        return true;
    }
}