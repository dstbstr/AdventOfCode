#include "Common.h"

SOLUTION(2024, 19) {
	constexpr bool IsPossible(std::string_view target, const std::vector<std::string_view>& available) {
		if (target.empty()) return true;
		for(auto sv : available) {
			if(target.starts_with(sv)) {
				if (IsPossible(target.substr(sv.size()), available)) {
					return true;
				}
			}
		}
		return false;
	}

	PART(1) {
		auto groups = SplitInputIntoGroups(lines);
		auto available = Constexpr::Split(groups[0][0], ", ");
		auto targets = groups[1];

		auto possible = std::count_if(targets.begin(), targets.end(), [&](const auto& target) { return IsPossible(target, available); });
		return static_cast<size_t>(possible);
	}

	using Cache = Constexpr::BigMap<std::string_view, u64, 100'000>;
	constexpr u64 CountPossible(std::string_view target, const std::vector<std::string_view>& available, Cache& cache) {
		if (cache.contains(target)) return cache.at(target);
		if (target.empty()) return 1;

		size_t result = 0;
		for (auto sv : available) {
			if (target.starts_with(sv)) {
				result += CountPossible(target.substr(sv.size()), available, cache);
			}
		}
		cache[target] = result;
		return result;
	}

	PART(2) {
		auto groups = SplitInputIntoGroups(lines);
		auto available = Constexpr::Split(groups[0][0], ", ");
		auto targets = groups[1];

		Cache cache{};
		return std::accumulate(targets.begin(), targets.end(), 0ull, [&](u64 acc, const auto& target) { return acc + CountPossible(target, available, cache); });
	}
	TEST(1) {
		std::vector<std::string> lines = {
			"r, wr, b, g, bwu, rb, gb, br",
			"",
			"brwrr",
			"bggr",
			"gbbr",
			"rrbgbr",
			"ubwu",
			"bwurrg",
			"brgr",
			"bbrgwb",
		};

		return std::get<size_t>(Part1(lines)) == 6 && std::get<size_t>(Part2(lines)) == 16;
	}
}