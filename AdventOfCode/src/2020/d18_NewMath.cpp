#include "Common.h"
#include "Core/Algorithms/ShuntingYard.h"
#include "Core/BigInt.h"

SOLUTION(2020, 18) {
    template<typename T>
    constexpr T Solve(const auto& lines, const Constexpr::SmallMap<std::string, ShuntingYard::OperatorData<T>>&operators) {
        T result = 0;
        for (const auto& line : lines) {
            result += ShuntingYard::Evaluate<T>(std::string(line), operators);
        }

        return result;
    }

    PART(1) {
        auto operators = ShuntingYard::Detail::DefaultOperatorData<BigInt>;
        for (auto& [key, value] : operators) {
            value.Precedence = 1;
        }

        return Solve(lines, operators).ToString();
    }

    PART(2) {
        auto operators = ShuntingYard::Detail::DefaultOperatorData<BigInt>;
        for (auto& [key, value] : operators) {
            value.Precedence = 3 - value.Precedence;
        }

        return Solve(lines, operators).ToString();
    }

    TEST(1) {
        auto operators = ShuntingYard::Detail::DefaultOperatorData<size_t>;
        for (auto& [key, value] : operators) {
            value.Precedence = 1;
        }

        if (ShuntingYard::Evaluate<size_t>("2 * 3 + (4 * 5)", operators) != 26) return false;
        if (ShuntingYard::Evaluate<size_t>("1 + (2 * 3) + (4 * (5 + 6))", operators) != 51) return false;
        if (ShuntingYard::Evaluate<size_t>("11 + (1 * (2 + 3)) * (3 + 4)", operators) != 112) return false;
        if (ShuntingYard::Evaluate<size_t>("5 + (8 * 3 + 9 + 3 * 4 * 3)", operators) != 437) return false;
        if (ShuntingYard::Evaluate<size_t>("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2", operators) != 13632) return false;
        if (ShuntingYard::Evaluate<size_t>("6 * 8 * (2 * 9) + 2 * 8 * 4", operators) != 27712) return false;

        return true;
    }


    TEST(2) {
        auto operators = ShuntingYard::Detail::DefaultOperatorData<size_t>;
        for (auto& [key, value] : operators) {
            value.Precedence = 3 - value.Precedence;
        }

        if (ShuntingYard::Evaluate<size_t>("1 + (2 * 3) + (4 * (5 + 6))", operators) != 51) return false;
        if (ShuntingYard::Evaluate<size_t>("2 * 3 + (4 * 5)", operators) != 46) return false;
        if (ShuntingYard::Evaluate<size_t>("5 + (8 * 3 + 9 + 3 * 4 * 3)", operators) != 1445) return false;
        if (ShuntingYard::Evaluate<size_t>("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))", operators) != 669060) return false;
        if (ShuntingYard::Evaluate<size_t>("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2", operators) != 23340) return false;

        return true;
    }
}