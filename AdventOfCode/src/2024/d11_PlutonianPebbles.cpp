#include "Common.h"

#include "Core/Concepts.h"

SOLUTION(2024, 11) {
	using Seen = Constexpr::BigMap<u64, std::vector<u64>>;

	constexpr std::pair<u64, std::optional<u64>> Next(u64 num) {
		if (num == 0) return {1, std::nullopt};
		auto digits = Constexpr::CountDigits(num);
		if (digits % 2 == 1) {
			return {num * 2024, std::nullopt};
		}
		else {
			auto factor = Constexpr::Pow(10, digits / 2);
			auto lhs = num / factor;
			auto rhs = num % factor;
			return { lhs, rhs };
		}
	}

	constexpr void Recurse(u64 num, size_t remaining, Seen& seen) {
		auto& cache = seen[num];
		if(cache.empty()) {
			cache.resize(76);
			cache[0] = 1;
		}

		if (cache[remaining] > 0) {
			return;
		}

		auto [lhs, rhs] = Next(num);
		Recurse(lhs, remaining - 1, seen);
		auto count = seen[lhs][remaining - 1];

		if (rhs.has_value()) {
			Recurse(rhs.value(), remaining - 1, seen);
			count += seen[rhs.value()][remaining - 1];
		}

		cache[remaining] = count;
	}

	constexpr u64 Solve(const std::vector<u64>& nums, size_t iters) {
		Seen seen;
		for (auto num : nums) {
			Recurse(num, iters, seen);
		}
		u64 result = 0;
		for (auto num : nums) {
			result += seen[num][iters];
		}
		return result;
	}

	PART(1) {
		auto nums = ParseLineAsNumbers<u64>(lines[0], " ");
		return Solve(nums, 25);
	}

	PART(2) {
		auto nums = ParseLineAsNumbers<u64>(lines[0], " ");
		return Solve(nums, 75);
	}

	static_assert(Next(0u) == std::make_pair(1u, std::nullopt));
	static_assert(Next(1u) == std::make_pair(2024u, std::nullopt));
	static_assert(Next(2024u) == std::make_pair(20u, 24u));
	static_assert(Next(125u) == std::make_pair(253000u, std::nullopt));
	static_assert(Next(253000u) == std::make_pair(253u, 0u));
}