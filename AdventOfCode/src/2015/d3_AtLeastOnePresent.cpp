#include "Common.h"
#include "Facing.h"

SOLUTION(2015, 3) {
    constexpr size_t Solve(std::string_view input, size_t moverCount) {
        Constexpr::BigSet<Coord, 10'000> seen;
        seen.SetSentinel({ 9999, 9999 });
		std::vector<Coord> movers(moverCount, { 0, 0 });
        size_t c = 0;
        seen.insert({ 0, 0 });
        while(c < input.size()) {
            for(auto& mover : movers) {
                Move(mover, ParseFacing(input[c++]));
                seen.insert(mover);
                if(c >= input.size()) break;
            }
        }
        return seen.size();
    }

    PART(1) {
        return Solve(lines[0], 1);
    }

    PART(2) {
        return Solve(lines[0], 2);
    }

    static_assert(Solve(">", 1) == 2);
	static_assert(Solve("^>v<", 1) == 4);

    static_assert(Solve("^v", 2) == 3);
    static_assert(Solve("^>v<", 2) == 3);
    static_assert(Solve("^v^v^v^v^v", 2) == 11);
}