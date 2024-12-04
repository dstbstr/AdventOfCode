#include "Common.h"
#include "Core/Constexpr/ConstexprRegex.h"
#include "ctre.hpp"

SOLUTION(2024, 3) {
	constexpr s64 SumOfMuls(const std::string& line) {
		s64 result = 0;
		auto re = ctre::search_all<"mul\\((\\d{1,3}),(\\d{1,3})\\)">;
		for (auto match : re(line)) {
			auto lhs = match.get<1>().to_number();
			auto rhs = match.get<2>().to_number();
			result += lhs * rhs;
		}
		return result;
	}

	PART(1) {
		auto line = Constexpr::JoinVec("", lines);
		return Constexpr::ToString(SumOfMuls(line));
	}
	
	constexpr s64 SolvePart2(const std::string& line) {
		s64 result = 0;
		bool active = true;
		auto re = ctre::search_all<"mul\\((\\d{1,3}),(\\d{1,3})\\)|do\\(\\)|don't\\(\\)">;
		for (auto match : re(line)) {
			if (match == "do()") {
				active = true;
			}
			else if (match == "don't()") {
				active = false;
			}
			else if (active) {
				auto lhs = match.get<1>().to_number();
				auto rhs = match.get<2>().to_number();
				result += lhs * rhs;
			}
		}

		return result;
	}

	PART(2) {
		auto line = Constexpr::JoinVec("", lines);
		return Constexpr::ToString(SolvePart2(line));
	}

	static_assert(SumOfMuls("xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))") == 161);
	static_assert(SolvePart2("xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))") == 48);
}