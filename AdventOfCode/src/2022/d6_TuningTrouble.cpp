#include "Common.h"

SOLUTION(2022, 6) {
    enum struct MessageType { Signal = 4, Message = 14};

    constexpr size_t FindUnique(std::string_view input, MessageType messageType) {
        std::vector<char> allChars;
        size_t runLength = static_cast<size_t>(messageType);
        for (size_t i = 0; i < input.size() - runLength; i++) {
            allChars.clear();
            for (size_t j = 0u; j < runLength; j++) {
                if (std::find(allChars.begin(), allChars.end(), input[i + j]) == allChars.end()) {
                    allChars.push_back(input[i + j]);
                }
                else {
                    break;
                }
            }
            if (allChars.size() == runLength) {
                return i + runLength;
            }
        }

        return 0;
    }

    PART(1) {
        return Constexpr::ToString(FindUnique(lines[0], MessageType::Signal));
    }

    PART(2) {
        return Constexpr::ToString(FindUnique(lines[0], MessageType::Message));
    }

    static_assert(FindUnique("bvwbjplbgvbhsrlpgdmjqwftvncz", MessageType::Signal) == 5);
    static_assert(FindUnique("nppdvjthqldpwncqszvftbrmjlhg", MessageType::Signal) == 6);
    static_assert(FindUnique("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", MessageType::Signal) == 10);
    static_assert(FindUnique("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", MessageType::Signal) == 11);

    static_assert(FindUnique("mjqjpqmgbljsphdztnvjfqwrcgsmlb", MessageType::Message) == 19);

    TEST(1) {
        return FindUnique("bvwbjplbgvbhsrlpgdmjqwftvncz", MessageType::Signal) == 5;
    }
    TEST(2) {
        return FindUnique("nppdvjthqldpwncqszvftbrmjlhg", MessageType::Signal) == 6;
    }
    TEST(3) {
        return FindUnique("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", MessageType::Signal) == 10;
    }
    TEST(4) {
        return FindUnique("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", MessageType::Signal) == 11;
    }
    TEST(5) {
        return FindUnique("mjqjpqmgbljsphdztnvjfqwrcgsmlb", MessageType::Message) == 19;
    }
}