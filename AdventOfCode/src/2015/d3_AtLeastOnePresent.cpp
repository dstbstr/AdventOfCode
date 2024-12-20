#include "Common.h"

SOLUTION(2015, 3) {
    constexpr size_t GridSize = 1000;

    constexpr size_t GetPos(u32 x, u32 y)
    {
        return (x * GridSize) + y;
    }

    constexpr void Move(char c, u32& x, u32& y) {
        switch (c) {
        case '<': x--; break;
        case '>': x++; break;
        case '^': y++; break;
        case 'v': y--; break;
        }
    }

    constexpr u32 UpdateSeen(u32 x, u32 y, u32& seenIndex, std::array<size_t, 8000>& seen) {
        auto coord = GetPos(x, y);
        if (std::find(seen.begin(), seen.end(), coord) == seen.end()) {
            seen[seenIndex++] = coord;
            return 1;
        }

        return 0;
    }

    constexpr u32 CountDeliveries(std::string_view input) {
        auto seen = std::array<size_t, 8000>{};
        u32 seenIndex = 0;
        u32 x = GridSize / 2;
        u32 y = GridSize / 2;
        u32 result = 1;

        seen[seenIndex++] = GetPos(x, y);

        for (const auto c : input) {
            Move(c, x, y);

            result += UpdateSeen(x, y, seenIndex, seen);
        }

        return result;
    }

    constexpr u32 CountDeliveriesWithRobot(std::string_view input) {
        auto seen = std::array<size_t, 8000>{};
        u32 seenIndex = 0;
        u32 santaX = GridSize / 2;
        u32 santaY = GridSize / 2;
        u32 botX = santaX;
        u32 botY = santaY;
        u32 result = 1;

        seen[seenIndex++] = GetPos(santaX, santaY);

        for (size_t i = 0u; i < input.size(); i += 2)
        {
            Move(input[i], santaX, santaY);
            result += UpdateSeen(santaX, santaY, seenIndex, seen);

            Move(input[i + 1], botX, botY);
            result += UpdateSeen(botX, botY, seenIndex, seen);
        }

        return result;
    }

    PART(1) {
        return CountDeliveries(lines[0]);
    }

    PART(2) {
        return CountDeliveriesWithRobot(lines[0]);
    }

    static_assert(CountDeliveries(">") == 2);
    static_assert(CountDeliveries("^>v<") == 4);

    static_assert(CountDeliveriesWithRobot("^v") == 3);
    static_assert(CountDeliveriesWithRobot("^>v<") == 3);
    static_assert(CountDeliveriesWithRobot("^v^v^v^v^v") == 11);
}