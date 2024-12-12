#include "Common.h"

SOLUTION(2020, 14) {
    using Memory = Constexpr::SmallMap<u64, u64>;

    //mask = 110000011XX0000X101000X10X01XX001011
    constexpr void UpdateMasks(std::string_view str, u64 & zeroMask, u64& oneMask) {
        u64 value = 1;
        zeroMask = 0;
        oneMask = 0;
        for (auto it = str.rbegin(); it != str.rend(); it++) {
            if (*it == '1') oneMask += value;
            else if (*it == '0') zeroMask += value;
            value *= 2;
        }
    }

    constexpr void Write(u64 val, u64 address, Memory & memory, u64 zeroMask, u64 oneMask) {
        val &= ~zeroMask;
        val |= oneMask;
        memory[address] = val;
    }

    constexpr void UpdateFloaters(u64 address, u64 floating, Memory & memory, u64 toWrite) {
        if (floating == 0) return;

        std::vector<u64> seenValues{ 0ull };
        u64 power = 1;
        while (power <= floating) {
            if ((floating & power) == power) {
                std::vector<u64> toAdd;
                for (auto v : seenValues) {
                    toAdd.push_back(v + power);
                }
                std::copy(toAdd.begin(), toAdd.end(), std::back_inserter(seenValues));
            }
            power *= 2;
        }

        for (auto v : seenValues) {
            memory[address + v] = toWrite;
        }
    }

    constexpr void Write2(u64 val, u64 address, Memory & memory, u64 zeroMask, u64 oneMask) {
        address |= oneMask;
        u64 floating = ~(zeroMask | oneMask);
        u64 floatingMask = ((~0ull) >> (64 - 36));
        floating &= floatingMask; //strip out 1s above 36 bits
        address &= (~floating); //erase floating bits from address

        UpdateFloaters(address, floating, memory, val);
    }

    constexpr size_t Solve(const auto& lines, auto writeFunc) {
        u64 zeroMask = 0;
        u64 oneMask = 0;
        Memory memory{};
        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, " = ");
            if (split[0] == "mask") {
                UpdateMasks(split[1], zeroMask, oneMask);
            }
            else {
                u64 val, address;
                Constexpr::ParseNumber(split[1], val);
                Constexpr::ParseNumber(split[0].substr(3, split[0].size() - 4), address);
                writeFunc(val, address, memory, zeroMask, oneMask);
            }
        }

        u64 result = 0;
        for (const auto& [address, value] : memory) {
            result += value;
        }
        return result;
    }


    PART(1) {
        return Solve(lines, Write);
    }

    PART(2) {
        return Solve(lines, Write2);
    }

    /*
    11000011XX0000X101000X10X01XX001011
    00111100001111001011100101000110100 = 8084900404 (zeros)
    11000011000000010100001000100001011 = 26173116683 (ones)
    */
    constexpr bool TestUpdateMasks() {
        std::string mask = "11000011XX0000X101000X10X01XX001011";
        u64 zeroMask = 0;
        u64 oneMask = 0;

        UpdateMasks(mask, zeroMask, oneMask);
        if (zeroMask != 8084900404) return false;
        if (oneMask != 26173116683) return false;
        return true;
    }


    static_assert(TestUpdateMasks());
    TEST(1) {
        std::vector<std::string> lines = {
            "mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X",
            "mem[8] = 11",
            "mem[7] = 101",
            "mem[8] = 0"
        };

        return Solve(lines, Write) == 165;
    }

    TEST(2) {
        std::vector<std::string> lines = {
            "mask = 000000000000000000000000000000X1001X",
            "mem[42] = 100",
            "mask = 00000000000000000000000000000000X0XX",
            "mem[26] = 1"
        };
        return Solve(lines, Write2) == 208;
    }
    TEST(3) {
        std::vector<std::string> lines = {
            "mask = 000000000000000000000000000000000XXX",
            "mem[8] = 4",
            "mask = XX0000000000000000000000000000000000",
            "mem[0] = 5"
        };
        return Solve(lines, Write2) == 52;
    }
}