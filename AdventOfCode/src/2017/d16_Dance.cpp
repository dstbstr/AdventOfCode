#include "Common.h"
#include "Core/Algorithms/FloydCycle.h"

SOLUTION(2017, 16) {
    static constexpr auto InitialDancers = "abcdefghijklmnop";

    constexpr void Spin(std::string & dancers, std::string_view amountStr) {
        u32 amount;
        Constexpr::ParseNumber(amountStr, amount);
        std::rotate(dancers.rbegin(), dancers.rbegin() + amount, dancers.rend());
    }

    constexpr void Exchange(std::string & dancers, std::string_view details) {
        auto split = Constexpr::Split(details, "/");
        size_t lhs;
        size_t rhs;
        Constexpr::ParseNumber(split[0], lhs);
        Constexpr::ParseNumber(split[1], rhs);

        std::swap(dancers[lhs], dancers[rhs]);
    }

    constexpr void Partner(std::string & dancers, std::string_view details) {
        auto split = Constexpr::Split(details, "/");
        size_t lhs = dancers.find(split[0]);
        size_t rhs = dancers.find(split[1]);

        std::swap(dancers[lhs], dancers[rhs]);
    }

    constexpr void ApplyMove(std::string & dancers, std::string_view move) {
        auto type = move[0];
        auto details = move.substr(1);
        switch (type) {
        case 's': Spin(dancers, details); break;
        case 'x': Exchange(dancers, details); break;
        case 'p': Partner(dancers, details); break;
        default: break;
        }
    }

    constexpr auto GenMove(std::string_view move) {
        auto type = move[0];
        auto details = move.substr(1);
        size_t v1{0}, v2{0};
        char d1{' '}, d2{' '};
        if (type == 's') {
            Constexpr::ParseNumber(details, v1);
        }
        else if (type == 'x') {
            auto s = Constexpr::Split(details, "/");
            Constexpr::ParseNumber(s[0], v1);
            Constexpr::ParseNumber(s[1], v2);
        }
        else if (type == 'p') {
            auto s = Constexpr::Split(details, "/");
            d1 = s[0][0];
            d2 = s[1][0];
        }
		else throw "Wat?";

        return [type, v1, v2, d1, d2](std::string& dancers) {
            switch (type) {
            case 's': std::rotate(dancers.rbegin(), dancers.rbegin() + v1, dancers.rend()); break;
            case 'x': std::swap(dancers[v1], dancers[v2]); break;
            case 'p': std::swap(dancers[dancers.find(d1)], dancers[dancers.find(d2)]); break;
            }
        };
    }

    constexpr auto GetMoves(std::string_view line) {
        return line 
            | std::views::split(',')
			| std::views::transform([](auto s) { return std::string_view(s.begin(), s.end()); })
			| std::views::transform(GenMove)
			| std::ranges::to<std::vector>();
    }

    constexpr std::string Dance(std::string dancers, const auto& moves) {
        for (const auto& move : moves) {
            move(dancers);
        }
        return dancers;
    }
    PART(1) {
        auto moves = GetMoves(lines[0]);

        std::string dancers = InitialDancers;
        for (const auto& move : moves) {
            move(dancers);
        }

        return dancers;
    }

    PART(2) {
        auto moves = GetMoves(lines[0]);
        std::string dancers = InitialDancers;
        u32 cycleLength, cycleStart;
        FloydCycle::FindCycle(dancers, [&](std::string d) { return Dance(d, moves); }, cycleLength, cycleStart);

        u32 cycles = 1'000'000'000 % cycleLength;
        for (auto i = 0u; i < cycles; i++) {
            dancers = Dance(dancers, moves);
        }
        return dancers;
    }

    TEST(1) {
        std::string dancers = "abcde";
        std::string line = "s1,x3/4,pe/b";
        Spin(dancers, "1");
        Exchange(dancers, "3/4");
        Partner(dancers, "e/b");
        if (dancers != "baedc") return false;

        dancers = "abcde";

        Spin(dancers, "3");
        if (dancers != "cdeab") return false;

        return true;
    }
}