#include "Common.h"
#include "Facing.h"
#include "Core/Algorithms/AStar.h"

namespace Year2024Day16 {
	struct State {
		RowCol Pos{};
		Facing Dir{};
		u64 Cost{ 0 };

		constexpr bool operator==(const State& other) const {
			return Pos == other.Pos && Dir == other.Dir;
		}
		constexpr bool operator<(const State& other) const {
			return Cost < other.Cost;
		}
	};
}
namespace Constexpr {
	template<>
	struct Hasher<Year2024Day16::State> {
		constexpr size_t operator()(const Year2024Day16::State& state) const {
			return Hasher<RowCol>()(state.Pos);
		}
	};
}
SOLUTION(2024, 16) {
	constexpr std::pair<RowCol, RowCol> FindStartEnd(const std::vector<std::string>& lines) {
		return { {lines.size() - 2, 1}, {1, lines[0].size() - 2} };
	}

	struct Path {
		std::vector<RowCol> Positions;
		u64 Cost{ 0 };
	};

	constexpr std::vector<State> GetNextSteps(const std::vector<std::string>& lines, const State& state) {
		auto forward = state.Pos;
		auto left = state.Pos;
		auto right = state.Pos;
		Move(forward, state.Dir);
		Move(left, TurnLeft(state.Dir));
		Move(right, TurnRight(state.Dir));

		auto nextC = [&](const RowCol& pos) { return lines[pos.Row][pos.Col]; };
		auto forwardC = nextC(forward);
		auto leftC = nextC(left);
		auto rightC = nextC(right);

		std::vector<State> result;
		if (forwardC != '#') result.push_back({ forward, state.Dir, state.Cost + 1 });
		if (leftC != '#') result.push_back({ left, TurnLeft(state.Dir), state.Cost + 1001 });
		if (rightC != '#') result.push_back({ right, TurnRight(state.Dir), state.Cost + 1001 });

		return result;
	}

	constexpr std::vector<State> FindShortestPath(const std::vector<std::string>& map, State start, RowCol end) {
		auto costFunc = [](const State& from, const State& to) -> size_t { return from.Dir == to.Dir ? 1 : 1001; };
		auto doneFunc = [](const State& pos, const State& end) { return pos.Pos == end.Pos; };
		auto hFunc = [](const State& pos, const State& end) { return pos.Cost + MDistance(pos.Pos, end.Pos); };
		AStarParameters<State, std::vector<std::string>> params{
			.map = map,
			.start = start,
			.end = State{end, Facing::Right, 0 },
			.costFunc = costFunc,
			.doneFunc = doneFunc,
			.hFunc = hFunc,
			.nFunc = GetNextSteps
		};

		return AStarMin<100'000>(params).value_or(std::vector<State>{});
	}

	PART(1) {
		auto [start, end] = FindStartEnd(lines);
		State state{ start, Facing::Right, 0 };
		auto path = FindShortestPath(lines, state, end);
		return path.back().Cost;
	}

	constexpr size_t SolvePart2(const std::vector<std::string>& lines) {
		auto [start, end] = FindStartEnd(lines);
		auto bestPath = FindShortestPath(lines, State{ start, Facing::Right, 0 }, end);

		Constexpr::BigSet<RowCol, 5'000> unique;
		for (const auto& state : bestPath) {
			unique.insert(state.Pos);
		}
		auto bestCost = bestPath.back().Cost;

		for (const auto& state : bestPath) {
			auto steps = GetNextSteps(lines, state);
			for (const auto& step : steps) {
				if (!unique.contains(step.Pos)) {
					auto path = FindShortestPath(lines, step, end);
					if (!path.empty() && path.back().Cost == bestCost) {
						for (const auto& state2 : path) {
							unique.insert(state2.Pos);
						}
					}
				}
			}
		}

		return unique.size();
	}

	PART(2) {
		return SolvePart2(lines);
	}

	TEST(1) {
		std::vector<std::string> lines = {
			"###############",
			"#.......#....E#",
			"#.#.###.#.###.#",
			"#.....#.#...#.#",
			"#.###.#####.#.#",
			"#.#.#.......#.#",
			"#.#.#####.###.#",
			"#...........#.#",
			"###.#.#####.#.#",
			"#...#.....#.#.#",
			"#.#.#.###.#.#.#",
			"#.....#...#.#.#",
			"#.###.#.#.#.#.#",
			"#S..#.....#...#",
			"###############"
		};

		return std::get<u64>(Part1(lines)) == 7036 &&
		SolvePart2(lines) == 45;
	}

	TEST(2) {
		std::vector<std::string> lines = {
			"#################",
			"#...#...#...#..E#",
			"#.#.#.#.#.#.#.#.#",
			"#.#.#.#...#...#.#",
			"#.#.#.#.###.#.#.#",
			"#...#.#.#.....#.#",
			"#.#.#.#.#.#####.#",
			"#.#...#.#.#.....#",
			"#.#.#####.#.###.#",
			"#.#.#.......#...#",
			"#.#.###.#####.###",
			"#.#.#...#.....#.#",
			"#.#.#.#####.###.#",
			"#.#.#.........#.#",
			"#.#.#.#########.#",
			"#S#.............#",
			"#################"
		};

		return std::get<u64>(Part1(lines)) == 11048 && SolvePart2(lines) == 64;
	}
}