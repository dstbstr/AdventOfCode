#include "Common.h"
#include "Comp.h"

SOLUTION(2019, 9) {
    constexpr s64 Solve(std::string_view line, s64 input) {
        auto instructions = ParseInstructions(line);
        Args args;
        args.Inputs.push_back(input);

        while (Apply(instructions, args));

        return args.Output;
    }

    PART(1) {
        return Solve(lines[0], 1);
    }

    PART(2) {
        return Solve(lines[0], 2);
    }
}