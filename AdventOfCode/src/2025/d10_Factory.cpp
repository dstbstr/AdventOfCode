#include "Common.h"
#include <Core/Constexpr/ConstexprBits.h>

SOLUTION(2025, 10) {
	using Cache = Constexpr::BigMap<std::vector<u32>, u32, 5'000>;
	using Patterns = Constexpr::BigMap<u32, std::vector<std::pair<std::vector<u32>, u32>>, 5'000>;

	struct Machine {
		u32 Desired;
		std::vector<u32> Buttons;
		std::vector<u32> Jolts;
	};

	constexpr u32 ParseDesired(std::string_view str) {
		u32 result = 0;
		for(size_t i = 1; i < str.size() - 1; i++) {
			result |= ((str[i] == '#') << (i - 1));
		}
		return result;
	}
	static_assert(ParseDesired("[.##.]") == 6);
	static_assert(ParseDesired("[...#.]") == 8);
	static_assert(ParseDesired("[.###.#]") == 46);

	constexpr u32 ToPairity(const std::vector<u32>& jolts) {
		u32 result = 0;
		for (size_t i = 0; i < jolts.size(); i++) {
			result |= ((jolts[i] & 1) << i);
		}
		return result;
	}

	static_assert(ToPairity({ 3,5,4,7 }) == 11);

	constexpr u32 ParseButton(std::string_view str) {
		u32 button = 0;
		auto nums = ParseLineAsNumbers<u32>(str.substr(1, str.size() - 2));
		for (auto n : nums) {
			button += (1 << n);
		}
		return button;
	}

	static_assert(ParseButton("(3)") == 8);
	static_assert(ParseButton("(0,2)") == 5);

	constexpr std::vector<u32> ParseJolts(std::string_view str) {
		return ParseLineAsNumbers<u32>(str.substr(1, str.size() - 2));
	}
	
	constexpr Machine ParseMachine(const std::string& line) {
		auto parts = Constexpr::Split(line, " ");
		Machine machine;
		machine.Desired = ParseDesired(parts[0]);

		for (size_t i = 1; i < parts.size() - 1; i++) {
			machine.Buttons.emplace_back(ParseButton(parts[i]));
		}

		machine.Jolts = ParseJolts(parts.back());

		return machine;
	}

	constexpr Patterns GetPatterns(const std::vector<u32>& buttons, size_t numVariables) {
		Patterns result;
		const size_t numButtons = buttons.size();
		u32 max = 1u << numButtons;
		for (u32 mask = 0; mask < max; mask++) {
			u32 buttonsPressed = std::popcount(mask);
			std::vector<u32> pattern(numVariables, 0);

			for (size_t i = 0; i < numButtons; i++) {
				if (Constexpr::IsBitSet(mask, i)) {
					auto btn = buttons[i];
					for (size_t j = 0; j < numVariables; j++) {
						pattern[j] += (btn >> j) & 1;
					}
				}
			}
			auto pairityPattern = ToPairity(pattern);
			result[pairityPattern].emplace_back(pattern, buttonsPressed);
		}
		return result;
	}

	constexpr u32 Recurse(const std::vector<u32>& goal, Cache& memo, const Patterns& patterns) {
		if (memo.contains(goal)) return memo.at(goal);
		if (std::all_of(goal.begin(), goal.end(), [](u32 g) { return g == 0; })) {
			memo[goal] = 0;
			return 0;
		}
		u32 answer = 1'000'000;
		auto pattern = ToPairity(goal);
		if (!patterns.contains(pattern)) {
			memo[goal] = answer;
			return answer;
		}
		for (const auto& [buttons, cost] : patterns.at(pattern)) {
			auto newGoal = goal;
			bool valid = true;

			for (size_t i = 0; i < goal.size(); i++) {
				if (buttons[i] > goal[i]) {
					valid = false;
					break;
				}
				newGoal[i] = (goal[i] - buttons[i]) / 2;
			}
			if (valid) {
				answer = std::min(answer, cost + 2 * Recurse(newGoal, memo, patterns));
			}
		}
		memo[goal] = answer;
		return answer;

	}

	constexpr size_t Solve(const std::vector<std::string>& lines, auto fn) {
		size_t score = 0;
		for (const auto& line : lines) {
			auto machine = ParseMachine(line);
			auto patterns = GetPatterns(machine.Buttons, machine.Jolts.size());
			Cache memo;
			score += fn(machine, memo, patterns);
		}
		return score;
	}

	constexpr size_t MinMatch(const Machine& machine, Cache& cache, const Patterns& patterns) {
		(void)cache;
		auto matches = patterns.at(machine.Desired);
		size_t best = std::numeric_limits<size_t>::max();
		for (const auto& [_, presses] : matches) {
			if (presses < best) {
				best = presses;
			}
		}
		return best;
	}

	constexpr size_t MinPresses(const Machine& machine, Cache& cache, const Patterns& patterns) {
		return Recurse(machine.Jolts, cache, patterns);
	}

	PART(1) {
		return Solve(lines, MinMatch);
	}

	PART(2) {
		return Solve(lines, MinPresses);
	}

	TEST(1) {
		std::vector<std::string> input = {
			"[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}",
			"[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}",
			"[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}"
		};

		return Solve(input, MinPresses) == 33;
	}
	
}