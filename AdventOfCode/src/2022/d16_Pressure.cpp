#include "Common.h"
#include "Core/Algorithms/FloydWarshall.h"
#include "Core/Constexpr/ConstexprBits.h"

SOLUTION(2022, 16) {
    struct Room {
        std::string Name = "";
        u32 FlowRate = 0;
        std::vector<std::string> Connections{};
    };

    template<size_t RoomCount, typename TDistance>
    struct State {
        std::vector<Room> Rooms{};
        Constexpr::SmallMap<std::string, size_t> IndexMap{};
        u64 AllRooms{0};

        std::array<std::array<TDistance, RoomCount>, RoomCount> DistanceMap{};
    };

    //Valve XK has flow rate=15; tunnels lead to valves CD, JC, TP, UE
    constexpr Room ParseRoom(std::string_view line) {
        auto s = Constexpr::Split(line, " ");
        Room room;
        room.Name = std::string(s[1]);
        
        auto flowRate = Constexpr::Split(s[4], "=")[1];
        flowRate = flowRate.substr(0, flowRate.size() - 1); //remove semi-colon
        Constexpr::ParseNumber(flowRate, room.FlowRate);

        for (size_t i = 9; i < s.size() - 1; i++) {
            room.Connections.push_back(std::string(s[i].substr(0, s[i].size() - 1)));
        }

        room.Connections.push_back(std::string(s.back()));
        return room;
    }

    constexpr Constexpr::SmallMap<std::string, size_t> GetIndexMap(const std::vector<Room>&rooms) {
        Constexpr::SmallMap<std::string, size_t> indexMap;

        for (size_t i = 0; i < rooms.size(); i++) {
            indexMap[rooms.at(i).Name] = i;
        }

        return indexMap;
    }

    template<size_t Verts, typename TDistance>
    constexpr std::array<std::array<TDistance, Verts>, Verts> GetDistanceMap(const std::vector<Room>&rooms, const Constexpr::SmallMap<std::string, size_t>&indexMap) {
        std::array<std::array<TDistance, Verts>, Verts> result;

        for (size_t i = 0; i < Verts; i++) {
            std::fill(result.at(i).begin(), result.at(i).end(), static_cast<TDistance>(99));
        }

        for (size_t i = 0; i < rooms.size(); i++) {
            auto room = rooms.at(i);
            for (const auto& connection : room.Connections) {
                auto a = indexMap.at(room.Name);
                auto b = indexMap.at(connection);

                result[a][a] = 0;
                result[b][b] = 0;
                result[a][b] = 1;
                result[b][a] = 1;
            }
        }
        FloydWarshall(result);

        return result;

    }

    template<size_t RoomCount, typename TDistance>
    constexpr State<RoomCount, TDistance> ParseState(const auto& lines) {
        State<RoomCount, TDistance> state;
        state.Rooms = ParseLines(lines, ParseRoom);
        auto allIndexes = GetIndexMap(state.Rooms);
        state.DistanceMap = GetDistanceMap<RoomCount, TDistance>(state.Rooms, allIndexes);

        for (const auto& [name, index] : allIndexes) {
            if (state.Rooms[index].FlowRate > 0 || name == "AA") {
                //state.IndexMap.insert({ name, index });
                state.IndexMap[name] = index;
                Constexpr::SetBit(state.AllRooms, index);
            }
        }

        return state;
    }

    template<size_t RoomCount, typename TDistance>
    constexpr void RecursePermutations(const State<RoomCount, TDistance>&state, u64 seen, size_t roomIndex, s16 remainingTime, u32 totalSteam, Constexpr::SmallMap<u64, u32>&outMap) {
        if (remainingTime <= 0) {
            outMap[seen] = std::max(outMap[seen], totalSteam);
            return;
        }

        totalSteam += static_cast<u32>(state.Rooms[roomIndex].FlowRate * remainingTime);
        Constexpr::SetBit(seen, roomIndex);

        if (seen == state.AllRooms) {
            outMap[seen] = std::max(outMap[seen], totalSteam);
            return;
        }

        for (const auto& [nextRoomName, nextIndex] : state.IndexMap) {
            if (Constexpr::IsBitSet(seen, nextIndex)) continue;
            auto distance = state.DistanceMap.at(roomIndex).at(nextIndex) + 1;
            auto nextTime = static_cast<s16>(remainingTime - distance);

            RecursePermutations(state, seen, nextIndex, nextTime, totalSteam, outMap);
        }
    }

    template<size_t RoomCount, typename TDistance>
    constexpr u32 FindBestSolo(const auto& lines) {
        auto state = ParseState<RoomCount, TDistance>(lines);
        auto startIndex = state.IndexMap.at("AA");
        Constexpr::SmallMap<u64, u32> permutations;

        RecursePermutations<RoomCount, TDistance>(state, 0, startIndex, 30, 0, permutations);
        std::vector<std::pair<u64, u32>> pairs;
        for (const auto& [valves, value] : permutations) {
            auto withoutStart = valves - (u64(1) << startIndex);
            pairs.push_back(std::make_pair(withoutStart, value));
        }

        std::sort(pairs.begin(), pairs.end(), [](const std::pair<u64, u32>& lhs, const std::pair<u64, u32>& rhs) {
            return lhs.second > rhs.second;
            });

        return pairs[0].second;
    }

    template<size_t RoomCount, typename TDistance>
    constexpr u32 FindBestPair(const auto&lines) {
        auto state = ParseState<RoomCount, TDistance>(lines);
        auto startIndex = state.IndexMap.at("AA");
        Constexpr::SmallMap<u64, u32> permutations;

        RecursePermutations<RoomCount, TDistance>(state, 0, startIndex, 26, 0, permutations);

        std::vector<std::pair<u64, u32>> pairs;
        for (const auto& [valves, value] : permutations) {
            auto withoutStart = valves - (u64(1) << startIndex);
            pairs.push_back(std::make_pair(withoutStart, value));
        }

        std::sort(pairs.begin(), pairs.end(), [](const std::pair<u64, u32>& lhs, const std::pair<u64, u32>& rhs) {
            return lhs.second > rhs.second;
            });

        u32 best = 0;
        for (size_t i = 0u; i < pairs.size(); i++) {
            for (auto j = i + 1; j < pairs.size(); j++) {
                if ((pairs[i].first & pairs[j].first) == 0) {
                    best = std::max(best, pairs[i].second + pairs[j].second);
                }
            }
        }
        return best;
    }

    // consider finding the maximum flow rate
    // and early-out if the remaining time at the max flow rate would be less than the best
    PART(1) {
        return FindBestSolo<51, u8>(lines);
    }

    PART(2) {
        return FindBestPair<51, u8>(lines);
    }

    TEST(1) {
        std::vector<std::string> lines = {
            "Valve AA has flow rate=0; tunnels lead to valves DD, II, BB",
            "Valve BB has flow rate=13; tunnels lead to valves CC, AA",
            "Valve CC has flow rate=2; tunnels lead to valves DD, BB",
            "Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE",
            "Valve EE has flow rate=3; tunnels lead to valves FF, DD",
            "Valve FF has flow rate=0; tunnels lead to valves EE, GG",
            "Valve GG has flow rate=0; tunnels lead to valves FF, HH",
            "Valve HH has flow rate=22; tunnel leads to valve GG",
            "Valve II has flow rate=0; tunnels lead to valves AA, JJ",
            "Valve JJ has flow rate=21; tunnel leads to valve II"
        };

        return FindBestSolo<10, u8>(lines) == 1651;
    }
}