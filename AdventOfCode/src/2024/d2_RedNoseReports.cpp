#include "Common.h"

SOLUTION(2024, 2) {
	constexpr bool IsBadLevel(s32 lhs, s32 rhs, bool ascending) {
		auto diff = rhs - lhs;
		/*		
		if ((diff < 0) == ascending) return true;
		return diff == 0 || diff > 3 || diff < -3;
		*/
		// yay for branchless programming
		return ((diff < 0) == ascending) + (diff == 0) + (diff > 3) + (diff < -3);
	}

	constexpr bool IsSafe(const std::vector<s32>& vals) {
		bool ascending = vals[0] < vals[1];
		for (size_t i = 1; i < vals.size(); i++) {
			if (IsBadLevel(vals[i - 1], vals[i], ascending)) return false;
		}
		return true;
	}

	constexpr bool IsSafeTolerance(const std::vector<s32>& vals) {
		if (IsSafe(vals)) return true;
		auto withoutFirst = vals;
		withoutFirst.erase(withoutFirst.begin());
		if (IsSafe(withoutFirst)) return true;

		// the previous check isn't required if we change vals[1] to vals.back,
		// but this approach is faster (probably a cache miss)
		bool ascending = vals[0] < vals[1];
		for(size_t i = 1; i < vals.size(); i++) {
			if (IsBadLevel(vals[i - 1], vals[i], ascending)) {
				auto first = vals;
				auto second = vals;

				first.erase(first.begin() + i);
				second.erase(second.begin() + i - 1);

				return IsSafe(first) || IsSafe(second);
			}
		}

		return true;
	}

	PART(1) {
		u32 result = 0;
		for(const auto& line : lines) {
			auto vals = ParseLineAsNumbers<s32>(line, " ");
			result += IsSafe(vals);
		}
		return result;
	}

	PART(2) {
		u32 result = 0;
		for (const auto& line : lines) {
			auto vals = ParseLineAsNumbers<s32>(line, " ");
			result += IsSafeTolerance(vals);
		}
		return result;
	}

	static_assert(IsSafe({ 7, 6, 4, 2, 1 }));
	static_assert(IsSafe({ 1, 3, 6, 7, 9 }));

	static_assert(!IsSafe({ 1, 2, 7, 8, 9 }));
	static_assert(!IsSafe({ 9, 7, 6, 2, 1 }));
	static_assert(!IsSafe({ 8, 6, 4, 4, 1 }));
	static_assert(!IsSafe({ 1, 3, 2, 4, 5 }));

	static_assert(IsSafeTolerance({ 7, 6, 4, 2, 1 }));
	static_assert(IsSafeTolerance({ 1, 3, 6, 7, 9 }));
	static_assert(IsSafeTolerance({ 1, 3, 2, 4, 5 }));
	static_assert(IsSafeTolerance({ 8, 6, 4, 4, 1 }));

	static_assert(!IsSafeTolerance({ 1, 2, 7, 8, 9 }));
	static_assert(!IsSafeTolerance({ 9, 7, 6, 2, 1 }));
}