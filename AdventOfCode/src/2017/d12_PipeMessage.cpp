#include "Common.h"
#include "Core/Algorithms/FloodFill.h"

SOLUTION(2017, 12) {
    constexpr std::vector<u32> ParseLine(std::string_view line) {
        auto rhs = std::string(Constexpr::Split(line, " <-> ")[1]);
        return ParseLineAsNumbers<u32>(rhs, ", ");
    }

    PART(1) {
        auto connections = ParseLines(lines, ParseLine);
        return FloodFill(0, [&connections](u32 pipe) { return connections[pipe]; }).size();
    }

    PART(2) {
        auto connections = ParseLines(lines, ParseLine);
        auto remainingGroups = std::views::iota(0u, static_cast<u32>(connections.size()))
            | std::ranges::to<std::vector<u32>>();
        /*
        std::vector<u32> remainingGroups;
        for (size_t i = 0u; i < connections.size(); i++) {
            remainingGroups.push_back(static_cast<u32>(i));
        }
        */

        u32 groupCount = 0;
        while (!remainingGroups.empty()) {
            groupCount++;
            auto group = FloodFill(remainingGroups.back(), [&connections](u32 pipe) { return connections[pipe]; });
            std::erase_if(remainingGroups, [&group](u32 val) {
                return std::find(group.cbegin(), group.cend(), val) != group.cend();
                });
        }

        return groupCount;
    }

    static_assert(ParseLine("0 <-> 2").size() == 1);
    static_assert(ParseLine("0 <-> 2")[0] == 2u);
    static_assert(ParseLine("2 <-> 0, 3, 4").size() == 3);
    static_assert(ParseLine("2 <-> 0, 3, 4") == std::vector<u32>{0, 3, 4});
}