#include "Common.h"

SOLUTION(2025, 5) {
	struct Range {
		size_t Begin, End;
		constexpr bool CanCombine(const Range& other) const {
			return !(other.End < Begin || other.Begin > End);
		}
		constexpr bool Contains(size_t value) const {
			return value >= Begin && value <= End;
		}
		constexpr size_t Count() const {
			return End - Begin + 1;
		}
	};

	static_assert(Range{ 5, 10 }.CanCombine(Range{ 8, 12 }));
	static_assert(Range{ 5, 10 }.CanCombine(Range{ 10, 15 }));
	static_assert(Range{ 5, 10 }.Count() == 6);

	constexpr Range ParseRange(std::string_view str) {
		auto parts = Constexpr::Split(str, "-");
		Range result;
		Constexpr::ParseNumber(parts[0], result.Begin);
		Constexpr::ParseNumber(parts[1], result.End);
		return result;
	}

	static_assert(ParseRange("5-10").Begin == 5);
	static_assert(ParseRange("5-10").End == 10);

	constexpr std::vector<Range> CombineRanges(const std::vector<Range>& ranges) {
		if (ranges.empty()) return {};
		std::vector<Range> sorted = ranges;
		std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.Begin < b.Begin; });
		std::vector<Range> result;
		result.push_back(sorted[0]);
		for (size_t i = 1; i < sorted.size(); i++) {
			auto current = sorted[i];
			auto& last = result.back();
			if(last.CanCombine(current)) {
				last.End = std::max(last.End, current.End);
			} else {
				result.push_back(current);
			}
		}
		return result;
	}

	constexpr size_t SolvePart1(const std::vector<std::string>& lines) {
		auto groups = SplitInputIntoGroups(lines);
		auto ranges = ParseLines(groups[0], ParseRange);
		auto combined = CombineRanges(ranges);
		auto numbers = ParseLinesAsNumbers<size_t>(groups[1]);
		size_t total = 0;
		for(auto number : numbers) {
			for(auto range : combined) {
				if(range.Contains(number)) {
					total++;
					break;
				}
			}
		}
		return total;
	}
	PART(1) {
		return SolvePart1(lines);
	}
	PART(2) {
		auto groups = SplitInputIntoGroups(lines);
		auto ranges = ParseLines(groups[0], ParseRange);
		auto combined = CombineRanges(ranges);

		return std::accumulate(combined.begin(), combined.end(), 0ull, [](size_t total, const auto& range) {
			return total + range.Count();
		});
	}

	constexpr bool TestPart1() {
		std::vector<std::string> input = {
			"3-5",
			"10-14",
			"16-20",
			"12-18",
			"",
			"1",
			"5",
			"8",
			"11",
			"17",
			"32"
		};

		return SolvePart1(input) == 3;
	}

	static_assert(TestPart1());
}
