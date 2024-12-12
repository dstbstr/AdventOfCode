#include "Common.h"

#include "Facing.h"
#include <ranges>

SOLUTION(2024, 6) {
	using Map = Constexpr::BigSet<RowCol, 100000>;

	constexpr RowCol FindGuardStart(const std::vector<std::string>&lines) {
		for (size_t row = 0; row < lines.size(); row++) {
			for (size_t col = 0; col < lines[0].size(); col++) {
				auto c = lines[row][col];
				if (c == '^') {
					return { row, col };
				}
			}
		}

		throw "Wat?";
	}

	template<size_t N>
	using Obstructions = std::array<std::vector<size_t>, N>;

	template<size_t N>
	constexpr std::pair<Obstructions<N>, Obstructions<N>> FindObstructions(const std::vector<std::string>& lines) {
		Obstructions<N> rows{};
		Obstructions<N> cols{};
		for (size_t row = 0; row < lines.size(); row++) {
			for (size_t col = 0; col < lines[0].size(); col++) {
				auto c = lines[row][col];
				if (c == '#') {
					rows[row].emplace_back(col);
					cols[col].emplace_back(row);
				}
			}
		}

		return { rows, cols };
	}

	template<size_t N>
	constexpr std::optional<RowCol> Up(const std::pair<Obstructions<N>, Obstructions<N>>& obstructions, RowCol pos) {
		auto& currentCol = obstructions.second[pos.Col];
		auto f = currentCol | std::views::filter([pos](size_t v) { return v < pos.Row; });
		if (f.empty()) return std::nullopt;
		return RowCol{ *std::ranges::max_element(f) + 1, pos.Col };
	}
	template<size_t N>
	constexpr std::optional<RowCol> Right(const std::pair<Obstructions<N>, Obstructions<N>>& obstructions, RowCol pos) {
		auto& currentRow = obstructions.first[pos.Row];
		auto f = currentRow | std::views::filter([pos](size_t v) { return v > pos.Col; });
		if (f.empty()) return std::nullopt;
		return RowCol{ pos.Row, *std::ranges::min_element(f) - 1 };
	}
	template<size_t N>
	constexpr std::optional<RowCol> Down (const std::pair<Obstructions<N>, Obstructions<N>>& obstructions, RowCol pos) {
		auto& currentCol = obstructions.second[pos.Col];
		auto f = currentCol | std::views::filter([pos](size_t v) { return v > pos.Row; });
		if (f.empty()) return std::nullopt;
		return RowCol{ *std::ranges::min_element(f) - 1, pos.Col };
	}
	template<size_t N>
	constexpr std::optional<RowCol> Left(const std::pair<Obstructions<N>, Obstructions<N>>& obstructions, RowCol pos) {
		auto& currentRow = obstructions.first[pos.Row];
		auto f = currentRow | std::views::filter([pos](size_t v) { return v < pos.Col; });
		if (f.empty()) return std::nullopt;
		return RowCol{ pos.Row, *std::ranges::max_element(f) + 1 };
	}

	template<size_t N>
	constexpr std::optional<RowCol> Next(const std::pair<Obstructions<N>, Obstructions<N>>& obstructions, RowCol pos, Facing dir) {
		switch (dir) {
		case Facing::Up: return Up(obstructions, pos);
		case Facing::Right: return Right(obstructions, pos);
		case Facing::Down: return Down(obstructions, pos);
		case Facing::Left: return Left(obstructions, pos);
		}
		return std::nullopt;
	}

	template<size_t N>
	constexpr std::vector<RowCol> CreatePath(const std::pair<Obstructions<N>, Obstructions<N>>& obstructions, RowCol start) {
		std::vector<RowCol> result;
		auto current = start;
		auto dir = Facing::Up;
		while(true) {
			auto next = Next(obstructions, current, dir);
			if(next.has_value()) {
				result.emplace_back(*next);
				current = *next;
				dir = TurnRight(dir);
			}
			else break;
		}

		return result;
	}

	template<size_t N>
	constexpr std::vector<std::pair<RowCol, Facing>> CreatePath2(const std::pair<Obstructions<N>, Obstructions<N>>& obstructions, RowCol start) {
		std::vector<std::pair<RowCol, Facing>> result;
		auto current = start;
		auto dir = Facing::Up;
		while (true) {
			auto next = Next(obstructions, current, dir);
			if (next.has_value()) {
				current = *next;
				dir = TurnRight(dir);
				result.emplace_back(*next, dir);
			}
			else break;
		}
		return result;
	}

	constexpr void FillPath(RowCol start, RowCol end, Map& map) {
		if(start.Row == end.Row) {
			for (auto i = std::min(start.Col, end.Col); i <= std::max(start.Col, end.Col); i++) {
				map.insert({ start.Row, i });
			}
		} else {
			for (auto i = std::min(start.Row, end.Row); i <= std::max(start.Row, end.Row); i++) {
				map.insert({ i, start.Col });
			}
		}
	}

	template<size_t N>
	constexpr Map FindGuardPath(const std::vector<std::string>& lines) {
		RowCol limits = { lines.size() - 1, lines[0].size() - 1 };
		auto start = FindGuardStart(lines);
		auto obstructions = FindObstructions<N>(lines);
		auto path = CreatePath<N>(obstructions, start);
		Map result;
		FillPath(start, path[0], result);
		for (size_t i = 0; i < path.size() - 1; i++) {
			FillPath(path[i], path[i + 1], result);
		}
		
		auto dir = Facing::Up;
		switch(path.size() % 4) {
		case 0: break;
		case 1: dir = Facing::Right; break;
		case 2: dir = Facing::Down; break;
		case 3: dir = Facing::Left; break;
		}
		
		auto last = path.back();
		switch(dir) {
			case Facing::Up: FillPath(last, { 0, last.Col }, result); break;
			case Facing::Right: FillPath(last, { last.Row, limits.Col }, result); break;
			case Facing::Down: FillPath(last, { limits.Row, last.Col }, result); break;
			case Facing::Left: FillPath(last, { last.Row, 0 }, result); break;
		}

		return result;
	}

	PART(1) {
		return FindGuardPath<130>(lines).size();
	}

	template<size_t N>
	constexpr bool HasLoop(const std::pair<Obstructions<N>, Obstructions<N>>& obstructions, RowCol start) {
		Constexpr::BigSet<RowCol, 10000> visited;
		auto dir = Facing::Up;
		auto current = start;
		while(true) {
			if (visited.contains(current)) return true;
			visited.insert(current);

			auto next = current;
			while(next == current) {
				auto n = Next(obstructions, current, dir);
				if (!n.has_value()) return false;
				next = *n;
				dir = TurnRight(dir);
			}
			current = next;
		}
	}

	template<size_t N>
	constexpr bool HasLoop2(const std::pair<Obstructions<N>, Obstructions<N>>& obstructions, RowCol start, Facing dir) {
		Constexpr::BigSet<RowCol, 10000> visited;
		auto current = start;
		while (true) {
			if (visited.contains(current)) return true;
			visited.insert(current);

			auto next = current;
			while (next == current) {
				auto n = Next(obstructions, current, dir);
				if (!n.has_value()) return false;
				next = *n;
				dir = TurnRight(dir);
			}
			current = next;
		}
	}

	template<size_t N>
	constexpr size_t BruteForce(const std::vector<std::string>& lines) {
		auto start = FindGuardStart(lines);
		auto obstructions = FindObstructions<N>(lines);
		auto& [rows, cols] = obstructions;
		size_t result = 0;
		for(size_t row = 0; row < N; row++) {
			for(size_t col = 0u; col < N; col++) {
				auto c = lines[row][col];
				if (c == '#' || c == '^') continue;
				rows[row].emplace_back(col);
				cols[col].emplace_back(row);
				result += HasLoop<N>(obstructions, start);
				rows[row].pop_back();
				cols[col].pop_back();
			}
		}

		return result;
	}

	// don't like the brute force, but that seems to be the most common approach
	// in theory, we could follow the path, and check for early turns which lead
	// to the existing path, but there may be edge cases (especially with corners)
	// Maybe the path could be a position and direction, and we can check for 
	// loops by turning early, and seeing if the pos/dir exists
	PART(2) {
		return BruteForce<130>(lines);
	}

	TEST(1) {
		std::vector<std::string> lines = {
			"....#.....",
			".........#",
			"..........",
			"..#.......",
			".......#..",
			"..........",
			".#..^.....",
			"........#.",
			"#.........",
			"......#..."
		};

		auto map = FindGuardPath<10>(lines);
		return map.size() == 41;
	}

	TEST(2) {
		std::vector<std::string> lines = {
			"....#.....",
			".........#",
			"..........",
			"..#.......",
			".......#..",
			"..........",
			".#..^.....",
			"........#.",
			"#.........",
			"......#..."
		};

		//{6,3}
		//{7,6}
		//{7,7}
		//{8,1}
		//{8,3}
		//{9,7}
		return BruteForce<10>(lines) == 6;
	}
}