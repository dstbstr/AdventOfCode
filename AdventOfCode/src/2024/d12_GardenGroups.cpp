#include "Common.h"

SOLUTION(2024, 12) {
	using Region = std::vector<RowCol>;

	constexpr bool RegionsTouch(const Region & lhs, const Region & rhs) {
		
		for (auto& lhsCell : lhs) {
			if (std::any_of(rhs.begin(), rhs.end(), [&](auto& cell) {
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
						lhsRegion.insert(lhsRegion.end(), rhsRegion.begin(), rhsRegion.end());
						rhsRegion.clear();
						progress = true;
					}
				}
			}
			//erase any empty regions
			regions.erase(std::remove_if(regions.begin(), regions.end(), [](auto& region) {
				return region.empty();
				}), regions.end());
		}
	}

	constexpr std::vector<Region> ParseMap(const std::vector<std::string>& lines) {
		std::array<std::vector<Region>, 26> regionsById;
		auto limits = GetLimits<RowCol>(lines);
		Constexpr::ForEach(limits, [&](RowCol pos) {
			auto id = lines[pos.Row][pos.Col];
			if (id == '.') return;
			regionsById[id - 'A'].emplace_back(Region{ pos });
		});

		for(auto& regions : regionsById) {
			Condense(regions);
		}
		std::vector<Region> result;
		for (auto& regions : regionsById) {
			result.insert(result.end(), regions.begin(), regions.end());
		}

		return result;
	}

	constexpr u32 GetArea(const Region& region) {
		return static_cast<u32>(region.size());
	}

	constexpr std::tuple<Region, Region, Region,Region> GetPerimeterCells(const Region& region, const std::vector<std::string>& lines) {
		Region up, left, down, right;
		auto h = RowCol{ 1, 0 };
		auto v = RowCol{ 0, 1 };
		auto getId = [&](const RowCol& pos) {return lines[pos.Row][pos.Col];};
		for(const auto& cell : region) {
			auto id = getId(cell);

			if (getId(cell - v) != id) up.push_back(cell);
			if (getId(cell - h) != id) left.push_back(cell);
			if (getId(cell + h) != id) right.push_back(cell);
			if (getId(cell + v) != id) down.push_back(cell);
		}

		return { up, left, down, right };
	}

	constexpr u32 GetPerimeter(const Region& region, std::vector<std::string>& lines) {
		auto [up, left, down, right] = GetPerimeterCells(region, lines);
		return static_cast<u32>(up.size() + left.size() + down.size() + right.size());
	}

	constexpr u32 GetSides(const Region& region, std::vector<std::string>& lines) {
		auto [up, left, down, right] = GetPerimeterCells(region, lines);

		//split up, left, down, right into subregions
		std::vector<Region> upRegions, leftRegions, downRegions, rightRegions;
		auto splitIntoRegions = [](const Region& side, std::vector<Region>& outRegions) {
			std::transform(side.begin(), side.end(), std::back_inserter(outRegions), [](const RowCol& pos) {
				return Region{ {pos} };
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
		std::vector<std::string> result;
		result.reserve(lines.size() + 2);
		result.emplace_back(lines[0].size() + 2, '.');
		for (const auto& line : lines) {
			result.emplace_back('.' + line + '.');
		}
		result.emplace_back(lines[0].size() + 2, '.');
		return result;
	}

	constexpr u32 Solve(const std::vector<std::string>& lines, auto func) {
		auto map = SurroundMap(lines);
		auto regions = ParseMap(map);
		u32 result = 0;
		for (const auto& region : regions) {
			result += GetArea(region) * func(region, map);
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