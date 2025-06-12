#include <iostream>
#include <vector>
#include <optional>

namespace Matryoshka {
    constexpr size_t CostToCombine(const std::vector<size_t>& lhs, const std::vector<size_t>& rhs) {
        if (lhs.empty() || rhs.empty()) return 0;
        auto minLhs = *std::min_element(lhs.begin(), lhs.end());
        auto minRhs = *std::min_element(rhs.begin(), rhs.end());
        size_t result = lhs.size() + rhs.size();
        if (minLhs < minRhs) {
            result -= std::count_if(lhs.begin(), lhs.end(), [&](size_t v) { return v < minRhs; });
        }
        else {
            result -= std::count_if(rhs.begin(), rhs.end(), [&](size_t v) { return v < minLhs; });
        }
        return result;
    }

    constexpr size_t CostToCombine(const std::vector<size_t>& dolls, size_t lhsStart, size_t lhsSize, size_t rhsSize) {
        if (lhsSize == 0 || rhsSize == 0) return 0;
        size_t result = lhsSize + rhsSize;
        auto rhsStart = lhsStart + lhsSize;
        auto rhsEnd = rhsStart + rhsSize;
        auto minLhs = *std::min_element(dolls.begin() + lhsStart, dolls.begin() + rhsStart);
        auto minRhs = *std::min_element(dolls.begin() + rhsStart, dolls.begin() + rhsEnd);
        if (minLhs < minRhs) {
            result -= std::count_if(dolls.begin() + lhsStart, dolls.begin() + rhsStart, [&](size_t v) { return v < minRhs; });
        }
        else {
            result -= std::count_if(dolls.begin() + rhsStart, dolls.begin() + rhsEnd, [&](size_t v) { return v < minLhs; });
        }
        return result;
    }
    static_assert(CostToCombine({ 1, 2, 5 }, { 3, 4 }) == 3);
    static_assert(CostToCombine({ 1 }, { 2 }) == 1);
    static_assert(CostToCombine({ 1, 2, 5, 3, 4 }, 0, 3, 2) == 3);

    constexpr bool IsSetComplete(const std::vector<size_t>& dolls) {
        return *std::min_element(dolls.begin(), dolls.end()) == 1 &&
            *std::max_element(dolls.begin(), dolls.end()) == dolls.size();
    }

    // assume a b c d e
    // This can be produced by combining [a] and [bcde] or [ab] and [cde] or [abc] and [de] or [abcd] and [e]
    // [bcde] can be produced by combining [b] and [cde] or [bc] and [de] or [bcd] and [e]
    // [cde] can be produced by combining [c] and [de] or [cd] and [e]
    // We'll use recursion to solve this (dynamic programming)
    constexpr size_t CombineDolls(const std::vector<size_t>& dolls, size_t start, size_t end) {
        size_t dist = end - start;
        if (dist <= 1) return 0;
        if (dist == 2) return 1; // cost to combine any 2 singles will always be 1
        size_t result = 999;
        for (size_t count = 1; count < dist; count++) {
            //cost to combine is cost to create left + cost to create right + cost to combine left/right
            size_t leftCost = CombineDolls(dolls, start, start + count);
            size_t rightCost = CombineDolls(dolls, start + count, end);
            size_t combineCost = CostToCombine(dolls, start, count, dist - count);
            size_t totalCost = leftCost + rightCost + combineCost;
            result = std::min(result, totalCost);
        }
        return result;
    }

    static_assert(CombineDolls({ 1, 2, 3 }, 0, 3) == 2);
    static_assert(CombineDolls({ 2, 4, 1, 3 }, 0, 4) == 5);
    constexpr std::optional<std::vector<size_t>> FindNext(const std::vector<size_t>& all, size_t& idx) {
        std::vector<size_t> result;
        while (idx < all.size()) {
            if (std::find(result.begin(), result.end(), all[idx]) == result.end()) {
                result.emplace_back(all[idx++]);
            }
            else {
                break;
            }
        }
        if (IsSetComplete(result)) return result;
        return std::nullopt;
    }

    void Main() {
        //std::vector<size_t> all{ 1, 2, 3, 2, 4, 1, 3 };
        //std::vector<size_t> all{ 1, 2, 1, 2, 4, 3, 3 };
        std::vector<size_t> all{ 1, 2, 5, 3, 4 };
        size_t idx = 0;
        size_t total = 0;
        while (idx < all.size()) {
            auto next = FindNext(all, idx);
            if (!next.has_value()) {
                std::cout << "impossible";
                return;
            }
            total += CombineDolls(next.value(), 0, next.value().size());
        }
        std::cout << total;
    }
}