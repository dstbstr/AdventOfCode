#include "Common.h"

SOLUTION(2025, 3) {
	constexpr size_t FindLargest(std::string_view str, size_t digitsLeft) {
		if (digitsLeft == 0) return 0;
		size_t result = 0;
		char best = '0';
		size_t bestIndex = 0;
		for(size_t i = 0; i < str.size() - digitsLeft + 1; i++) {
			auto current = str[i];
			if(current > best) {
				best = current;
				bestIndex = i;
				if (best == '9') break;
			}
		}
		result = (best - '0') * Constexpr::Pow(10ull, digitsLeft - 1);
		return result + FindLargest(str.substr(bestIndex + 1), digitsLeft - 1);
	}

	static_assert(FindLargest("987654321111111", 2) == 98);
	static_assert(FindLargest("811111111111119", 2) == 89);
	static_assert(FindLargest("234234234234278", 2) == 78);
	static_assert(FindLargest("818181911112111", 2) == 92);

	static_assert(FindLargest("987654321111111", 12) == 987654321111);
	static_assert(FindLargest("811111111111119", 12) == 811111111119);
	static_assert(FindLargest("234234234234278", 12) == 434234234278);
	static_assert(FindLargest("818181911112111", 12) == 888911112111);

	constexpr size_t Solve(const std::vector<std::string>& lines, size_t digits) {
		size_t total = 0;
		for(const auto& line : lines) {
			total += FindLargest(line, digits);
		}
		return total;
	}

	PART(1) {
		return Solve(lines, 2);
	}
	PART(2) {
		return Solve(lines, 12);
	}
}