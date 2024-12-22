#include "Common.h"
#include "Facing.h"

SOLUTION(2024, 21) {
	/*
		  0	  1   2
		+---+---+---+
		| 7 | 8 | 9 | 0
		+---+---+---+
		| 4 | 5 | 6 | 1
		+---+---+---+
		| 1 | 2 | 3 | 2
		+---+---+---+
		    | 0 | A | 3
		    +---+---+
          0   1   2
			+---+---+
			| ^ | A | 0
		+---+---+---+
		| < | v | > | 1
		+---+---+---+
		  0   1   2
	*/

	constexpr std::string Inverse(const std::string & path) {
		std::string result;
		for (char c : path) {
			switch (c) {
			case '^': result.push_back('v'); break;
			case 'v': result.push_back('^'); break;
			case '<': result.push_back('>'); break;
			case '>': result.push_back('<'); break;
			}
		}
		std::reverse(result.begin(), result.end());
		return result;
	}

	// paths can be shortened by pressing A multiple times in a row
	// it is better to go <^ than ^< because ^ is closer to A than < is
	// same with <v over v<

	using Paths = Constexpr::SmallMap<char, Constexpr::SmallMap<char, std::string>>;
	Paths shortestPath = []() {
		Paths result{};

		result['0']['0'] = "";
		result['0']['A'] = ">";
		result['0']['1'] = "^<";
		result['0']['2'] = "^";
		result['0']['3'] = "^>";
		result['0']['4'] = "^^<";
		result['0']['5'] = "^^";
		result['0']['6'] = ">^^";
		result['0']['7'] = "^^^<";
		result['0']['8'] = "^^^";
		result['0']['9'] = ">^^^";

		result['1']['1'] = "";
		result['1']['2'] = ">";
		result['1']['3'] = ">>";
		result['1']['4'] = "^";
		result['1']['5'] = "^>";
		result['1']['6'] = "^>>";
		result['1']['7'] = "^^";
		result['1']['8'] = "^^>";
		result['1']['9'] = "^^>>";
		result['1']['A'] = ">>v";

		result['2']['2'] = "";
		result['2']['3'] = ">";
		result['2']['4'] = "<^";
		result['2']['5'] = "^";
		result['2']['6'] = "^>";
		result['2']['7'] = "<^^";
		result['2']['8'] = "^^";
		result['2']['9'] = "^^>";
		result['2']['A'] = "v>";

		result['3']['3'] = "";
		result['3']['4'] = "<<^";
		result['3']['5'] = "<^";
		result['3']['6'] = "^";
		result['3']['7'] = "<<^^";
		result['3']['8'] = "<^^";
		result['3']['9'] = "^^";
		result['3']['A'] = "v";

		result['4']['4'] = "";
		result['4']['5'] = ">";
		result['4']['6'] = ">>";
		result['4']['7'] = "^";
		result['4']['8'] = "^>";
		result['4']['9'] = "^>>";
		result['4']['A'] = ">>vv";
		
		result['5']['5'] = "";
		result['5']['6'] = ">";
		result['5']['7'] = "<^";
		result['5']['8'] = "^";
		result['5']['9'] = "^>";
		result['5']['A'] = "vv>";

		result['6']['6'] = "";
		result['6']['7'] = "<<^";
		result['6']['8'] = "<^";
		result['6']['9'] = "^";
		result['6']['A'] = "vv";
		
		result['7']['7'] = "";
		result['7']['8'] = ">";
		result['7']['9'] = ">>";
		result['7']['A'] = ">>vvv";
		
		result['8']['8'] = "";
		result['8']['9'] = ">";
		result['8']['A'] = "vvv>";
		
		result['9']['9'] = "";
		result['9']['A'] = "vvv";
		// movepad
		result['^']['^'] = "";
		result['^']['A'] = ">";
		result['^']['>'] = "v>";
		result['^']['v'] = "v";
		result['^']['<'] = "v<";

		result['A']['A'] = "";
		result['A']['>'] = "v";
		result['A']['v'] = "<v";
		result['A']['<'] = "v<<";
		
		result['>']['>'] = "";
		result['>']['v'] = "<";
		result['>']['<'] = "<<";
		
		result['v']['v'] = "";
		result['v']['<'] = "<";
		
		result['<']['<'] = "";

		Constexpr::SmallMap<char, Constexpr::SmallMap<char, std::string>> inverses;

		for(const auto& [from, toPaths] : result) {
			for (const auto& [to, path] : toPaths) {
				inverses[to][from] = Inverse(path);
			}
		}

		for(const auto& [from, toPaths] : inverses) {
			for(const auto& [to, path] : toPaths) {
				result[from][to] = path;
			}
		}

		return result;
		}();

	constexpr std::string GetInstructions(const std::string& input) {
		std::string result;
		auto current = 'A';
		for(auto next : input) {
			result += shortestPath[current][next];
			result.push_back('A');
			current = next;
		}
		return result;
	}

	using Pairs = Constexpr::SmallMap<char, Constexpr::SmallMap<char, u64>>;
	constexpr Pairs MakePairs(const std::string& inst) {
		Pairs result{};
		auto current = 'A';
		for (auto next : inst) {
			result[current][next]++;
			current = next;
		}
		return result;
	}

	constexpr Pairs Next(const Pairs& pairs) {
		// need to know the number of paths from each node to each other node
		// i.e. A:0=1 A:1=0 A:3=5
		// Given this map, we can update the map with the number of transitions
		// from the shortestPaths
		// i.e. if 7:9 is ">>A", then we have A:>=1, >:>=1, >:A=1
		Pairs result{};
		for(const auto& [from, toCount] : pairs) {
			for (const auto& [to, count] : toCount) {
				char current = 'A';
				for (auto next : shortestPath[from][to]) {
					result[current][next] += count;
					current = next;
				}
				result[current]['A'] += count;
			}
		}

		return result;
	}

	constexpr u64 GetTotalComplexity(const std::vector<std::string>& lines, size_t iterations) {
		u64 result = 0u;

		for(const auto& line : lines) {
			auto pairs = MakePairs(GetInstructions(line));
			for(size_t i = 1u; i < iterations; i++) {
				pairs = Next(pairs);
			}
			u64 length = 0;
			for (const auto& [from, toCount] : pairs) {
				for (const auto& [to, count] : toCount) {
					length += count;
				}
			}
			u64 codeVal;
			Constexpr::ParseNumber(line.substr(0, 3), codeVal);
			result += length * codeVal;
		}
		return result;
	}

	PART(1) {
		return GetTotalComplexity(lines, 3);
	}

	PART(2) {
		return GetTotalComplexity(lines, 26);
	}
	TEST(1) {
		std::vector<std::string> lines = {
			"029A",
			"980A",
			"179A",
			"456A",
			"379A"
		};

		return GetTotalComplexity(lines, 3) == 126384;
	}
}