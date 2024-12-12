#include "Common.h"

SOLUTION(2018, 22) {
    static constexpr u64 limit = 20183;
    static constexpr u64 RowMul = 48271;
    static constexpr u64 ColMul = 16807;
    static constexpr size_t Unset = limit * 2;

    constexpr size_t GetIndex(size_t row, size_t col, size_t up, size_t left) {
        return (up != Unset) && (left != Unset) ? up * left : (col * ColMul) + (row * RowMul);
    }
    constexpr std::vector<Vec3<u64>> BuildMap(RowCol target, RowCol limits, size_t depth) {
        std::vector<Vec3<u64>> result;
        std::vector<size_t> indexes;

        for (size_t row = 0; row <= limits.Row; row++) {
            for (size_t col = 0; col <= limits.Col; col++) {
                auto up = row == 0 ? Unset : indexes[((row - 1) * (limits.Col + 1)) + col];
                auto left = col == 0 ? Unset : indexes.back();
                auto index = (depth + GetIndex(row, col, up, left)) % limit;
                Vec3<u64> pos = { col, row, (index % 3) + 1 };

                result.push_back(pos);
                indexes.push_back(index);
            }
        }

        result[target.Row * (limits.Col + 1) + target.Col].Z = 1;
        return result;
    }

    constexpr u64 GetRisk(const std::vector<Vec3<u64>>& map) {
        u64 result = 0;
        for (const auto& pos : map) {
            result += pos.Z;
        }
        return result - map.size();
    }
    
    constexpr size_t GetMapIndex(const Vec3<size_t>& pos, const Vec3<size_t>& limits) {
        return (pos.Y * (limits.X + 1)) + pos.X;
    }

    constexpr size_t GetMapIndex1(const Vec4<size_t>& pos, const Vec4<size_t>& limits) {
        return (pos.Y * (limits.X + 1)) + pos.X;
    }

    std::string ToString(const std::vector<Vec3<size_t>>& map, RowCol target) {
        std::string result;
        result.reserve(map.size());

        for (const auto& pos : map) {
            if (pos.X == 0) result.push_back('\n');
            char icon = pos.Z == 1 ? '.' : pos.Z == 2 ? '=' : '|';
            if (pos.X == 0 && pos.Y == 0) icon = 'M';
            else if (pos.X == target.Col && pos.Y == target.Row) icon = 'T';
            result.push_back(icon);
        }

        return result;
    }

    //STONE = 1, WET = 2, NARROW = 3
    //NONE = 1, TORCH = 2, CLIMBING = 3
    
    //Allowed
    //Z != equipment

    //Shared
    // 6 - lhs.Z - rhs.Z

    size_t Bfs(const std::vector<Vec3<u64>>& map, RowCol target) {
        Vec4<size_t> Horizontal = { 1u, 0u, 0u, 0u };
        Vec4<size_t> Vertical = { 0u, 1u, 0u, 0u };
        auto back = map.back();
        Vec4<size_t> limits = { 
            static_cast<size_t>(back.X), 
            static_cast<size_t>(back.Y), 
            static_cast<size_t>(back.Z), 
            size_t(3u)
        };
        Vec4<size_t> end = { target.Col, target.Row, size_t(1u), size_t(2u) };

        using Entry = std::pair<size_t, Vec4<size_t>>;
        std::vector<Entry> q{{0, { 0, 0, 1, 2 }}};
        std::vector<Entry> next;
        Constexpr::BigSet<Vec4<size_t>> seen;

        size_t minute = 0;
        while (true) {
            for (const auto& [waiting, pos] : q) {
                if (waiting > 0) {
                    next.push_back({ {waiting - 1}, pos });
                    continue;
                }
                if (pos == end) {
                    return minute;
                }
                if (!seen.insert(pos)) continue;

                std::vector<Vec3<u64>> neighbors;

                if (pos.Y > 0) neighbors.push_back(map[GetMapIndex1(pos - Vertical, limits)]);
                if (pos.X > 0) neighbors.push_back(map[GetMapIndex1(pos - Horizontal, limits)]);
                if (pos.Y < limits.Y) neighbors.push_back(map[GetMapIndex1(pos + Vertical, limits)]);
                if (pos.X < limits.X) neighbors.push_back(map[GetMapIndex1(pos + Horizontal, limits)]);

                for (auto& n : neighbors) {
					auto sizetN = Vec3<size_t>{ static_cast<size_t>(n.X), static_cast<size_t>(n.Y), static_cast<size_t>(n.Z )};
                    if (pos.W != sizetN.Z) {
                        next.push_back({ 0, { sizetN.X, sizetN.Y, sizetN.Z, pos.W } }); //keep same tool
                    }
                    if (pos.Z == sizetN.Z) {
                        continue; //No need to switch
                    }
                    size_t commonTool = (6 - pos.Z) - sizetN.Z;
                    next.push_back({ 7, {sizetN.X, sizetN.Y, sizetN.Z, commonTool } }); //switch to common tool
                }
            }
            minute++;
            q = next;
            next.clear();
        }
        return minute;
    }

    //depth: 7863
    //target: 14,760
    constexpr void GetTargetAndDepth(const auto& lines, RowCol& outTarget, u32& outDepth) {
        auto s1 = Constexpr::Split(lines[0], " ");
        Constexpr::ParseNumber(s1[1], outDepth);

        auto s2 = Constexpr::Split(lines[1], ": ");
        auto xy = Constexpr::Split(s2[1], ",");
        Constexpr::ParseNumber(xy[0], outTarget.Col);
        Constexpr::ParseNumber(xy[1], outTarget.Row);
    }

    PART(1) {
        RowCol target;
        u32 depth;
        GetTargetAndDepth(lines, target, depth);

        auto map = BuildMap(target, target, depth);
        return GetRisk(map);
    }

    PART(2) {
        RowCol target;
        u32 depth;
        GetTargetAndDepth(lines, target, depth);
        RowCol buffer = { target.Row + 50, target.Col + 500 };
        auto map = BuildMap(target, buffer, depth);
        return Bfs(map, target); //< 1 s
    }

    TEST(1) {
        RowCol target = { 10, 10 };
        auto map = BuildMap(target, target, size_t(510u));
        if (GetRisk(map) != 114ull) return false;

        return true;
    }
}