#include "Common.h"

SOLUTION(2023, 10) {
    using Map = Constexpr::BigMap<RowCol, std::pair<RowCol, RowCol>>;
    //static RowCol StartPos = { 0, 0 };

    constexpr void Connect(char current, RowCol pos, Map& map, RowCol& startPos) {
        RowCol up = { pos.Row - 1, pos.Col };
        RowCol down = { pos.Row + 1, pos.Col };
        RowCol left = { pos.Row, pos.Col - 1 };
        RowCol right = { pos.Row, pos.Col + 1 };
        switch (current) {
        case '|': map[pos] = std::make_pair(up, down); break;
        case '-': map[pos] = std::make_pair(left, right); break;
        case 'L': map[pos] = std::make_pair(up, right); break;
        case 'J': map[pos] = std::make_pair(up, left); break;
        case '7': map[pos] = std::make_pair(left, down); break;
        case 'F': map[pos] = std::make_pair(right, down); break;
        case 'S': startPos = pos; break;
        }
    }

    constexpr Map CreateMap(const std::vector<std::string>& lines, RowCol& startPos) {
        Map result;
        RowCol pos = { 0, 0 };

        for (const auto& line : lines) {
            for (auto c : line) {
                Connect(c, pos, result, startPos);
                pos.Col++;
            }
            pos.Row++;
            pos.Col = 0;
        }

        return result;
    }

    constexpr RowCol GetNext(RowCol current, RowCol prev, const Map & map) {
        auto [left, right] = map.at(current);
        return left == prev ? right : left;
    }

    constexpr bool TryGetPath(RowCol start, const Map & map, std::vector<RowCol>&outPath, const RowCol& startPos) {
        if (!map.contains(start)) return false;
        RowCol current = start;
        RowCol next = map.at(current).first;
        outPath.clear();
        if (next == startPos) next = map.at(current).second;
        outPath.push_back(current);
        while (true) {
            auto prev = current;
            current = next;
            next = GetNext(current, prev, map);
            outPath.push_back(current);
            if (next == startPos) break;
            if (!map.contains(next)) return false;
        }
        outPath.push_back(startPos);
        return true;
    }

    constexpr std::vector<RowCol> GetPath(const Map & map, const RowCol& startPos) {
        auto neighbors = GetDirectNeighbors(startPos, { 999, 999 });
        std::vector<std::vector<RowCol>> candidates;
        for (auto n : neighbors) {
            std::vector<RowCol> result;
            if (TryGetPath(n, map, result, startPos)) {
                candidates.push_back(result);
            }
        }

        if (candidates.empty()) {
            throw "Wat?";
        }
        std::sort(candidates.begin(), candidates.end(), [](const std::vector<RowCol>& lhs, const std::vector<RowCol>& rhs) {
            return lhs.size() < rhs.size();
            });
        return candidates.back();
    }

    PART(1) {
        RowCol startPos = { 0, 0 };

        auto map = CreateMap(lines, startPos);
        auto path = GetPath(map, startPos);
        return (path.size() / 2);
    }

    constexpr Constexpr::BigSet<RowCol> GetLoop(const std::vector<RowCol>&path) {
        Constexpr::BigSet<RowCol> result;
        result.SetSentinel({ 9999, 9999 });
        for (auto p : path) result.insert(p);
        return result;
    }

    constexpr char GetStartPosSymbol(const std::vector<RowCol>& path, const RowCol& startPos) {
        auto a = path[0];
        auto b = path[path.size() - 2];
        
        if (a.Row == b.Row) {
            return '-';
        }
        else if (a.Col == b.Col) {
            return '|';
        }

        if (a.Row < b.Row) {
            if (a.Col < b.Col) {
                if (startPos.Row == a.Row) {
                    return '7';
                }
                else {
                    return 'L';
                }
            }
            else {
                if (startPos.Row == a.Row) {
                    return 'F';
                }
                else {
                    return 'J';
                }
            }
        }
        else {
            if (a.Col < b.Col) {
                if (startPos.Row == a.Row) {
                    return 'J';
                }
                else {
                    return 'F';
                }
            }
            else {
                if (startPos.Row == a.Row) {
                    return 'L';
                }
                else {
                    return '7';
                }
            }
        }
    }

    constexpr bool EntranceMatchesExit(char entrance, char exit) {
        switch (entrance) {
        case 'L': return exit == 'J';
        case 'J': return exit == 'L';
        case 'F': return exit == '7';
        case '7': return exit == 'F';
        }
        return false;
    }

    //if entrance matches exit (L, J or F, 7) it's 2 pipes
    //otherwise it's 1 pipe
    constexpr std::string TransformRow(RowCol pos, const std::string & line, const Constexpr::BigSet<RowCol>&loop) {
        std::string result;
        bool inside = false;
        char entrance = ' ';
        for (auto c : line) {
            if (loop.contains(pos)) {
                result.push_back(c);
                if (c == '-') {}
                else if (c == '|') inside = !inside;
                else {
                    if (entrance == ' ') entrance = c;
                    else {
                        if (!EntranceMatchesExit(entrance, c)) {
                            inside = !inside;
                        }
                        entrance = ' ';
                    }
                }
            }
            else {
                result += inside ? 'I' : 'O';
            }
            pos.Col++;
        }

        return result;
    }

    constexpr std::vector<std::string> Transform(const std::vector<std::string>& lines, RowCol& startPos) {
        std::vector<std::string> result;
        auto path = GetPath(CreateMap(lines, startPos), startPos);
        auto loop = GetLoop(path);
        RowCol pos{ 0, 0 };

        auto copy = lines;
        copy[startPos.Row][startPos.Col] = GetStartPosSymbol(path, startPos);
        for (const auto& line : copy) {
            result.push_back(TransformRow(pos, line, loop));
            pos.Row++;
        }
        return result;
    }

    PART(2) {
		RowCol startPos = { 0, 0 };
        auto transformed = Transform(lines, startPos);
        return std::accumulate(transformed.begin(), transformed.end(), 0ull, [](size_t running, const std::string& line) {
            return running + std::count(line.begin(), line.end(), 'I');
            });
    }

    TEST(1) {
        std::vector<std::string> lines = {
            ".....",
            ".S-7.",
            ".|.|.",
            ".L-J.",
            "....."
        };

        if (std::get<size_t>(Part1(lines)) != 4) return false;
        if (std::get<size_t>(Part2(lines)) != 1) return false;
        return true;
    }

    TEST(2) {
        std::vector<std::string> lines = {
            "..........",
            ".S------7.",
            ".|F----7|.",
            ".||....||.",
            ".||....||.",
            ".|L-7F-J|.",
            ".|..||..|.",
            ".L--JL--J.",
            ".........."
        };

        return std::get<size_t>(Part2(lines)) == 4;
    }

    TEST(3) {
        std::vector<std::string> lines = {
            "FF7FSF7F7F7F7F7F---7",
            "L|LJ||||||||||||F--J",
            "FL-7LJLJ||||||LJL-77",
            "F--JF--7||LJLJ7F7FJ-",
            "L---JF-JLJ.||-FJLJJ7",
            "|F|F-JF---7F7-L7L|7|",
            "|FFJF7L7F-JF7|JL---7",
            "7-L-JL7||F7|L7F-7F7|",
            "L.L7LFJ|||||FJL7||LJ",
            "L7JLJL-JLJLJL--JLJ.L"
        };

        return std::get<size_t>(Part2(lines)) == 10;
    }
}