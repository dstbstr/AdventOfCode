#include "Common.h"

SOLUTION(2024, 12) {
	struct Region {
		char Id;
		std::vector<RowCol> Cells;
	};

	constexpr bool RegionsTouch(const Region& lhs, const Region& rhs) {
		for (auto& lhsCell : lhs.Cells) {
			if(std::any_of(rhs.Cells.begin(), rhs.Cells.end(), [&](auto& cell) {
				return MDistance(lhsCell, cell) == 1;
			})) {
				return true;
			}
		}
		return false;
	}

	constexpr void Condense(std::vector<Region>& regions) {
		bool progress = true;
		while (progress) {
			progress = false;
			for (size_t i = 0; i < regions.size(); i++) {
				auto& lhsRegion = regions[i];
				for (size_t j = i + 1; j < regions.size(); j++) {
					auto& rhsRegion = regions[j];
					if (RegionsTouch(lhsRegion, rhsRegion)) {
						lhsRegion.Cells.insert(lhsRegion.Cells.end(), rhsRegion.Cells.begin(), rhsRegion.Cells.end());
						rhsRegion.Cells.clear();
						progress = true;
					}
				}
			}
			//erase any empty regions
			regions.erase(std::remove_if(regions.begin(), regions.end(), [](auto& region) {
				return region.Cells.empty();
				}), regions.end());
		}

	}

	using Map = std::array<std::pair<char, std::vector<Region>>, 26>;
	constexpr Map ParseMap(const std::vector<std::string>& lines) {
		Map result{};
		auto limits = GetLimits<RowCol>(lines);
		Constexpr::ForEach(limits, [&](RowCol pos) {
			auto id = lines[pos.Row][pos.Col];
			if (id == '.') return;
			result[id - 'A'].second.emplace_back(Region{id, {pos}});
		});

		// combine regions with the same Id and at least 1 adjacent cell
		for(auto& [id, regions] : result) {
			Condense(regions);
		}

		return result;
	}

	constexpr u32 GetArea(const Region& region) {
		return static_cast<u32>(region.Cells.size());
	}

	constexpr std::tuple<
		std::vector<RowCol>, 
		std::vector<RowCol>, 
		std::vector<RowCol>,
		std::vector<RowCol>>
		GetPerimeterCells(const Region& region, const std::vector<std::string>& lines) {
		std::vector<RowCol> up, left, down, right;
		auto h = RowCol{ 1, 0 };
		auto v = RowCol{ 0, 1 };
		for(const auto& cell : region.Cells) {
			auto leftCell = cell - h;
			auto rightCell = cell + h;
			auto upCell = cell - v;
			auto downCell = cell + v;

			if (lines[leftCell.Row][leftCell.Col] != region.Id) {
				left.push_back(cell);
			}
			if (lines[rightCell.Row][rightCell.Col] != region.Id) {
				right.push_back(cell);
			}
			if (lines[upCell.Row][upCell.Col] != region.Id) {
				up.push_back(cell);
			}
			if (lines[downCell.Row][downCell.Col] != region.Id) {
				down.push_back(cell);
			}
		}

		return { up, left, down, right };
	}

	constexpr u32 GetPerimeter(const Region& region, std::vector<std::string>& lines) {
		auto [up, left, down, right] = GetPerimeterCells(region, lines);
		return static_cast<u32>(up.size() + left.size() + down.size() + right.size());
	}

	constexpr u32 GetSides(const Region& region, std::vector<std::string>& lines) {
		// find all left, right, top, bottom sides
		// the left sides are the cells which have no left neighbor
		auto [up, left, down, right] = GetPerimeterCells(region, lines);

		//split up, left, down, right into subregions
		std::vector<Region> upRegions, leftRegions, downRegions, rightRegions;
		auto splitIntoRegions = [](const std::vector<RowCol>& side, std::vector<Region>& outRegions) {
			std::transform(side.begin(), side.end(), std::back_inserter(outRegions), [](const RowCol& pos) {
				return Region{ 'U', {pos} };
				});
			};

		splitIntoRegions(up, upRegions);
		splitIntoRegions(left, leftRegions);
		splitIntoRegions(down, downRegions);
		splitIntoRegions(right, rightRegions);

		Condense(upRegions);
		Condense(leftRegions);
		Condense(downRegions);
		Condense(rightRegions);

		return static_cast<u32>(upRegions.size() + leftRegions.size() + downRegions.size() + rightRegions.size());
	}

	constexpr std::vector<std::string> SurroundMap(const std::vector<std::string>& lines) {
		auto copy = lines;
		copy.insert(copy.begin(), std::string(copy[0].size(), '.'));
		copy.push_back(std::string(copy[0].size(), '.'));
		for (auto& line : copy) {
			line.insert(line.begin(), '.');
			line.push_back('.');
		}
		return copy;
	}

	constexpr u32 Solve(const std::vector<std::string>& lines, auto func) {
		auto copy = SurroundMap(lines);
		auto map = ParseMap(copy);
		u32 result = 0;
		for (const auto& [id, regions] : map) {
			for (const auto& region : regions) {
				result += GetArea(region) * func(region, copy);
			}
		}

		return result;

	}

	PART(1) {
		return Solve(lines, GetPerimeter);
	}

	PART(2) {
		return Solve(lines, GetSides);
	}
	TEST(1) {
		std::vector<std::string> lines = {
			"AAAA",
			"BBCD",
			"BBCC",
			"EEEC"
		};

		return Solve(lines, GetPerimeter) == 140 &&
			Solve(lines, GetSides) == 80;
	}

	TEST(2) {
		std::vector<std::string> lines = {
			"RRRRIICCFF",
			"RRRRIICCCF",
			"VVRRRCCFFF",
			"VVRCCCJFFF",
			"VVVVCJJCFE",
			"VVIVCCJJEE",
			"VVIIICJJEE",
			"MIIIIIJJEE",
			"MIIISIJEEE",
			"MMMISSJEEE"
		};

		return Solve(lines, GetPerimeter) == 1930 &&
			Solve(lines, GetSides) == 1206;
	}
}