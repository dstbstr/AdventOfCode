#include "Common.h"

SOLUTION(2020, 23) {
    constexpr std::vector<u32> GetCups(std::string_view line, u64 max) {
        //389125467
        //cups[0] = 3
        //cups[1] = 2
        //cups[2] = 5
        //cups[3] = 8
        //cups[4] = 6
        //cups[5] = 4
        //cups[6] = 7
        //cups[7] = 3
        //cups[8] = 9
        //cups[9] = 1

        std::vector<u32> cups;
        cups.resize(max + 1);
        u32 first = line[0] - '0';
        u32 prev = 0;
        u32 next = 0;
        cups[0] = first;
        for (size_t i = 1u; i < line.size(); i++) {
            prev = line[i - 1] - '0';
            next = line[i] - '0';
            cups[prev] = next;
        }

        prev = next;
        next = static_cast<u32>(line.size()) + 1;
        
        while (next <= max) {
            cups[prev] = next;
            prev = next++;
        }
        cups[prev] = cups[0];
        
        return cups;
    }

    constexpr void RunRounds(std::vector<u32>& cups, size_t rounds) {
        auto max = static_cast<u32>(cups.size() - 1);
        u32 r1, r2, r3, dest, after;

        for (size_t round = 0u; round < rounds; round++) {
            r1 = cups[cups[0]];
            r2 = cups[r1];
            r3 = cups[r2];
            dest = cups[0] - 1;
            while (dest == r1 || dest == r2 || dest == r3 || dest == 0) {
                if (dest == 0) dest = max;
                else dest--;
            }

            cups[cups[0]] = cups[r3];
            after = cups[dest];
            cups[dest] = r1;
            cups[r3] = after;
            cups[0] = cups[cups[0]];
        }

    }

    constexpr std::string SolvePartOne(std::string_view line) {
        auto cups = GetCups(line, line.size());
        RunRounds(cups, 100);

        std::string result = "";
        size_t index = 1;
        while (cups[index] != 1) {
            result += (static_cast<char>('0' + cups[index]));
            index = cups[index];
        }
        return result;
    }

    PART(1) {
        return SolvePartOne(lines[0]);
    }

    constexpr u64 SolvePartTwo(std::string_view line) {
        auto cups = GetCups(line, 1'000'000);
        RunRounds(cups, 10'000'000);

        return static_cast<u64>(cups[1]) * static_cast<u64>(cups[cups[1]]);
    }

    PART(2) {
        return SolvePartTwo(lines[0]);
    }

	//static_assert(SolvePartOne("389125467") == "67384529");
	//static_assert(SolvePartTwo("389125467") == 149245887792);

    
    TEST(1) {
        std::string line = "389125467";

        if (SolvePartOne( line ) != "67384529") return false;
        if (SolvePartTwo( line ) != 149245887792) return false;

        return true;
    }
    
}