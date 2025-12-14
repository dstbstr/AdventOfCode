#include "Common.h"

SOLUTION(2025, 12) {
	using Gift = std::array<std::array<bool, 3>, 3>;
	struct Grid {
		size_t Width;
		size_t Height;
		std::array<size_t, 6> GiftCounts;
	};

	constexpr std::vector<Gift> ParseGifts(const std::vector<std::vector<std::string>>& groups) {
		std::vector<Gift> gifts;
		for (const auto& group : groups) {
			Gift gift{};
			for (size_t i = 0; i < 3; i++) {
				for (size_t j = 0; j < 3; j++) {
					gift[i][j] = (group[i][j] == '#');
				}
			}
			gifts.push_back(gift);
		}
		return gifts;
	}

	constexpr std::vector<Grid> ParseGrids(const std::vector<std::string>& lines) {
		std::vector<Grid> grids;
		for(const auto& line : lines) {
			auto s1 = Constexpr::Split(line, ": ");
			auto wh = Constexpr::Split(s1[0], "x");
			auto s2 = Constexpr::Split(s1[1], " ");

			Grid grid{};
			Constexpr::ParseNumber(wh[0], grid.Width);
			Constexpr::ParseNumber(wh[1], grid.Height);
			for (size_t i = 0; i < 6; i++) {
				Constexpr::ParseNumber(s2[i], grid.GiftCounts[i]);
			}
			grids.push_back(grid);
		}

		return grids;
	}
	
	PART(1) {
		auto groups = SplitInputIntoGroups(lines);
		//std::vector<std::vector<std::string>> giftGroups;
		//std::copy_n(groups.begin(), groups.size() - 1, std::back_inserter(giftGroups));
		//auto gifts = ParseGifts(giftGroups);
		auto grids = ParseGrids(groups.back());

		size_t result = 0;
		for(const auto& grid : grids) {
			auto totalSize = grid.Width * grid.Height;
			auto neededSize = std::accumulate(grid.GiftCounts.begin(), grid.GiftCounts.end(), 0ull);
			neededSize *= 9;
			result += neededSize <= totalSize;
		}

		return result;
	}
}