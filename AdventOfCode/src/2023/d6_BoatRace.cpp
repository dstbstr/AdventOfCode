#include "Common.h"

SOLUTION(2023, 6) {
    constexpr std::vector<std::pair<u64, u64>> ParseInput(const std::vector<std::string>&lines) {
        auto timeTrimmed = lines[0].substr(6);
        auto distTrimmed = lines[1].substr(10);
        auto timeStrs = Constexpr::Split(timeTrimmed, " ");
        auto distStrs = Constexpr::Split(distTrimmed, " ");

        std::vector<std::pair<u64, u64>> result;
        for (size_t i = 0u; i < timeStrs.size(); i++) {
            auto p = std::make_pair(0u, 0u);
            Constexpr::ParseNumber(timeStrs[i], p.first);
            Constexpr::ParseNumber(distStrs[i], p.second); 
            result.push_back(p);
        }

        return result;
    }

    constexpr std::pair<u64, u64> ParseAsSingle(const std::vector<std::string>& lines) {
        auto timeTrimmed = lines[0].substr(6);
        auto distTrimmed = lines[1].substr(10);
        Constexpr::ReplaceAll(timeTrimmed, " ", "");
        Constexpr::ReplaceAll(distTrimmed, " ", "");

        std::pair<u64, u64> result{};
        Constexpr::ParseNumber(timeTrimmed, result.first);
        Constexpr::ParseNumber(distTrimmed, result.second);

        return result;
    }

    constexpr u64 CountWins(u64 time, u64 distance) {
        auto t = 1;
        while (t * (time - t) <= distance) t++;
        return (time - t) - t + 1;
    }

    PART(1) {
        auto pairs = ParseInput(lines);
		return std::accumulate(pairs.begin(), pairs.end(), 1ull, [](u64 running, const auto& p) {
			return running * CountWins(p.first, p.second);
		});
    }

    PART(2) {
        auto [time, dist] = ParseAsSingle(lines);

        return CountWins(time, dist);
    }

    TEST(1) {
        std::vector < std::string> lines = {
            "Time:      7  15   30",
            "Distance:  9  40  200"
        };

        return std::get<u64>(Part1(lines)) == 288;
    }
}