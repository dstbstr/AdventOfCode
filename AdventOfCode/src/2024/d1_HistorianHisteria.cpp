#include "Common.h"

SOLUTION(2024, 1) {
	constexpr std::pair<s32, s32> ParseLine(const std::string & line) {
		s32 a, b;
		auto parts = Constexpr::Split(line, "   ");
		Constexpr::ParseNumber(parts[0], a);
		Constexpr::ParseNumber(parts[1], b);
		return { a, b };
	}

	constexpr s32 SolvePart1(const std::vector<std::string>&lines) {
        std::vector<s32> list1;
		std::vector<s32> list2;
		list1.reserve(lines.size());
		list2.reserve(lines.size());

		for (const auto& line : lines) {
            auto [lhs, rhs] = ParseLine(line);
            list1.emplace_back(lhs);
			list2.emplace_back(rhs);
		}
		std::sort(list1.begin(), list1.end());
		std::sort(list2.begin(), list2.end());

        s32 result = 0;
        for(size_t i = 0; i < list1.size(); i++) {
			result += Constexpr::AbsDistance(list1[i], list2[i]);
        }
		return result;
	}

    PART(1) {
        return SolvePart1(lines);
    }

    constexpr s32 SolvePart2(const std::vector<std::string>& lines) {
        std::vector<s32> list1;
        std::vector<s32> list2;
        list1.reserve(lines.size());
        list2.reserve(lines.size());

        for (const auto& line : lines) {
            auto [lhs, rhs] = ParseLine(line);
            list1.emplace_back(lhs);
            list2.emplace_back(rhs);
        }

        Constexpr::BigMap<s32, s32, 10'000> counts;
        for(auto val : list2) {
			counts[val]++;
        }
		return std::accumulate(list1.begin(), list1.end(), 0, [&counts](s32 acc, s32 val) { return acc + (val * counts[val]); });
    }

    PART(2) {
        return SolvePart2(lines);
    }

	constexpr std::vector<std::string> GetTestInput() {
		return {
			"3   4",
			"4   3",
			"2   5",
			"1   3",
			"3   9",
			"3   3"
		};
	}

	static_assert(SolvePart1(GetTestInput()) == 11);
	static_assert(SolvePart2(GetTestInput()) == 31);
}