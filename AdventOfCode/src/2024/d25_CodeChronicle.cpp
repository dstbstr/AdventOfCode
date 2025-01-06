#include "Common.h"

SOLUTION(2024, 25) {
	constexpr void ParseGroup(const std::vector<std::string>&group, std::vector<std::vector<u8>>&keys, std::vector<std::vector<u8>>&locks) {
		std::vector<u8> output;
		output.resize(group[0].size());

		for(size_t row = 1; row < group.size() - 1; row++) {
			for(size_t col = 0; col < group[0].size(); col++) {
				output[col] += group[row][col] == '#';
			}
		}

		group[0][0] == '.' ? keys.push_back(output) : locks.push_back(output);
	}

	constexpr u32 SolvePart1(const std::vector<std::string>& lines) {
		auto groups = SplitInputIntoGroups(lines);
		std::vector<std::vector<u8>> keys;
		std::vector<std::vector<u8>> locks;
		for(const auto& group : groups) {
			ParseGroup(group, keys, locks);
		}

		u32 result = 0;
		for(const auto& key: keys) {
			for(const auto& lock : locks) {
				bool fit = true;
				for(size_t i = 0; i < key.size(); i++) {
					if(key[i] + lock[i] > 5) {
						fit = false;
						break;
					}
				}
				result += fit;
			}
		}

		return result;
	}

	PART(1) {
		return SolvePart1(lines);
	}
	TEST(1) {
		std::vector<std::string> lines = {
			"#####",
			".####",
			".####",
			".####",
			".#.#.",
			".#...",
			".....",
			"",
			"#####",
			"##.##",
			".#.##",
			"...##",
			"...#.",
			"...#.",
			".....",
			"",
			".....",
			"#....",
			"#....",
			"#...#",
			"#.#.#",
			"#.###",
			"#####",
			"",
			".....",
			".....",
			"#.#..",
			"###..",
			"###.#",
			"###.#",
			"#####",
			"",
			".....",
			".....",
			".....",
			"#....",
			"#.#..",
			"#.#.#",
			"#####",
		};

		return SolvePart1(lines) == 3;
	}
}