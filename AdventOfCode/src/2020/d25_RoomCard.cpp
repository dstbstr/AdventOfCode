#include "Common.h"

SOLUTION(2020, 25) {
    constexpr u64 RunLoop(u64 value, u64 subject) {
        value *= subject;
        return value % 20201227;
    }

    constexpr u64 FindLoopSize(u64 publicKey) {
        u64 value = 1;
        u64 subject = 7;
        u64 loopCount = 0;
        while (value != publicKey) {
            value = RunLoop(value, subject);
            loopCount++;
        }

        return loopCount;
    }

    constexpr u64 Decrypt(size_t pk1, size_t pk2) {
        auto loopSize = FindLoopSize(pk1);
        u64 value = 1;
        for (size_t i = 0u; i < loopSize; i++) {
            value = RunLoop(value, pk2);
        }

        return value;
    }


    PART(1) {
        auto values = ParseLinesAsNumbers<size_t>(lines);
        return Constexpr::ToString(Decrypt(values[0], values[1]));
    }

    constexpr bool RunTest() {
        u64 value = 1;
        u64 subject = 7;
        for (auto i = 0; i < 8; i++) {
            value = RunLoop(value, subject);
        }
        if (value != 5764801) return false;

        value = 1;
        for (auto i = 0; i < 11; i++) {
            value = RunLoop(value, subject);
        }
        if (value != 17807724) return false;
        return true;
    }

    static_assert(RunTest());
    static_assert(FindLoopSize(5764801) == 8);
    static_assert(FindLoopSize(17807724) == 11);
    static_assert(Decrypt(5764801, 17807724) == 14897079);
}