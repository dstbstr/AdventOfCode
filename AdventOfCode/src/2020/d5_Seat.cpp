#include "Common.h"

SOLUTION(2020, 5) {
    constexpr RowCol FindSeat(std::string_view pass) {
        //F = Lower
        //B = Upper
        //L = Lower
        //R = Upper
        size_t minRow = 0;
        size_t maxRow = 127;
        size_t minCol = 0;
        size_t maxCol = 7;

        for (auto c : pass) {
            switch (c) {
            case 'F': maxRow = minRow + ((maxRow - minRow) / 2); break;
            case 'B': minRow = maxRow - ((maxRow - minRow) / 2); break;
            case 'L': maxCol = minCol + ((maxCol - minCol) / 2); break;
            case 'R': minCol = maxCol - ((maxCol - minCol) / 2); break;
            }
        }

        return { minRow, minCol };
    }

    constexpr size_t GetId(RowCol seat) {
        return (seat.Row * 8) + seat.Col;
    }

    constexpr std::vector<size_t> GetSeatIds(const auto& lines) {
        std::vector<size_t> result;
        std::transform(lines.begin(), lines.end(), std::back_inserter(result), [](std::string_view line) { return GetId(FindSeat(line)); });
        return result;
    }
    PART(1) {
        auto seatIds = GetSeatIds(lines);
        return *std::max_element(seatIds.begin(), seatIds.end());
    }

    PART(2) {
        auto seatIds = GetSeatIds(lines);
        std::sort(seatIds.begin(), seatIds.end());
        for (size_t i = 0; i < seatIds.size() - 1; i++) {
            if (seatIds[i] + 1 != seatIds[i + 1]) {
                return seatIds[i] + 1;
            }
        }

        throw "Not Found";
    }

    static_assert(FindSeat("FBFBBFFRLR") == RowCol{ 44, 5 });
    static_assert(FindSeat("FFFBBBFRRR") == RowCol{ 14, 7 });
    static_assert(FindSeat("BFFFBBFRRR") == RowCol{ 70, 7 });
    static_assert(FindSeat("BBFFBBFRLL") == RowCol{ 102, 4 });
    static_assert(FindSeat("FFFFFFFLLL") == RowCol{ 0, 0 });
    static_assert(FindSeat("BBBBBBBRRR") == RowCol{ 127, 7 });

    static_assert(GetId({ 44, 5 }) == 357);
}