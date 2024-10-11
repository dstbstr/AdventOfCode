#include "Common.h"
#include "Comp.h"

SOLUTION(2019, 5) {
    constexpr s64 Solve(std::string_view line, s64 input) {
        auto code = ParseInstructions(line);
        Args args;
        args.Inputs.push_back(input);

        while (Apply(code, args));
        return args.Output;
    }

    PART(1) {
        return Constexpr::ToString(Solve(lines[0], 1));
    }

    PART(2) {
        return Constexpr::ToString(Solve(lines[0], 5));
    }

    TEST(1) {
        return true;
    }
}