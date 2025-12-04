#include "Common.h"

SOLUTION(2025, 2) {
	struct Range{size_t Start, End;};
	constexpr Range ParseRange(std::string_view str) {
		Range range;
		auto parts = Constexpr::Split(str, "-");
		Constexpr::ParseNumber(parts[0], range.Start);
		Constexpr::ParseNumber(parts[1], range.End);
		return range;
	}
	constexpr bool IsRepeat(size_t val) {
		auto digits = Constexpr::CountDigits(val);
		auto mid = Constexpr::Pow(10, digits / 2);
		return val % mid == val / mid;
	}

	static_assert(IsRepeat(1212));
	static_assert(IsRepeat(11));
	static_assert(IsRepeat(22));
	static_assert(IsRepeat(1010));
	static_assert(IsRepeat(1188511885));
	static_assert(IsRepeat(222222));
	static_assert(IsRepeat(38593859));

	static_assert(!IsRepeat(1234));
	static_assert(!IsRepeat(101));

	constexpr bool HasAnyRepeat(size_t val) {
		auto digits = Constexpr::CountDigits(val);
		auto max = digits / 2;
		for(size_t size = 1; size <= max; size++) {
			if (digits % size != 0) continue;

			auto window = Constexpr::Pow(10, size);
			auto pattern = val % window;
			auto count = digits / size;
			auto running = val;
			bool allMatch = true;
			for(size_t i = 1; i < count; i++) {
				running /= window;
				if (running % window != pattern) {
					allMatch = false;
					break;
				}
			}
			if (allMatch) return true;
		}
		return false;
	}

	static_assert(HasAnyRepeat(111));
	static_assert(HasAnyRepeat(565656));
	static_assert(HasAnyRepeat(123123123));
	static_assert(HasAnyRepeat(1212121212));

	static_assert(!HasAnyRepeat(1234));
	static_assert(!HasAnyRepeat(12312123));

	constexpr size_t SumInvalid(Range rng, auto rule) {
		size_t sum = 0;
		for(auto i = rng.Start; i <= rng.End; i++) {
			sum += i * rule(i);
		}
		return sum;
	}
	
	constexpr size_t Solve(const std::vector<Range>& ranges, auto rule) {
		size_t total = 0;
		for(const auto& rng : ranges) {
			total += SumInvalid(rng, rule);
		}
		return total;
	}
	PART(1) {
		auto parts = Constexpr::Split(lines[0], ",");
		auto ranges = ParseLines(parts, ParseRange);
		return Solve(ranges, IsRepeat);
	}
	PART(2) {
		auto parts = Constexpr::Split(lines[0], ",");
		auto ranges = ParseLines(parts, ParseRange);
		return Solve(ranges, HasAnyRepeat);
	}
	static_assert(SumInvalid({ 11, 22 }, IsRepeat) == 33);
	static_assert(SumInvalid({ 95, 115 }, HasAnyRepeat) == 99 + 111);
	static_assert(SumInvalid({ 998, 1012 }, HasAnyRepeat) == 999 + 1010);

	static_assert(Solve({
		{11, 22},
		{95, 115},
		{998, 1012},
		{1188511880, 1188511890},
		{222220, 222224},
		{1698522, 1698528},
		{446443, 446449},
		{38593856, 38593862},
		{565653, 565659},
		{824824821, 824824827},
		{2121212118, 2121212124}
		}, IsRepeat) == 1227775554);
		

}