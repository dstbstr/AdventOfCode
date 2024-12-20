#include "Common.h"

SOLUTION(2018, 6) {
    constexpr std::vector<u32> FindAreas(const std::vector<Coord>&coords, s32 area) {
        std::vector<u32> result(coords.size(), 0);

        for (s32 x = -area; x < area; x++) {
            for (s32 y = -area; y < area; y++) {
                size_t m = 100000;
                Coord point{ x, y };

                bool duplicate = false;
                size_t bestIndex = 0;
                for (size_t i = 0; i < coords.size(); i++) {
                    auto dist = MDistance(point, coords[i]);
                    if (dist == m) {
                        duplicate = true;
                    }
                    else if (dist < m) {
                        duplicate = false;
                        m = dist;
                        bestIndex = i;
                    }
                }

                if (!duplicate) {
                    result[bestIndex]++;
                }
            }
        }

        return result;
    }

    constexpr bool IsInRegion(const Coord & pos, std::vector<Coord> targets, size_t maxDistance) {
        size_t totalDistance = 0;
        for (const auto& target : targets) {
            totalDistance += MDistance(pos, target);
            if (totalDistance >= maxDistance) return false;
        }

        return true;
    }

    PART(1) {
        std::vector<Coord> coords;
        for (const auto& line : lines) {
            coords.push_back(Coord(line));
        }

        auto areas = FindAreas(coords, 400);
        auto areas2 = FindAreas(coords, 500);


        u32 maxArea = 0;
        for (size_t i = 0; i < coords.size(); i++) {
            if (areas[i] == areas2[i]) {
                maxArea = std::max(maxArea, areas[i]);
            }
        }

        return maxArea;
    }

    PART(2) {
        const size_t maxDistance = 10'000;

        std::vector<Coord> coords;
        for (const auto& line : lines) {
            coords.push_back(Coord(line));
        }

        u32 count = 0;
        for (s32 x = -1000; x < 1000; x++) {
            for (s32 y = -1000; y < 1000; y++) {
                if (IsInRegion({ x, y }, coords, maxDistance)) {
                    count++;
                }
            }
        }

        return count;
    }

    /*
    TEST(1) {
        std::vector<std::string> lines = {
            "1, 1",
            "1, 6",
            "8, 3",
            "3, 4",
            "5, 5",
            "8, 9"
        };
        
        if (Part1(lines) != "17") return false;

        return true;
    }
    */
}