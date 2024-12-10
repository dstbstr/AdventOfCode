#include "Common.h"

SOLUTION(2020, 15) {
    template<u32 Rounds>
    constexpr u32 Solve(const std::string& input) {
        auto nums = ParseLineAsNumbers<u32>(input);
        u32 pos = 0;
        auto last = nums.back();
        auto positions = std::make_unique<std::array<u32, Rounds>>();
        for (auto num : nums) {
            (*positions)[num] = ++pos;
        }
        while (pos < Rounds) {
            auto lastPos = (*positions)[last];
            //auto nextNum = lastPos == 0 ? 0 : pos - lastPos;
            auto nextNum = (pos - lastPos) * (lastPos != 0);
            (*positions)[last] = pos++;
            last = nextNum;
        }

        return last;
    }

    PART(1) {
		return Constexpr::ToString(Solve<2020>(lines[0]));
    }

    PART(2) {
		return Constexpr::ToString(Solve<30'000'000>(lines[0]));
    }

    static_assert(Solve<2020>("0,3,6") == 436);
	static_assert(Solve<2020>("1,3,2") == 1);

    TEST(1) {
		return Solve<30'000'000>("0,3,6") == 175594;
    }
}