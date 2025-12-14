#include "Common.h"
#include <Core/Constexpr/ConstexprBits.h>

SOLUTION(2025, 10) {
	//struct Machine {
	//	u32 Desired;
	//	std::vector<u32> Buttons;
	//	std::vector<u32> Jolts;
	//};

	static constexpr size_t MAX_BUTTONS = 16;
	static constexpr size_t MAX_JOLTS = 11;
	using Buttons = std::array<u32, MAX_BUTTONS>;
	using Jolts = std::array<u32, MAX_JOLTS>;

	struct Machine {
		u32 Desired;
		Buttons Buttons;
		size_t ButtonCount = 0;
		Jolts Jolts;
		size_t JoltCount = 0;
	};

	constexpr u32 ParseDesired(std::string_view str) {
		u32 result = 0;
		u32 factor = 1;
		for(auto it = str.begin() + 1; it != str.end() - 1; ++it) {
			result += (*it == '#') * factor;
			factor *= 2;
		}
		return result;
	}

	static_assert(ParseDesired("[.##.]") == 6);
	static_assert(ParseDesired("[...#.]") == 8);
	static_assert(ParseDesired("[.###.#]") == 46);


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

	/*
	constexpr Machine ParseMachine(const std::string& line) {
		auto parts = Constexpr::Split(line, " ");
		Machine machine;
		machine.Desired = ParseDesired(parts[0]);
		// Fill Buttons
		for (size_t i = 1; i < parts.size() - 1; i++) {
			machine.Buttons.emplace_back(ParseButton(parts[i]));
		}

		machine.Jolts = ParseJolts(parts.back());

		// Available buttons is always last jolt
		machine.Jolts.push_back((1 << machine.Buttons.size()) - 1);
		return machine;
	}
	*/
	
	constexpr Machine ParseMachine(const std::string& line) {
		auto parts = Constexpr::Split(line, " ");
		Machine machine;
		machine.Desired = ParseDesired(parts[0]);
		// Fill Buttons
		machine.ButtonCount = 0;
		for(size_t i = 1; i < parts.size() - 1; i++) {
			machine.Buttons[machine.ButtonCount++] = ParseButton(parts[i]);
		}
		// Fill Jolts
		auto joltsVec = ParseJolts(parts.back());
		machine.JoltCount = joltsVec.size();
		for(size_t i = 0; i < joltsVec.size(); ++i) {
			machine.Jolts[i] = joltsVec[i];
		}
		// Available buttons is always last jolt
		machine.Jolts.back() = (1 << machine.ButtonCount) - 1;
		return machine;
	}
	

	constexpr size_t CountIndicators(const Machine& machine) {
		Constexpr::SmallSet<u32> q{0};
		size_t steps = 0;
		while(true) {
			Constexpr::SmallSet<u32> next;
			for(auto current : q) {
				if(current == machine.Desired) return steps;

				for(size_t i = 0; i < machine.ButtonCount; i++) {
					next.insert(current ^ (machine.Buttons[i]));
				}
			}
			steps++;
			q = next;
		}
		return 0;
	}

	constexpr size_t Solve(const std::vector<std::string>& lines, auto fn) {
		auto machines = ParseLines(lines, ParseMachine);
		size_t totalSteps = 0;
		for (const auto& machine : machines) {
			totalSteps += fn(machine);
		}
		return totalSteps;
	}
	PART(1) {
		return Solve(lines, CountIndicators);
	}

    constexpr bool NextCombination(std::vector<u32>& current) {
        for(size_t i = current.size() - 1; i > 0; i--) {
            if(current[i] > 0) {
				auto v = current[i];
                current[i - 1] += 1;
                current[i] = 0;
                current.back() = v - 1;
                return true;
            }
		}
        return false;
    }
	constexpr bool NextCombination(std::array<u32, 16>& current, size_t end) {
		for (size_t i = end - 1; i > 0; i--) {
			if (current[i] > 0) {
				auto v = current[i];
				current[i - 1] += 1;
				current[i] = 0;
				current[end - 1] = v - 1;
				return true;
			}
		}
		return false;
	}

	// {presses, buttonIndecies, indexCount}
	constexpr std::tuple<u32, std::array<size_t, MAX_BUTTONS>, size_t> GetNextButtons(const Jolts& jolts, size_t joltCount, const Buttons& buttons, size_t buttonCount) {
		size_t minIndex = 0;
		size_t minCount = std::numeric_limits<size_t>::max();
		auto availBits = jolts.back();
		for (size_t joltIndex = 0; joltIndex < joltCount; joltIndex++) {
			if (jolts[joltIndex] == 0) continue;
			size_t btnCount = 0;
			for (size_t btnIdx = 0; btnIdx < buttonCount; btnIdx++) {
				btnCount += Constexpr::IsBitSet(buttons[btnIdx], joltIndex) && Constexpr::IsBitSet(availBits, btnIdx);
			}
			if (minCount == btnCount && jolts[joltIndex] < jolts[minIndex]) minIndex = joltIndex;
			else if (btnCount < minCount) {
				minCount = btnCount;
				minIndex = joltIndex;
			}
		}

		std::array<size_t, MAX_BUTTONS> result{};
		size_t index = 0;
		for(size_t i = 0; i < buttonCount; i++) {
			if(Constexpr::IsBitSet(availBits, i) && Constexpr::IsBitSet(buttons[i], minIndex)) {
				result[index++] = i;
			}
		}
		return { jolts[minIndex], result, index };
	}
	constexpr size_t Part2Recurse(const Jolts& jolts, size_t joltCount, const Buttons& buttons, size_t buttonCount) {
		if (std::all_of(jolts.begin(), jolts.begin() + joltCount, [](u32 j) { return j == 0; })) {
			return 0;
		}

		auto [presses, nextButtonIndecies, indexCount] = GetNextButtons(jolts, joltCount, buttons, buttonCount);
		size_t result = std::numeric_limits<size_t>::max();
		if (indexCount == 0) return result;

		auto nextBits = jolts.back();
		for(size_t i = 0; i < indexCount; i++) {
			nextBits &= ~(1 << nextButtonIndecies[i]);
		}

		std::array<u32, MAX_BUTTONS> counts{};
		counts[indexCount - 1] = presses;
		size_t r;
		while (true) {
			auto nextJolts = jolts;
			nextJolts.back() = nextBits;
			for (size_t i = 0; i < indexCount; i++) {
				if (counts[i] == 0) continue;
				auto count = counts[i];
				auto btn = buttons[nextButtonIndecies[i]];
				for (size_t j = 0; j < joltCount; j++) {
					if(Constexpr::IsBitSet(btn, j)) {
						if(nextJolts[j] < count) {
							goto skipRecurse;
						}
						nextJolts[j] -= count;
					}
				}
			}
			r = Part2Recurse(nextJolts, joltCount, buttons, buttonCount);
			if (r != std::numeric_limits<size_t>::max()) {
				result = std::min(result, r + presses);
			}

			skipRecurse:
			if (!NextCombination(counts, indexCount)) {
				break;
			}
		}

		return result;
	}

    constexpr bool TestNextCombination() {
		std::vector<u32> comb = { 0, 0, 4 };
        Constexpr::SmallSet<std::vector<u32>> seen{};
        seen.insert(comb);
        while(NextCombination(comb)) {
            seen.insert(comb);
		}
		return seen.size() == 15;
    }
    static_assert(TestNextCombination());

	constexpr size_t SolvePart2(const std::vector<std::string>& lines) {
		auto machines = ParseLines(lines, ParseMachine);
		size_t total = 0;
		for (const auto& machine : machines) {
			total += Part2Recurse(machine.Jolts, machine.JoltCount, machine.Buttons, machine.ButtonCount);
		}
		return total;
	}

	PART(2) {
		return SolvePart2(lines);
	}
	
	/*
	3,5,4,7
	(3)
	(1,3)x3
	(2,3)x3
	(0,2)
	(0,1)x2
	*/
	TEST(1) {
		std::vector<std::string> input = {
			"[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}",
			"[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}",
			"[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}"
		};

		return SolvePart2(input) == 33;
	}
}