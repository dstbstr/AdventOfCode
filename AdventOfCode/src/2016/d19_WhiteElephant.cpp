#include "Common.h"

SOLUTION(2016, 19) {
    constexpr double ln3 = 1.09861228866810969;
    constexpr double lnInput = 14.91818458658705;

    constexpr u32 FindLeftWinner(u32 playerCount) {
        auto power = Constexpr::Log2I(playerCount);
        auto remainder = playerCount - Constexpr::Pow(2u, power);
        return static_cast<u32>(2 * remainder + 1);
    }

    constexpr u32 FindAcrossWinner(u32 playerCount) {
        //auto pow = static_cast<u32>(std::floor(std::log(playerCount) / std::log(3)));
        auto pow = static_cast<u32>(lnInput / ln3);
        auto b = Constexpr::Pow(3u, pow);
        
        if (playerCount == b) return playerCount;
        if (playerCount - b <= b) return playerCount - b;
        return 2 * playerCount - 3 * b;
    }

    PART(1) {
        u32 playerCount;
        Constexpr::ParseNumber(lines[0], playerCount);
        return FindLeftWinner(playerCount);
    }
    PART(2) {
        u32 playerCount;
        Constexpr::ParseNumber(lines[0], playerCount);
        return FindAcrossWinner(playerCount);
    }

    static_assert(FindLeftWinner(5) == 3);
}