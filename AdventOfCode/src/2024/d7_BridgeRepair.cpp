#include "Common.h"

SOLUTION(2024, 7) {
	constexpr bool Recurse(const std::vector<u64>& nums, size_t idx, u64 current, u64 target, auto... funcs) {
		if (idx == nums.size()) return current == target;
		auto next = nums[idx];

		return (Recurse(nums, idx + 1, funcs(current, next), target, funcs...) || ...);
	}

	constexpr u64 Solve(const std::vector<std::string>& lines, auto... funcs) {
		u64 result = 0;
		for(const auto& line : lines) {
			u64 ans;
			auto parts = Constexpr::Split(line, ": ");
			Constexpr::ParseNumber(parts[0], ans);
			auto values = ParseLineAsNumbers<u64>(parts[1], " ");
			result += Recurse(values, 1, values[0], ans, funcs...) ? ans : 0ull;
		}

		return result;
	}

	constexpr u64 plus(u64 a, u64 b) { return a + b; }
	constexpr u64 mul(u64 a, u64 b) { return a * b; }
	constexpr u64 concat(u64 a, u64 b) {
		u64 magnitude = 1ull;
		while (magnitude <= b) magnitude *= 10ull;
		return a * magnitude + b;
	}
	PART(1) {
		return Constexpr::ToString(Solve(lines, plus, mul));
	}

	PART(2) {
		return Constexpr::ToString(Solve(lines, plus, mul, concat));
	}

	constexpr std::vector<std::string> GetTestInput() {
		return {
		"190: 10 19",
		"3267: 81 40 27",
		"292: 11 6 16 20",
		"83: 17 5",
		"156: 15 6",
		"7290: 6 8 6 15",
		"161011: 16 10 13",
		"192: 17 8 14",
		"21037: 9 7 18 13"
		};
	}

	static_assert(Solve(GetTestInput(), plus, mul) == 3749ull);
	static_assert(Solve(GetTestInput(), plus, mul, concat) == 11387ull);
}