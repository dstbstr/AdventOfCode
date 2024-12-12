#include "Common.h"

#include <ranges>

SOLUTION(2024, 5) {
	using Map = Constexpr::BigMap<u32, std::vector<u32>, 1000>;
	constexpr Map GetRules(const std::vector<std::string>&lines) {
		Map result;
		for (const auto& line : lines) {
			auto parts = Constexpr::Split(line, "|");
			u32 key, value;
			Constexpr::ParseNumber(parts[0], key);
			Constexpr::ParseNumber(parts[1], value);

			result[key].emplace_back(value);
		}
		return result;
	}

	constexpr bool IsValid(const Map & rules, const std::vector<u32>&nums) {
		for (auto num : nums) {
			if (rules.contains(num)) {
				auto lhs = std::find(nums.begin(), nums.end(), num);
				auto afterValues = rules.at(num);
				for (auto afterValue : afterValues) {
					auto rhs = std::find(nums.begin(), nums.end(), afterValue);
					if (rhs != nums.end() && rhs < lhs) {
						return false;
					}
				}
			}
		}
		return true;
	}

	constexpr u32 SolvePart1(const std::vector<std::string>&lines) {
		auto groups = SplitInputIntoGroups(lines);
		auto rules = GetRules(groups[0]);
		u32 result = 0;
		for (const auto& line : groups[1]) {
			auto nums = ParseLineAsNumbers<u32>(line);
			if (IsValid(rules, nums)) {
				result += nums[nums.size() / 2];
			}
		}

		return result;
	}

	PART(1) {
		return SolvePart1(lines);
	}

	constexpr void Order(const Map & rules, std::vector<u32>&nums) {
		while (!IsValid(rules, nums)) {
			for (auto num : nums) {
				if (rules.contains(num)) {
					auto lhs = std::find(nums.begin(), nums.end(), num);
					const auto& afterValues = rules.at(num);
					for (auto afterValue : afterValues) {
						auto rhs = std::find(nums.begin(), nums.end(), afterValue);
						if (rhs != nums.end() && rhs < lhs) {
							std::iter_swap(lhs, rhs);
						}
					}
				}
			}
		}
	}

	constexpr u32 SolvePart2(const std::vector<std::string>&lines) {
		auto groups = SplitInputIntoGroups(lines);
		auto rules = GetRules(groups[0]);
		u32 result = 0;
		
		for (const auto& line : groups[1]) {
			auto nums = ParseLineAsNumbers<u32>(line);
			if (!IsValid(rules, nums)) {
				Order(rules, nums);
				result += nums[nums.size() / 2];
			}
		}
		
		return result;
	}

	PART(2) {
		return SolvePart2(lines);
	}

#if CLANG
	constexpr std::vector<std::string> GetTestInput() {
		return {
		"47|53"s,
		"97|13"s,
		"97|61"s,
		"97|47"s,
		"75|29"s,
		"61|13"s,
		"75|53"s,
		"29|13"s,
		"97|29"s,
		"53|29"s,
		"61|53"s,
		"97|53"s,
		"61|29"s,
		"47|13"s,
		"75|47"s,
		"97|75"s,
		"47|61"s,
		"75|61"s,
		"47|29"s,
		"75|13"s,
		"53|13"s,
		""s,
		"75,47,61,53,29"s,
		"97,61,53,29,13"s,
		"75,29,13"s,
		"75,97,47,61,53"s,
		"61,13,29"s,
		"97,13,75,29,47"s
		};
	}

	static_assert(SolvePart1(GetTestInput()) == 143);
	static_assert(SolvePart2(GetTestInput()) == 123);
#endif
}