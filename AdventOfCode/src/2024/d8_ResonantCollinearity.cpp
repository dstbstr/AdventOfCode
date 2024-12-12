#include "Common.h"

SOLUTION(2024, 8) {
	using Map = std::array<std::vector<Coord>, 128 - '0'>;

	constexpr Map ParseLines(const std::vector<std::string>& lines) {
		Map result{};
		auto limits = GetLimits<Coord>(lines);
		Constexpr::ForEach(Origin<Coord>, limits, [&](Coord pos) {
			auto c = lines[pos.Y][pos.X];
			if (c != '.') {
				result[c - '0'].push_back(pos);
			}
			});
		return result;
	}

	template<bool IsPart2>
	constexpr size_t Solve(const std::vector<std::string>& lines) {
		auto map = ParseLines(lines);
		Constexpr::BigSet<Coord> antinodes;
		auto limits = GetLimits<Coord>(lines);

		for (const auto& coords : map) {
			for (size_t i = 0; i < coords.size(); i++) {
				for (size_t j = i + 1; j < coords.size(); j++) {
					auto a = coords[i];
					auto b = coords[j];
					auto dx = a.X - b.X;
					auto dy = a.Y - b.Y;
					if constexpr(IsPart2) {
						antinodes.insert(a);
						antinodes.insert(b);
					}

					auto move = Coord{ dx, dy };
					auto first = a + move;
					auto second = b - move;
					while (first.Between(Origin<Coord>, limits)) {
						antinodes.insert(first);
						if constexpr (!IsPart2) break;
						else first += move;
					}
					while (second.Between(Origin<Coord>, limits)) {
						antinodes.insert(second);
						if constexpr (!IsPart2) break;
						else second -= move;
					}
				}
			}
		}

		return antinodes.size();
	}

	PART(1) {
		return Solve<false>(lines);
	}

	PART(2) {
		return Solve<true>(lines);
	}

	TEST(1) {
		std::vector<std::string> lines = {
			"............",
			"........0...",
			".....0......",
			".......0....",
			"....0.......",
			"......A.....",
			"............",
			"............",
			"........A...",
			".........A..",
			"............",
			"............"
		};

		return Solve<false>(lines) == 14;
	}

	TEST(2) {
		std::vector<std::string> lines = {
			"............",
			"........0...",
			".....0......",
			".......0....",
			"....0.......",
			"......A.....",
			"............",
			"............",
			"........A...",
			".........A..",
			"............",
			"............"
		};

		return Solve<true>(lines) == 34;
	}
}