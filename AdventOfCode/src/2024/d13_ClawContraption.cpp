#include "Common.h"

SOLUTION(2024, 13) {
	struct Game {
		Coord A, B, Prize;
	};

	constexpr Coord ParseLine(const std::string& line) {
		auto s1 = Constexpr::Split(line, ": ");
		auto s2 = Constexpr::Split(s1[1], ", ");
		auto x = s2[0].substr(2);
		auto y = s2[1].substr(2);
		Coord result;
		Constexpr::ParseNumber(x, result.X);
		Constexpr::ParseNumber(y, result.Y);
		return result;
	}

	static_assert(ParseLine("Button A: X+94, Y+34") == Coord{ 94, 34 });
	static_assert(ParseLine("Prize: X=8400, Y=5400") == Coord{8400, 5400});

	constexpr Game ParseGame(const std::vector<std::string>& lines, s64 extra) {
		return Game {
			.A = ParseLine(lines[0]),
			.B = ParseLine(lines[1]),
			.Prize = ParseLine(lines[2]) + Coord{extra, extra}
		};
	}

	//A costs 3, B costs 1
	constexpr u64 SolveGame(const Game& game) {
		/*
		Ax*a + Bx*b = Px
		Ay*a + By*b = Py

		Ax*a = Px - Bx*b
		a = (Px - Bx*b)/Ax

		Ay*((Px-Bx*b)/Ax) + By*b = Py
		(PxAy - AyBxb)/Ax + Byb = Py
		PxAy - AyBxb + AxByb = PyAx
		AxByb - AyBxb = PyAx - PxAy
		b(AxBy - AyBx) = PyAx - PxAy
		b = (PyAx - PxAy) / (AxBy - AyBx)
		*/
		auto Px = game.Prize.X;
		auto Py = game.Prize.Y;
		auto Ax = game.A.X;
		auto Ay = game.A.Y;
		auto Bx = game.B.X;
		auto By = game.B.Y;
		
		auto b = (Py * Ax - Px * Ay) / (Ax * By - Ay * Bx);
		auto a = (Px - Bx * b) / Ax;

		if((game.A * a) + (game.B * b) == game.Prize) {
			return a * 3 + b;
		}
		return 0;
	}

	constexpr s64 Solve(const std::vector<std::string>& lines, u64 extra) {
		auto groups = SplitInputIntoGroups(lines);
		s64 result = 0;
		for (const auto& group : groups) {
			auto game = ParseGame(group, extra);
			result += SolveGame(game);
		}
		return result;
	}
	PART(1) {
		return Solve(lines, 0);
	}

	PART(2) {
		return Solve(lines, 10'000'000'000'000ll);
	}

	static_assert(SolveGame(Game{ { 94, 34 }, { 22, 67 }, { 8400, 5400 } }) == 280);
	static_assert(SolveGame(Game{ {26, 66}, {67, 21}, {12748, 12176} }) == 0);
	static_assert(SolveGame(Game{ {17, 86}, {84, 37}, {7870, 6450} }) == 200);
	static_assert(SolveGame(Game{ {69, 23}, {27, 71}, {18641, 10279} }) == 0);
}