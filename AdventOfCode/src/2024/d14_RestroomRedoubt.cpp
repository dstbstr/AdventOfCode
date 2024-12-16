#include "Common.h"

SOLUTION(2024, 14) {
	struct Bot {
		Coord Pos;
		Coord Vel;
	};

	constexpr Bot ParseBot(std::string_view line) {
		auto parts = Constexpr::Split(line, " ");
		return { 
			Coord(Constexpr::Split(parts[0], "=")[1], ","), 
			Coord(Constexpr::Split(parts[1], "=")[1], ",") 
		};
	}

	static_assert(ParseBot("p=0,4 v=3,-3").Pos == Coord{0, 4});
	static_assert(ParseBot("p=0,4 v=3,-3").Vel== Coord{ 3, -3 });

	constexpr void PrintMap(const std::vector<u32>& grid, Coord Limits) {
		std::string log = "";
		for (auto i = 0; i <= Limits.Y; i++) {
			for (auto j = 0; j <= Limits.X; j++) {
				auto index = i * (Limits.X + 1) + j;
				log.push_back(grid[index] == 0 ? '.' : '0' + static_cast<u8>(grid[index]));
			}
			log += '\n';
		}
		GET_LOGS().emplace_back(log);
	}

	template<s64 Rows, s64 Cols>
	constexpr u32 Solve(const std::vector<std::string>& lines) {
		auto seconds = 100ll;
		auto bots = ParseLines(lines, ParseBot);

		std::vector<u32> grid;
		grid.resize((Rows + 1) * (Cols + 1));
		std::vector<u32> initialGrid;
		initialGrid.resize((Rows + 1) * (Cols + 1));

		auto GetIndex = [](Coord pos) {
			return pos.Y * (Cols + 1) + pos.X;
		};
		for (auto& bot : bots) {
			auto index = GetIndex(bot.Pos);
			initialGrid[index]++;
			bot.Pos += (bot.Vel * seconds);
			bot.Pos.X %= Cols;
			bot.Pos.Y %= Rows;
			bot.Pos.X = (bot.Pos.X + Cols) % Cols;
			bot.Pos.Y = (bot.Pos.Y + Rows) % Rows;
			index = GetIndex(bot.Pos);
			grid[index]++;
		}

		u32 q1 = 0; u32 q2 = 0; u32 q3 = 0; u32 q4 = 0;
		for (auto i = 0; i < Cols / 2; i++) {
			for (auto j = 0; j < Rows / 2; j++) {
				auto index = GetIndex(Coord{ i, j });
				q1 += grid[index];
			}
			for (auto j = (Rows / 2 + 1); j <= Rows; j++) {
				auto index = GetIndex(Coord{ i, j });
				q2 += grid[index];
			}
		}
		for (auto i = (Cols / 2) + 1; i <= Cols; i++) {
			for (auto j = 0; j < Rows / 2; j++) {
				auto index = GetIndex(Coord{ i, j });
				q3 += grid[index];
			}
			for (auto j = (Rows / 2) + 1; j <= Rows; j++) {
				auto index = GetIndex(Coord{ i, j });
				q4 += grid[index];
			}
		}

		return q1 * q2 * q3 * q4;
	}

	PART(1) {
		return Solve<103, 101>(lines);
	}

	template<s64 Rows, s64 Cols>
	constexpr size_t SolvePart2(const std::vector<std::string>& lines) {
		auto bots = ParseLines(lines, ParseBot);
		auto GuessDensity = [&]() {
			s64 density = 0;
			for (auto i = 0; i < 100; i++) {
				for (auto j = i; j < 100; j++) {
					density += MDistance(bots[i].Pos, bots[j].Pos);
				}
			}
			return density;
		};

		auto PrintMap = [&]() {
			std::string log = "";
			for (auto i = 0; i <= Rows; i++) {
				for (auto j = 0; j <= Cols; j++) {
					auto bot = std::find_if(bots.begin(), bots.end(), [i, j](const auto& bot) { return bot.Pos == Coord{ j, i }; });
					log.push_back(bot != bots.end() ? 'X' : '.');
				}
				log += '\n';
			}
			GET_LOGS().emplace_back(log);
			};
		(void)PrintMap;

		auto MoveBot = [](Bot& bot, size_t steps = 1) {
			bot.Pos += bot.Vel * steps;
			bot.Pos.X %= Cols;
			bot.Pos.Y %= Rows;
			bot.Pos.X = (bot.Pos.X + Cols) % Cols;
			bot.Pos.Y = (bot.Pos.Y + Rows) % Rows;
			};
		auto baseGuess = 7000;
		for(auto& bot : bots) {
			MoveBot(bot, baseGuess);
		}
		auto expected = 200000;
		for(auto i = 0; i < 500; i++) {
			for (auto& bot : bots) {
				MoveBot(bot);
			}
			
			if (GuessDensity() < expected) {
				//GET_LOGS().emplace_back(std::format("Second: {}, Density: {}", i, density));
				//PrintMap();
				return i + baseGuess + 1;
			}
		}
		return 0;
	}

	PART(2) {
		return SolvePart2<103, 101>(lines);
	}
	TEST(1) {
		std::vector<std::string> lines = {
			"p=0,4 v=3,-3",
			"p=6,3 v=-1,-3",
			"p=10,3 v=-1,2",
			"p=2,0 v=2,-1",
			"p=0,0 v=1,3",
			"p=3,0 v=-2,-2",
			"p=7,6 v=-1,-3",
			"p=3,0 v=-1,-2",
			"p=9,3 v=2,3",
			"p=7,3 v=-1,2",
			"p=2,4 v=2,-3",
			"p=9,5 v=-3,-3"
		};

		return Solve<7, 11>(lines) == 12;
	}
}