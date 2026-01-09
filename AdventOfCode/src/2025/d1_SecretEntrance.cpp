#include "Common.h"
#include <print>

SOLUTION(2025, 1) {
	constexpr int Turn(int dial, int amount) {
		return (((dial + amount) % 100) + 100) % 100;
	}

	constexpr size_t OnZero(int dial, int amount) {
		return (dial + amount) % 100 == 0;
	}
	constexpr size_t PassZero(int dial, int amount) {
		int end = dial + amount;

		if (end > 0) {
			return end / 100;
		}
		else {
			end = -end;
			return end / 100 + (dial > 0);
		}
	}

	constexpr size_t Solve(const std::vector<std::string>& lines, auto fn) {
		size_t result = 0;
		int dial = 50;
		int amount;
		for (const auto& line : lines) {
			Constexpr::ParseNumber(line.substr(1), amount);
			amount *= 1 - 2 * (line[0] == 'L');
			result += fn(dial, amount);
			dial = Turn(dial, amount);
		}
		return result;
	}

	PART(1) {
		return Solve(lines, OnZero);
	}
	PART(2) {
		return Solve(lines, PassZero);
	}

	static_assert(Turn(99, 1) == 0);
	static_assert(Turn(0, -1) == 99);
	static_assert(Turn(50, 1000) == 50);
	static_assert(Turn(50, -1000) == 50);

	static_assert(PassZero(0, 99) == 0);
	static_assert(PassZero(0, 1) == 0);
	static_assert(PassZero(99, 1) == 1);
	static_assert(PassZero(90, 10) == 1);
	static_assert(PassZero(1, -1) == 1);
	static_assert(PassZero(50, 1000) == 10);
	static_assert(PassZero(99, 101) == 2);
	static_assert(PassZero(0, -99) == 0);
	static_assert(PassZero(0, -100) == 1);
	static_assert(PassZero(10, -100) == 1);


	static_assert(PassZero(50, -68) == 1);
	static_assert(PassZero(82, -30) == 0);
	static_assert(PassZero(52, 48) == 1);
	static_assert(PassZero(0, -5) == 0);

	static_assert(Solve({
		"L68",
		"L30",
		"R48",
		"L5",
		"R60",
		"L55",
		"L1",
		"L99",
		"R14",
		"L82"
		}, OnZero) == 3);

	static_assert(Solve({
		"L68",
		"L30",
		"R48",
		"L5",
		"R60",
		"L55",
		"L1",
		"L99",
		"R14",
		"L82"
		}, PassZero) == 6);

}