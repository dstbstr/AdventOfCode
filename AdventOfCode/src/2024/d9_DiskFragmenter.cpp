#include "Common.h"

#include <utility>

SOLUTION(2024, 9) {
	constexpr u32 Empty = 999999;

	constexpr std::vector<u32> Parse(const std::string & input) {
		std::vector<u32> result;
		auto sum = std::accumulate(input.begin(), input.end(), 0u, [](u32 acc, char c) {
			return acc + (c - '0');
		});
		result.reserve(sum);

		bool isFile = true;
		u32 value = 0;
		size_t index = 0;
		for (auto c : input) {
			auto count = c - '0';
			if(isFile) {
				result.insert(result.begin() + index, count, value);
				value++;
			} else {
				result.insert(result.begin() + index, count, Empty);
			}
			isFile = !isFile;
			index += count;
		}

		return result;
	}

	static_assert(Parse("12345") == std::vector<u32>{0, Empty, Empty, 1, 1, 1, Empty, Empty, Empty, Empty, 2, 2, 2, 2, 2});

	constexpr void Compact(std::vector<u32>& input) {
		size_t lhs = 0;
		size_t rhs = input.size() - 1;
		
		while(lhs < rhs) {
			while(input[lhs] != Empty) {
				lhs++;
			}
			while(input[rhs] == Empty) {
				rhs--;
			}
			std::swap(input[lhs], input[rhs]);
			lhs++;
			rhs--;
		}
	}

	constexpr u64 SolvePart1(const std::string& input) {
		auto nums = Parse(input);
		Compact(nums);

		u64 result = 0;
		for(u64 i = 0; i < static_cast<u64>(nums.size()); i++) {
			auto val = nums[i];
			if (val == Empty) break;
			result += val * i;
		}

		return result;
	}
	
	PART(1) {
		return SolvePart1(lines[0]);
	}

	constexpr void Compact2(std::vector<u32>& input) {
		std::vector<std::pair<size_t, u32>> empties;
		size_t lhs = 0;
		while (lhs < input.size()) {
			while (lhs < input.size() && input[lhs] != Empty) {
				lhs++;
			}
			if (lhs >= input.size()) break;
			auto start = lhs;
			auto available = 0;
			while (input[lhs] == Empty) {
				available++;
				lhs++;
			}
			empties.push_back({ start, available });
		}

		size_t rhs = input.size() - 1;
		u32 currentFile = input[rhs];
		while (currentFile == Empty) {
			currentFile = input[--rhs];
		}
		while (currentFile > 0) {
			u32 required = 0;
			while (input[rhs] != currentFile) {
				rhs--;
			}
			while (input[rhs] == currentFile) {
				rhs--;
				required++;
			}
			size_t swapIndex = 0;
			bool found = false;
			for (; swapIndex < empties.size(); swapIndex++) {
				if (empties[swapIndex].first > rhs) break;
				if (empties[swapIndex].second >= required) {
					found = true;
					break;
				}
			}
			if (found)
			{
				for (size_t i = 0; i < required; i++) {
					std::swap(input[empties[swapIndex].first + i], input[rhs + i + 1]);
				}
				empties[swapIndex].second -= required;
				empties[swapIndex].first += required;
			}

			currentFile--;
		}
	}

	constexpr u64 SolvePart2(const std::string& line) {
		auto nums = Parse(line);
		Compact2(nums);
		u64 result = 0;
		for (u64 i = 0; i < static_cast<u64>(nums.size()); i++) {
			auto val = nums[i];
			if (val != Empty) {
				result += val * i;
			}
		}
		return result;
	}

	PART(2) {
		return SolvePart2(lines[0]);
	}

	static_assert(SolvePart1("2333133121414131402") == 1928);
	static_assert(SolvePart2("2333133121414131402") == 2858);
}