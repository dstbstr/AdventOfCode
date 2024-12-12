#include "Common.h"

SOLUTION(2023, 23) {
    constexpr RowCol horizontal{ 0, 1 };
    constexpr RowCol vertical{ 1, 0 };
    RowCol start{ 0, 1 };

	constexpr bool IsPath(const std::vector<std::string>& lines, RowCol pos) {
		return lines[pos.Row][pos.Col] != '#';
	}

    constexpr bool IsNode(const std::vector<std::string>& lines, RowCol pos) {
        if (lines[pos.Row][pos.Col] == '#') return false;
		return 3 <= IsPath(lines, pos - horizontal) + IsPath(lines, pos + horizontal) + IsPath(lines, pos - vertical) + IsPath(lines, pos + vertical);
    }

	constexpr void Step(const std::vector<std::string>& map, RowCol& pos, RowCol& prev) {
        auto dir = pos - prev;
        auto next = pos + dir;

        if (!IsPath(map, next)) {
            std::swap(dir.Row, dir.Col);
            next = IsPath(map, pos + dir) ? pos + dir : pos - dir;
        }
        prev = pos;
        pos = next;
    }

    constexpr RowCol Downhill(char c, RowCol pos) {
        switch(c) {
		case '<': return pos - horizontal;
		case '>': return pos + horizontal;
		case 'v': return pos + vertical;
		case '^': return pos - vertical;
        }
        throw "Wat?";
    }

    template<bool IsPart1>
    constexpr void RecurseDistances(const std::vector<std::string>& map, u32 fromNode, RowCol pos, RowCol prev, std::vector<std::vector<u32>>& outDistances) {
        u32 steps = 0;
        while(map[pos.Row][pos.Col] >= '.') {
            Step(map, pos, prev);
            steps++;
        }
        auto toNode = map[pos.Row][pos.Col];
		if (outDistances[fromNode][toNode] > 0) return;

		outDistances[fromNode][toNode] = steps;
        if constexpr(!IsPart1) {
			outDistances[toNode][fromNode] = steps;
        }
		if (toNode == static_cast<char>(outDistances.size() - 1)) return; // found the end

        auto dir = pos - prev;
        auto next = pos + dir;
		char c = map[next.Row][next.Col];
        if(IsPath(map, next)) {
            if(c == '.' || Downhill(c, next) != pos) {
				RecurseDistances<IsPart1>(map, toNode, next, pos, outDistances);
            }
        }
		std::swap(dir.Row, dir.Col);
        next = pos + dir;
		c = map[next.Row][next.Col];
        if(IsPath(map, next)) {
			if (c == '.' || Downhill(c, next) != pos) {
                RecurseDistances<IsPart1>(map, toNode, next, pos, outDistances);
            }
        }
		next = pos - dir;
		c = map[next.Row][next.Col];
		if (IsPath(map, next)) {
			if (c == '.' || Downhill(c, next) != pos) {
				RecurseDistances<IsPart1>(map, toNode, next, pos, outDistances);
            }
        }
    }

	constexpr u32 RecursePaths(const std::vector<std::vector<u32>>& distances, const std::vector<RowCol>& nodes, size_t fromNode, u32 currentDistance, u64 path) {
        u32 best = 0;
        if (fromNode == nodes.size() - 1) return currentDistance;
        const auto& neighbors = distances[fromNode];
        Constexpr::SetBit(path, fromNode);
        for(size_t n = 0; n < neighbors.size(); n++) {
            if (neighbors[n] == 0 || Constexpr::IsBitSet(path, n)) continue;
			auto distance = currentDistance + neighbors[n] + 1;
			best = std::max(best, RecursePaths(distances, nodes, n, distance, path));
        }
        Constexpr::UnsetBit(path, fromNode);
        return best;
	}

    constexpr std::vector<RowCol> GetNodes(std::vector<std::string>& map) {
        RowCol end{ map.size() - 1, map[0].size() - 2 };
        std::vector<RowCol> nodes{ start };
        for (size_t row = 1; row < map.size() - 1; row++) {
            for (size_t col = 1; col < map[0].size() - 1; col++) {
                if (IsNode(map, { row, col })) {
                    map[row][col] = static_cast<u8>(nodes.size());
                    nodes.push_back({ col, row });
                }
            }
        }
        map[start.Row][start.Col] = static_cast<u8>(0);
        map[end.Row][end.Col] = static_cast<u8>(nodes.size());
        nodes.emplace_back(end);

        return nodes;
    }

    template<bool IsPart1>
	constexpr std::vector<std::vector<u32>> GetDistances(const std::vector<std::string>& map, const std::vector<RowCol>& nodes) {
		std::vector<std::vector<u32>> distances(nodes.size(), std::vector<u32>(nodes.size(), 0));
		RecurseDistances<IsPart1>(map, 0, start + vertical, start, distances);
		return distances;
	}

    constexpr u32 SolvePart1(const std::vector<std::string>& lines) {
        auto map = lines;
		auto nodes = GetNodes(map);
        auto distances = GetDistances<true>(map, nodes);

        return RecursePaths(distances, nodes, 0, 0, 0);
    }

    constexpr u32 SolvePart2(const std::vector<std::string>& lines) {
        auto map = lines;
        for (auto& line : map) {
            for (auto& c : line) {
                if (c != '#') c = '.';
            }
        }
        auto nodes = GetNodes(map);
        auto distances = GetDistances<false>(map, nodes);

        return RecursePaths(distances, nodes, 0, 0, 0);
    }

    PART(1) {
		return SolvePart1(lines);
    }
    PART(2) {
        return SolvePart2(lines);
    }

    TEST(1) {
        std::vector<std::string> lines = {
            "#.#####################",
            "#.......#########...###",
            "#######.#########.#.###",
            "###.....#.>.>.###.#.###",
            "###v#####.#v#.###.#.###",
            "###.>...#.#.#.....#...#",
            "###v###.#.#.#########.#",
            "###...#.#.#.......#...#",
            "#####.#.#.#######.#.###",
            "#.....#.#.#.......#...#",
            "#.#####.#.#.#########v#",
            "#.#...#...#...###...>.#",
            "#.#.#v#######v###.###v#",
            "#...#.>.#...>.>.#.###.#",
            "#####v#.#.###v#.#.###.#",
            "#.....#...#...#.#.#...#",
            "#.#########.###.#.#.###",
            "#...###...#...#...#.###",
            "###.###.#.###v#####v###",
            "#...#...#.#.>.>.#.>.###",
            "#.###.###.#.###.#.#v###",
            "#.....###...###...#...#",
            "#####################.#"
        };

		if (SolvePart1(lines) != 94) return false;
        if (SolvePart2(lines) != 154) return false;

        return true;
    }
}