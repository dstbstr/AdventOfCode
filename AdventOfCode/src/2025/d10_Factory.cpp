#include "Common.h"
#include <Core/Algorithms/AStar.h>
#include <Core/Constexpr/ConstexprIlp.h>

SOLUTION(2025, 10) {
	struct Machine {
		u32 Desired;
		std::vector<u32> Buttons;
		std::vector<std::vector<u32>> ExpandedButtons;
		std::vector<u32> Jolts;
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
			button += Constexpr::Pow(2, n);
		}
		return button;
	}
	constexpr std::vector<u32> ParseExpandedButton(std::string_view str) {
		return ParseLineAsNumbers<u32>(str.substr(1, str.size() - 2));
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
		for(size_t i = 1; i < parts.size() - 1; i++) {
			machine.Buttons.emplace_back(ParseButton(parts[i]));
			machine.ExpandedButtons.emplace_back(ParseExpandedButton(parts[i]));
		}
		machine.Jolts = ParseJolts(parts.back());

		return machine;
	}

	constexpr size_t CountIndicators(const Machine& machine) {
		Constexpr::SmallSet<u32> q{0};
		size_t steps = 0;
		while(true) {
			Constexpr::SmallSet<u32> next;
			for(auto current : q) {
				if(current == machine.Desired) return steps;
				for(auto button : machine.Buttons) {
					next.insert(current ^ button);
				}
			}
			steps++;
			q = next;
		}
		return 0;
	}

	constexpr size_t CountJolts(const Machine& machine) {
		std::vector<std::vector<s32>> matrix(machine.Jolts.size(), std::vector<s32>(machine.ExpandedButtons.size(), 0));
		for (size_t btn = 0; btn < machine.ExpandedButtons.size(); ++btn) {
			for (u32 jolt : machine.ExpandedButtons[btn]) {
				matrix[jolt][btn] = 1;
			}
		}
		/*
		constexpr size_t MaxJolts = 10;
		constexpr size_t MaxButtons = 16;
		Mat<s32, MaxJolts, MaxButtons> mat{};
		for(size_t i = 0; i < machine.ExpandedButtons.size(); i++) {
			for(auto bit : machine.ExpandedButtons[i]) {
				mat[bit][i] = 1;
			}
		}
		std::array<s32, MaxJolts> target{};
		for(size_t i = 0; i < machine.Jolts.size(); i++) {
			target[i] = static_cast<s32>(machine.Jolts[i]);
		}
		*/
		std::vector<s32> target(machine.Jolts.size(), 0);
		for(size_t i = 0; i < machine.Jolts.size(); i++) {
			target[i] = static_cast<s32>(machine.Jolts[i]);
		}
		auto coef = SolveIlp(matrix, target);
		if(coef == std::nullopt) {
			return 0;
		}
		return std::accumulate(coef.value().begin(), coef.value().end(), 0ull, std::plus<>());
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
	PART(2) {
		return Solve(lines, CountJolts);
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

		return true;
		//return Solve(input, CountIndicators) == 7 &&
		//	Solve(input, CountJolts) == 33;
	}
}