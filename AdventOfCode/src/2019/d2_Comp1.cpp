#include "Common.h"
#include "Comp.h"

SOLUTION(2019, 2) {

    PART(1) {
        auto instructions = ParseInstructions(lines[0]);
        instructions[1] = 12;
        instructions[2] = 2;

        Args args;
        while (Apply(instructions, args));

        return instructions[0];
    }

    PART(2) {
        auto initialInstructions = ParseInstructions(lines[0]);
        Args initialArgs;
        for (u64 noun = 0; noun < 100; noun++) {
            for (u64 verb = 0; verb < 100; verb++) {
                auto instructions = initialInstructions;
                auto args = initialArgs;
                instructions[1] = noun;
                instructions[2] = verb;
                while (Apply(instructions, args));

                if (instructions[0] == 19690720) {
                    return (noun * 100) + verb;
                }
            }
        }

        throw "No Solution Found";
    }

    TEST(1) {
        std::vector<s64> inst = { 1,1,1,4,99,5,6,0,99 };
        Args args;
        while (Apply(inst, args));
        return inst[0] == 30;
    }
}