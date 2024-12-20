#include "Common.h"

SOLUTION(2019, 10) {
    /*
    Angle between two points is just the slope, Rise/Run
    Two asteroids are on the same line if they share a slope (fully reduced)
    The closest is the asteroid with the relative position closest to the slope (5,15 and 1,3 are the same slope, but 1,3 doesn't need to be reduced)
    To find the 200th asteroid, sort the targets by quadrant, then by slope (Rise/Run)
    */
    using Map = std::vector<Coord>;
    using VisibilityMap = Constexpr::SmallMap<Coord, Constexpr::SmallMap<Coord, bool>>;

    constexpr Map ParseMap(const auto& lines) {
        Map result;
        for (auto row = 0ull; row < lines.size(); row++) {
            for (auto col = 0ull; col < lines[row].size(); col++) {
                if (lines[row][col] == '#') {
                    result.push_back({ static_cast<s64>(col), static_cast<s64>(row) });
                }
            }
        }
        return result;
    }

    struct Target {
        Coord Pos;
        Coord Slope;
        int Quad{ 0 };
        size_t Distance{ 0 };

        constexpr bool operator<(const Target& other) const {
            if (Quad != other.Quad) return Quad < other.Quad;
            if (Slope != other.Slope) {
                auto lhs = static_cast<double>(Slope.Y) / (Slope.X == 0 ? 0.00001 : static_cast<double>(Slope.X));
                auto rhs = static_cast<double>(other.Slope.Y) / (other.Slope.X == 0 ? 0.00001 : static_cast<double>(other.Slope.X));
                return lhs < rhs; //probably not totally correct, but works
            }
            return Distance < other.Distance;
        }

        constexpr bool operator==(const Target& other) const {
            return Slope == other.Slope;
        }
    };

    constexpr Target MakeTarget(Coord start, Coord end) {
        auto relative = Coord{ end.X - start.X, end.Y - start.Y };
        auto gcd = Constexpr::FindGcd(Constexpr::Abs(relative.X), Constexpr::Abs(relative.Y));
        Target t;
        t.Pos = end;
        t.Slope = { relative.X / gcd, relative.Y / gcd };
        t.Distance = gcd;
        if (t.Slope.X < 0) t.Quad = t.Slope.Y <= 0 ? 3 : 2;
        else t.Quad = t.Slope.Y < 0 ? 0 : 1;
        return t;
    }

    constexpr std::vector<Target> FindMostTargets(const Map& map) {
        std::vector<Target> result;
        Constexpr::SmallSet<Target> targets;
        size_t count = 0;
        for (const auto& start : map) {
            targets.clear();
            for (const auto& end : map) {
                if (start == end) continue;
                auto t = MakeTarget(start, end);
                if (!targets.insert(t)) {
                    auto& existing = targets[t];
                    if (t.Distance < existing.Distance) { //only keep the closest target
                        existing = t;
                    }
                }
            }

            if (count < targets.size()) {
                count = targets.size();

                result = std::vector<Target>(targets.begin(), targets.end());
            }
        }

        return result;
    }

    PART(1) {
        auto map = ParseMap(lines);
        return FindMostTargets(map).size();
    }

    PART(2) {
        auto map = ParseMap(lines);
        auto targets = FindMostTargets(map);
        std::sort(targets.begin(), targets.end());

        return (targets[199].Pos.X * 100) + targets[199].Pos.Y;
    }

    TEST(1) {
        std::vector<std::string> lines = {
            ".#..#",
            ".....",
            "#####",
            "....#",
            "...##"
        };
        
        auto targets = FindMostTargets(ParseMap(lines));
        return targets.size() == 8;
    }

    TEST(2) {
        std::vector<std::string> lines = {
            "......#.#.",
            "#..#.#....",
            "..#######.",
            ".#.#.###..",
            ".#..#.....",
            "..#....#.#",
            "#..#....#.",
            ".##.#..###",
            "##...#..#.",
            ".#....####"
        };
        auto targets = FindMostTargets(ParseMap(lines));
        return targets.size() == 33;
    }

    TEST(3) {
        std::vector<std::string> lines = {
            "#.#...#.#.",
            ".###....#.",
            ".#....#...",
            "##.#.#.#.#",
            "....#.#.#.",
            ".##..###.#",
            "..#...##..",
            "..##....##",
            "......#...",
            ".####.###."
        };
        auto targets = FindMostTargets(ParseMap(lines));
        return targets.size() == 35;
    }

    TEST(4) {
        std::vector<std::string> lines = {
            ".#..#..###",
            "####.###.#",
            "....###.#.",
            "..###.##.#",
            "##.##.#.#.",
            "....###..#",
            "..#.#..#.#",
            "#..#.#.###",
            ".##...##.#",
            ".....#.#.."
        };
        auto targets = FindMostTargets(ParseMap(lines));
        return targets.size() == 41;
    }

    TEST(5) {
        std::vector<std::string> lines = {
            ".#..##.###...#######",
            "##.############..##.",
            ".#.######.########.#",
            ".###.#######.####.#.",
            "#####.##.#.##.###.##",
            "..#####..#.#########",
            "####################",
            "#.####....###.#.#.##",
            "##.#################",
            "#####.##.###..####..",
            "..######..##.#######",
            "####.##.####...##..#",
            ".#####..#.######.###",
            "##...#.##########...",
            "#.##########.#######",
            ".####.#.###.###.#.##",
            "....##.##.###..#####",
            ".#.#.###########.###",
            "#.#.#.#####.####.###",
            "###.##.####.##.#..##"
        };
        auto targets = FindMostTargets(ParseMap(lines));
        return targets.size() == 210;
    }

    /*
    TEST(6) {
        std::vector<std::string> lines = {
            ".#..##.###...#######",
            "##.############..##.",
            ".#.######.########.#",
            ".###.#######.####.#.",
            "#####.##.#.##.###.##",
            "..#####..#.#########",
            "####################",
            "#.####....###.#.#.##",
            "##.#################",
            "#####.##.###..####..",
            "..######..##.#######",
            "####.##.####...##..#",
            ".#####..#.######.###",
            "##...#.##########...",
            "#.##########.#######",
            ".####.#.###.###.#.##",
            "....##.##.###..#####",
            ".#.#.###########.###",
            "#.#.#.#####.####.###",
            "###.##.####.##.#..##"
        };
        return Part2(lines) == "802";
    }
    */

}