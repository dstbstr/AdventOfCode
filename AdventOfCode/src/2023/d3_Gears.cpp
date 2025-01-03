#include "Common.h"

SOLUTION(2023, 3) {

	struct Number {
		u32 Val = 0;
		RowCol Start;
		RowCol End;

		constexpr bool IsNear(RowCol pos) const {
			if (Constexpr::AbsDistance(pos.Row, Start.Row) > 1) return false;
			auto left = Start.Col == 0 ? 0 : Start.Col - 1;
			auto right = End.Col + 1;
			return left <= pos.Col && pos.Col <= right;
		}
	};

	constexpr void Parse(const auto & lines, std::vector<Number>&outNumbers, Constexpr::SmallMap<RowCol, char>&outSymbols) {
		std::string currentNumber;

		auto InsertNumber = [&](size_t row, size_t col) {
			if (!currentNumber.empty()) {
				Number num;
				Constexpr::ParseNumber(currentNumber, num.Val);
				num.End = { row, col - 1};
				num.Start = num.End - RowCol{0, currentNumber.size() - 1};
				outNumbers.push_back(num);
			}
			currentNumber.clear();
		};
	
		for (size_t row = 0; row < lines.size(); row++) {
			for (size_t col = 0; col < lines[row].size(); col++) {
				auto c = lines[row][col];
				if (c == '.') {
					InsertNumber(row, col);
				}
				else if (std::isdigit(c)) {
					currentNumber.push_back(c);
				}
				else {
					outSymbols[{ row, col }] = c;
					InsertNumber(row, col);
				}
			}
			InsertNumber(row, lines[row].size());
		}
	}

	PART(1) {
		std::vector<Number> numbers;
		Constexpr::SmallMap<RowCol, char> symbols{};
		Parse(lines, numbers, symbols);
		u32 result = 0;
		
		for (const auto& num : numbers) {
			for (auto [pos, c] : symbols) {
				if (num.IsNear(pos)) {
					result += num.Val;
					break;
				}
			}
		}
		
		return result;
	}

	PART(2) {
		std::vector<Number> numbers;
		Constexpr::SmallMap<RowCol, char> symbols{};
		Parse(lines, numbers, symbols);
		u32 result = 0;

		for (const auto& [pos, c] : symbols) {
			if (c != '*') continue;
			u32 lhs = 0;
			u32 rhs = 0;
			bool third = false;
			for (const auto& num : numbers) {
				if (num.IsNear(pos)) {
					if (lhs == 0) lhs = num.Val;
					else if (rhs == 0) rhs = num.Val;
					else {
						third = true;
						break;
					}
				}
			}
			if (lhs && rhs && !third) {
				result += lhs * rhs;
			}
		}
		return result;
	}

	TEST(1) {
		std::vector<std::string> input = {
			"467..114..",
			"...*......",
			"..35..633.",
			"......#...",
			"617*......",
			".....+.58.",
			"..592.....",
			"......755.",
			"...$.*....",
			".664.598.."
		};

		std::vector<Number> numbers;
		Constexpr::SmallMap<RowCol, char> symbols{};
		Parse(input, numbers, symbols);

		if (numbers.size() != 10) return false;
		if (numbers[0].Val != 467) return false;
		if (numbers[0].Start != RowCol{ 0, 0 }) return false;
		if (numbers[0].End != RowCol{ 0, 2 }) return false;

		if (symbols.size() != 6) return false;

		if (std::get<u32>(Part1(input)) != 4361) return false;

		return true;
	}

	constexpr Number TestNum{ 12345, {0, 0}, {0, 4} };
	static_assert(TestNum.IsNear({ 0, 5 }));
	static_assert(!TestNum.IsNear({ 2, 2 }));

	constexpr Number TestNum2{ 12345, {3, 3}, {3, 7} };
	static_assert(TestNum2.IsNear({ 3, 2 }));
	static_assert(TestNum2.IsNear({ 3, 8 }));
	static_assert(TestNum2.IsNear({ 2, 2 }));
	static_assert(TestNum2.IsNear({ 4, 2 }));
	static_assert(TestNum2.IsNear({ 2, 7 }));
	static_assert(TestNum2.IsNear({ 2, 8 }));
	static_assert(TestNum2.IsNear({ 4, 7 }));
	static_assert(TestNum2.IsNear({ 4, 8 }));
	static_assert(TestNum2.IsNear({ 4, 5 }));

	static_assert(!TestNum2.IsNear({ 2, 1 }));
	static_assert(!TestNum2.IsNear({ 2, 9 }));
	static_assert(!TestNum2.IsNear({ 4, 1 }));
	static_assert(!TestNum2.IsNear({ 4, 9 }));
	static_assert(!TestNum2.IsNear({ 1, 5 }));
}