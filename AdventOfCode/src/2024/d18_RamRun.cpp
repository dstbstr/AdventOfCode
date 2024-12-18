#include "Common.h"
#include "Core/Algorithms/AStar.h"

SOLUTION(2024, 18) {
	using PosType = Vec2<u8>;
	using Grid = Constexpr::BigSet<PosType, 5'000>;
	constexpr PosType Begin{ 0, 0 };

	constexpr Grid ParseGrid(const std::vector<std::string>& lines, size_t bytes) {
		Grid result;
		for (size_t i = 0u; i < bytes; i++) {
			PosType pos;
			auto s = Constexpr::Split(lines[i], ",");
			Constexpr::ParseNumber(s[0], pos.X);
			Constexpr::ParseNumber(s[1], pos.Y);
			result.insert(pos);
		}
		return result;
	}

	template<size_t Dim>
	constexpr std::vector<PosType> GetNeighbors(const Grid& grid, const PosType& pos) {
		return GetDirectNeighbors(pos, { Dim, Dim })
			| std::views::filter([&](const PosType pos) { return !grid.contains(pos); })
			| std::ranges::to<std::vector>();
	}

	template<size_t Dim>
	constexpr std::optional<std::vector<PosType>> FindPath(const std::vector<std::string>& lines, size_t bytes) {
		AStarParameters<PosType, Grid> params{
			.map = ParseGrid(lines, bytes),
			.start = Begin,
			.end = {Dim, Dim},
			.nFunc = GetNeighbors<Dim>
		};
		return AStarMin<5'000>(params);
	}

	// binary search for first coordinate that fails
	template<size_t Dim>
	constexpr PosType SolvePart2(const std::vector<std::string>& lines) {
		size_t min = 0;
		size_t max = lines.size();
		while(true) {
			auto mid = (max + min) / 2;
			auto path = FindPath<Dim>(lines, mid);
			if(path.has_value()) {
				min = mid;
			} else {
				max = mid;
			}
			if(max - min == 1) {
				PosType result;
				auto split = Constexpr::Split(lines[min], ",");
				Constexpr::ParseNumber(split[0], result.X);
				Constexpr::ParseNumber(split[1], result.Y);
				return result;
			}
		}
	}

	PART(1) {
		return FindPath<70>(lines, 1024).value().size() - 1;
	}

	PART(2) {
		return Constexpr::ToString(SolvePart2<70>(lines));
	}

	constexpr std::vector<std::string> GetTestInput() {
		return {
		"5,4",
		"4,2",
		"4,5",
		"3,0",
		"2,1",
		"6,3",
		"2,4",
		"1,5",
		"0,6",
		"3,3",
		"2,6",
		"5,1",
		"1,2",
		"5,5",
		"2,5",
		"6,5",
		"1,4",
		"0,4",
		"6,4",
		"1,1",
		"6,1",
		"1,0",
		"0,5",
		"1,6",
		"2,0"
		};

	}
	TEST(1) {
		auto path = FindPath<6>(GetTestInput(), 12);
		return path.has_value() && path.value().size() == 23;
	}

	TEST(2) {
		return SolvePart2<6>(GetTestInput()) == PosType{ 6,1 };
	}
}