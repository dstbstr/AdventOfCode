#include "Common.h"

SOLUTION(2016, 18) {
    constexpr size_t CountSafeTiles(std::string_view initial, u32 rowCount) {
        std::vector<bool> a;
        a.emplace_back(false);
        std::transform(initial.begin(), initial.end(), std::back_inserter(a), [](u8 c) { return c == '^'; });
        a.emplace_back(false);

        std::vector<bool> b(a.size());
        size_t safe = std::count(a.begin() + 1, a.end() - 1, false);
        for (u32 row = 1; row < rowCount; row++) {
            for (size_t i = 1; i < a.size() - 1; i++) {
                b[i] = a[i - 1] ^ a[i + 1];
            }
            safe += std::count(b.begin() + 1, b.end() - 1, false);
            a.swap(b);
        }
        return safe;
    }

    PART(1) {
        return Constexpr::ToString(CountSafeTiles(lines[0], 40));
    }
    PART(2) {
        return Constexpr::ToString(CountSafeTiles(lines[0], 400'000));
    }

    TEST(1) {
        static_assert(CountSafeTiles(".^^.^.^^^^", 10) == 38);

        return true;
    }
}

