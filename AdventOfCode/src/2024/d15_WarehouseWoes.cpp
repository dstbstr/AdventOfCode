#include "Common.h"
#include "Facing.h"

SOLUTION(2024, 15) {
	using Grid = std::vector<std::string>;
	constexpr Coord Vert{ 0, 1 };
	constexpr Coord Horz{ 1, 0 };

	constexpr u32 GetGps(RowCol pos) {
		return static_cast<u32>((pos.Row * 100) + pos.Col);
	}

	constexpr u32 GetAllGps(const Grid& grid) {
		u32 sum = 0;
		for (size_t row = 1u; row < grid.size() - 1; row++) {
			for (size_t col = 1u; col < grid[row].size() - 1; col++) {
				if (grid[row][col] == 'O' || grid[row][col] == '[') {
					sum += GetGps({ row, col });
				}
			}
		}
		return sum;
	}

	constexpr bool CanMoveVertical(const Grid& grid, Coord pos, Facing dir) {
		auto next = pos;
		if (dir == Facing::Up) {
			next -= Vert;
		}
		else {
			next += Vert;
		}
		auto c = grid[next.Y][next.X];
		if (c == '.') return true;
		if (c == '#') return false;
		if (c == 'O') return CanMoveVertical(grid, next, dir);
		if (c == '[') return CanMoveVertical(grid, next, dir) && CanMoveVertical(grid, next + Horz, dir);
		if (c == ']') return CanMoveVertical(grid, next, dir) && CanMoveVertical(grid, next - Horz, dir);
		throw "Wat?";
	}

	constexpr bool CanMove(const Grid& grid, Coord pos, Facing dir) {
		switch (dir) {
		case Facing::Up: return CanMoveVertical(grid, pos, dir);
		break; case Facing::Down: return CanMoveVertical(grid, pos, dir);
		break; case Facing::Left: {
			auto nextSpace = grid[pos.Y].rfind('.', pos.X);
			auto nextWall = grid[pos.Y].rfind('#', pos.X);
			return nextSpace != std::string::npos && nextWall < nextSpace;
		}
		break; case Facing::Right: {
			auto nextSpace = grid[pos.Y].find('.', pos.X);
			auto nextWall = grid[pos.Y].find('#', pos.X);
			return nextSpace != std::string::npos && nextWall > nextSpace;
		}
		}
		throw "Wat?";
	}

	constexpr void Move2(Grid& grid, Coord start, Facing dir) {
		auto next = start;
		if(dir == Facing::Left || dir == Facing::Right) {
			next.X += dir == Facing::Left ? -1 : 1;
			if(grid[next.Y][next.X] != '.') {
				Move2(grid, next, dir);
			}
		} else {
			next.Y += dir == Facing::Up ? -1 : 1;
			auto c = grid[next.Y][next.X];
			if(c == '[') {
				auto next2 = next + Horz;
				Move2(grid, next, dir);
				Move2(grid, next2, dir);
			} else if(c == ']') {
				auto next2 = next - Horz;
				Move2(grid, next, dir);
				Move2(grid, next2, dir);
			} else if(c == 'O') {
				Move2(grid, next, dir);
			}
		}

		std::swap(grid[start.Y][start.X], grid[next.Y][next.X]);
	}

	constexpr u32 Solve(Grid& grid, const std::string& moves) {
		Coord bot;

		for (size_t row = 1u; row < grid.size() - 1; row++) {
			for (size_t col = 1u; col < grid[row].size() - 1; col++) {
				if (grid[row][col] == '@') {
					bot = { static_cast<s64>(col), static_cast<s64>(row) };
					break;
				}
			}
		}

		for (auto c : moves) {
			auto dir = ParseFacing(c);
			if(CanMove(grid, bot, dir)) {
				Move2(grid, bot, dir);
				Move(bot, dir);
			}
		}

		return GetAllGps(grid);
	}

	PART(1) {
		auto groups = SplitInputIntoGroups(lines);
		auto grid = groups[0];
		auto moves = Constexpr::JoinVec("", groups[1]);
		return Solve(grid, moves);
	}

	constexpr Grid ExpandGrid(const Grid& input) {
		Grid result;
		result.reserve(input.size());
		auto newShape = [](char c) {
			switch (c) {
			case '#': return "##";
			case 'O': return "[]";
			case '.': return "..";
			case '@': return "@.";
			default: throw "Wat?";
			}};

		for (auto& line : input) {
			std::string newLine;
			newLine.reserve(line.size() * 2);
			for (auto c : line) {
				newLine += newShape(c);
			}
			result.emplace_back(newLine);
		}
		return result;
	}

	PART(2) {
		auto groups = SplitInputIntoGroups(lines);
		auto grid = ExpandGrid(groups[0]);
		auto moves = Constexpr::JoinVec("", groups[1]);
		return Solve(grid, moves);
	}
	
	TEST(1) {
		std::vector<std::string> lines = {
			"########",
			"#..O.O.#",
			"##@.O..#",
			"#...O..#",
			"#.#.O..#",
			"#...O..#",
			"#......#",
			"########",
			"",
			"<^^>>>vv<v>>v<<"
		};

		auto groups = SplitInputIntoGroups(lines);
		auto grid = groups[0];
		auto moves = Constexpr::JoinVec("", groups[1]);
		return Solve(grid, moves) == 2028;
	}

	TEST(2) {
		std::vector<std::string> lines = {
			"##########",
			"#..O..O.O#",
			"#......O.#",
			"#.OO..O.O#",
			"#..O@..O.#",
			"#O#..O...#",
			"#O..O..O.#",
			"#.OO.O.OO#",
			"#....O...#",
			"##########",
			"",
			"<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^",
			"vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v",
			"><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<",
			"<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^",
			"^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><",
			"^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^",
			">^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^",
			"<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>",
			"^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>",
			"v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^"
		};
		auto groups = SplitInputIntoGroups(lines);
		auto grid = groups[0];
		auto grid2 = ExpandGrid(grid);
		auto moves = Constexpr::JoinVec("", groups[1]);

		return Solve(grid, moves) == 10092 && Solve(grid2, moves) == 9021;
	}
}