#include "Common.h"

SOLUTION(2025, 7) {
	constexpr std::pair<size_t, size_t> Solve(std::vector<std::string> lines) {
		auto start = lines[0].find('S');
		std::vector<size_t> counts(lines[0].size(), 0);
		counts[start] = 1;
		size_t splits = 0;

		for(size_t row = 1; row < lines.size(); row++) {
			std::vector<size_t> newCounts(lines[row].size(), 0);
			for(size_t col = 0; col < lines[row].size(); col++) {
				auto c = lines[row][col];
				if(c == '.') {
					newCounts[col] += counts[col];
				} else {
					splits += counts[col] > 0;
					newCounts[col - 1] += counts[col];
					newCounts[col + 1] += counts[col];
				}
			}
			counts = newCounts;
		}

		auto paths = std::accumulate(counts.begin(), counts.end(), 0ull, std::plus<>());
		return { splits, paths };
	}

	PART(1) {
		return Solve(lines).first;
	}
	PART(2) {
		return Solve(lines).second;
	}

	static constexpr std::vector<std::string> sampleInput() {
		return {
			".......S.......",
			"...............",
			".......^.......",
			"...............",
			"......^.^......",
			"...............",
			".....^.^.^.....",
			"...............",
			"....^.^...^....",
			"...............",
			"...^.^...^.^...",
			"...............",
			"..^...^.....^..",
			"...............",
			".^.^.^.^.^...^.",
			"..............."
		};
	}

	static_assert(Solve(sampleInput()) == std::pair<size_t, size_t>{21, 40});
}