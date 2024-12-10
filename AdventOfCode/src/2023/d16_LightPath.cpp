#include "Common.h"
#include "Facing.h"

SOLUTION(2023, 16) {
	constexpr Facing TurnIfNeeded(Facing dir, char c) {
		if (c != '/' && c != '\\') return dir;
		switch(dir) {
			case Up: return c == '/' ? Right : Left;
			case Down: return c == '/' ? Left : Right;
			case Left: return c == '/' ? Down : Up;
			case Right: return c == '/' ? Up : Down;
		}
		throw "Wat?";
	}

	constexpr size_t CountEnergy(const std::vector<std::string>& lines, RowCol startPos, Facing startDir) {
		struct KeyHash {
			constexpr size_t operator()(const std::pair<RowCol, Facing>& p) const {
				return Constexpr::Hasher<RowCol>{}(p.first);
			}
		};
		Constexpr::BigSet<std::pair<RowCol, Facing>, 50'000, KeyHash> seen;
		Constexpr::BigSet<RowCol, 50'000> energized;
		energized.SetSentinel({ 9999, 9999 });

		std::vector<std::pair<RowCol, Facing>> remaining;
		auto startChar = lines[startPos.Row][startPos.Col];
		remaining.push_back(std::make_pair(startPos, TurnIfNeeded(startDir, startChar)));

		seen.insert(remaining.back());
		energized.insert(startPos);

		while (!remaining.empty()) {
			auto key = remaining.back();
			auto [pos, dir] = key;
			remaining.pop_back();

			Move(pos, dir);
			if (pos.Row >= lines.size() || pos.Col >= lines[0].size()) continue;

			auto c = lines[pos.Row][pos.Col];
			dir = TurnIfNeeded(dir, c);

			if (!seen.insert(std::make_pair(pos, dir))) continue;
			energized.insert(pos);

			if ((c == '-' && (dir == Up || dir == Down)) ||
				(c == '|' && (dir == Left || dir == Right))) {
				remaining.push_back(std::make_pair(pos, TurnLeft(dir)));
				remaining.push_back(std::make_pair(pos, TurnRight(dir)));
			}
			else {
				remaining.push_back(std::make_pair(pos, dir));
			}
		}
	
		return energized.size();
	}

	PART(1) {
		return Constexpr::ToString(CountEnergy(lines, { 0, 0 }, Right));
	}

	PART(2) {
		size_t result = 0;
		for (size_t row = 0; row < lines.size(); row++) {
			result = std::max(result, CountEnergy(lines, { row, 0 }, Right));
			result = std::max(result, CountEnergy(lines, { row, lines[0].size() - 1 }, Left));
		}
		for (size_t col = 0; col < lines[0].size(); col++) {
			result = std::max(result, CountEnergy(lines, { 0, col }, Down));
			result = std::max(result, CountEnergy(lines, { lines.size() - 1, col }, Up));
		}

		return Constexpr::ToString(result);
	}

	TEST(1) {
		std::vector<std::string> lines = {
			".|...\\....",
			"|.-.\\.....",
			".....|-...",
			"........|.",
			"..........",
			".........\\",
			"..../.\\\\..",
			".-.-/..|..",
			".|....-|.\\",
			"..//.|...."
		};

		return Part1(lines) == "46";
	}
}