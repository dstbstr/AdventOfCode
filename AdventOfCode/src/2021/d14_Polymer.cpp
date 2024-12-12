#include "Common.h"

SOLUTION(2021, 14) {
    using Map = std::array<char, Constexpr::FromBase26("ZZ")>;
    using ProduceMap = std::array<std::pair<std::string, std::string>, Constexpr::FromBase26("ZZ")>;
    using Counts = std::array<size_t, Constexpr::FromBase26("ZZ")>;

    constexpr void BuildMaps(const auto& lines, std::vector<std::string>& outKnownKeys, ProduceMap& outProduceMap) {
        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, " -> ");
            outKnownKeys.push_back(std::string(split[0]));
            //CH -> B
            //CB, BH
            std::string lhs;
            lhs.push_back(split[0][0]);
            lhs.push_back(split[1][0]);

            std::string rhs;
            rhs.push_back(split[1][0]);
            rhs.push_back(split[0][1]);
            outProduceMap[Constexpr::FromBase26(split[0])] = std::make_pair(lhs, rhs);
        }
    }

    constexpr Counts GetInitialCounts(const std::string & polymer) {
        Counts result{};
        for (size_t i = 0u; i < polymer.size() - 1; i++) {
            result[Constexpr::FromBase26(polymer.substr(i, 2))]++;
        }
        return result;
    }

    constexpr Counts Next(const Counts& counts, const ProduceMap& produceMap, const std::vector<std::string>& knownKeys) {
        Counts result{};
        for (const auto& key : knownKeys) {
            auto index = Constexpr::FromBase26(key);
            auto [lhs, rhs] = produceMap[index];
            auto lhsIndex = Constexpr::FromBase26(lhs);
            auto rhsIndex = Constexpr::FromBase26(rhs);
            result[lhsIndex] += counts[index];
            result[rhsIndex] += counts[index];
        }

        return result;
    }

    constexpr std::array<u64, 26> GetLetterCounts(const Counts & counts, const std::vector<std::string>& knownKeys) {
        std::array<u64, 26> result{};
        for (const auto& key : knownKeys) {
            auto count = counts[Constexpr::FromBase26(key)];
            result[key[0] - 'A'] += count;
        }

        return result;
    }

    constexpr auto Solve(const auto& lines, size_t iterations) {
        auto groups = SplitInputIntoGroups(lines);
        auto polymer = groups[0][0];
        std::vector<std::string> knownKeys;
        ProduceMap produceMap{};
        BuildMaps(groups[1], knownKeys, produceMap);

        auto counts = GetInitialCounts(polymer);
        for (size_t i = 0u; i < iterations; i++) {
            counts = Next(counts, produceMap, knownKeys);
        }

        auto letterCounts = GetLetterCounts(counts, knownKeys);
        letterCounts[polymer.back() - 'A']++; //need to add in one instance of the last letter
        auto min = std::numeric_limits<u64>::max();
        for (auto count : letterCounts) {
            if (count > 0) {
                min = std::min(min, count);
            }
        }
        return Constexpr::FindMax(letterCounts) - min;
    }

    PART(1) {
        return Solve(lines, 10);
    }

    PART(2) {
        return Solve(lines, 40);
    }

    constexpr bool TestSolve() {
        std::vector<std::string> lines = {
            "NNCB",
            "",
            "CH -> B",
            "HH -> N",
            "CB -> H",
            "NH -> C",
            "HB -> C",
            "HC -> B",
            "HN -> C",
            "NN -> C",
            "BH -> H",
            "NC -> B",
            "NB -> B",
            "BN -> B",
            "BB -> N",
            "BC -> B",
            "CC -> N",
            "CN -> C"
        };
        if (Solve(lines, 10) != 1588ull) return false;
        return Solve(lines, 40) == 2188189693529ull;
    }

    static_assert(GetInitialCounts("NNCB")[Constexpr::FromBase26("NN")] == 1);
    static_assert(GetInitialCounts("NNCB")[Constexpr::FromBase26("NC")] == 1);
    static_assert(GetInitialCounts("NNCB")[Constexpr::FromBase26("CB")] == 1);
    //static_assert(TestSolve());

    TEST(1) {
        return TestSolve();
    }
}