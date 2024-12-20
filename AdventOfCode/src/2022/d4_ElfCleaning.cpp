#include "Common.h"

SOLUTION(2022, 4) {
    struct Section {
        u32 Start;
        u32 End;
    };

    constexpr bool FullyContains(Section lhs, Section rhs) {
        if (lhs.Start == rhs.Start) {
            return true;
        }
        if (lhs.Start < rhs.Start) {
            return rhs.Start > lhs.Start && rhs.End <= lhs.End;
        }
        else {
            return lhs.Start > rhs.Start && lhs.End <= rhs.End;
        }
    }

    constexpr bool Overlap(Section lhs, Section rhs) {
        if (lhs.Start == rhs.Start || lhs.End == rhs.End) {
            return true;
        }
        if (lhs.Start < rhs.Start) {
            return lhs.End >= rhs.Start;
        }
        else {
            return rhs.End >= lhs.Start;
        }
    }

    constexpr std::pair<Section, Section> ParseLine(std::string_view line) {
        auto s = Constexpr::Split(line, ",");
        auto a = Constexpr::Split(s[0], "-");
        auto b = Constexpr::Split(s[1], "-");
        Section lhs, rhs;
        Constexpr::ParseNumber(a[0], lhs.Start);
        Constexpr::ParseNumber(a[1], lhs.End);
        Constexpr::ParseNumber(b[0], rhs.Start);
        Constexpr::ParseNumber(b[1], rhs.End);
        return std::make_pair(lhs, rhs);
    }

    constexpr size_t RunImpl(const auto& lines, auto func) {
        auto pairs = ParseLines(lines, ParseLine);
        return std::count_if(pairs.begin(), pairs.end(), [func](const auto& p) {
            return func(p.first, p.second);
            });
    }

    PART(1) {
        return RunImpl(lines, FullyContains);
    }

    PART(2) {
        return RunImpl(lines, Overlap);
    }

    static_assert(FullyContains({ 6, 6 }, { 4, 6 }));
    static_assert(FullyContains({ 4, 6 }, { 6, 6 }));
    static_assert(FullyContains({ 2, 8 }, { 3, 7 }));
    static_assert(FullyContains({ 3, 7 }, { 2, 8 }));
    static_assert(!FullyContains({ 2, 4 }, { 6, 8 }));
    static_assert(!FullyContains({ 2, 6 }, { 4, 8 }));
    static_assert(FullyContains({ 55, 55 }, { 55, 83 }));
        
    static_assert(!Overlap({ 2, 4 }, { 6, 8 }));
    static_assert(!Overlap({ 2, 3 }, { 4, 5 }));
    static_assert(Overlap({ 5, 7 }, { 7, 9 }));
    static_assert(Overlap({ 2, 8 }, { 3, 7 }));
    static_assert(Overlap({ 6, 6 }, { 4, 6 }));
    static_assert(Overlap({ 2, 6 }, { 4, 8 }));
    static_assert(Overlap({ 55, 55 }, { 55, 83 }));
}