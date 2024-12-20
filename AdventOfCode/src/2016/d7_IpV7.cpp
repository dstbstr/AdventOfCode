#include "Common.h"

SOLUTION(2016, 7) {
    constexpr bool ContainsAbba(std::string_view str) {
        for (size_t i = 0; i < str.size() - 3; i++) {
            if (str[i] == str[i + 3] && str[i + 1] == str[i + 2] && str[i] != str[i + 1]) {
                return true;
            }
        }

        return false;
    }

    constexpr std::vector<std::string> GetThreeLetterSequences(const std::string& str) {
        std::vector<std::string> result;

        for (size_t i = 0u; i < str.size() - 2; i++) {
            if (str[i] == str[i + 2] && str[i + 1] != str[i]) {
                result.emplace_back(str.substr(i, 3));
            }
        }

        return result;
    }

    constexpr void ParseLine(std::string_view line, std::vector<std::string>& outOfBrackets, std::vector<std::string>& inBrackets) {
        bool outsideBrackets = true;
        size_t index = 0;
        std::string str = "";

        while (index < line.size()) {
            if (outsideBrackets) {
                str = line.substr(index, line.find('[', index) - index);
                outOfBrackets.emplace_back(str);
                if (line.find('[', index) == line.npos) {
                    break;
                }
                index = line.find('[', index) + 1;
                outsideBrackets = false;
            }
            else {
                str = line.substr(index, line.find(']', index) - index);
                inBrackets.emplace_back(str);
                if (line.find(']', index) == line.npos) {
                    break;
                }
                index = line.find(']', index) + 1;
                outsideBrackets = true;
            }
        }
    }

    constexpr bool IsValid(std::string_view line) {
        std::vector<std::string> inBrackets;
        std::vector<std::string> outOfBrackets;
        ParseLine(line, outOfBrackets, inBrackets);

        for (const auto& str : inBrackets) {
            if (ContainsAbba(str)) {
                return false;
            }
        }

        for (const auto& str : outOfBrackets) {
            if (ContainsAbba(str)) {
                return true;
            }
        }

        return false;
    }

    constexpr std::string InvertThreeLetters(const std::string & code) {
		return { code[1], code[0], code[1] };
    }

    constexpr bool SupportsSsl(std::string_view line) {
        std::vector<std::string> inBrackets;
        std::vector<std::string> outOfBrackets;
        ParseLine(line, outOfBrackets, inBrackets);

        std::vector<std::string> toFind;
        toFind.reserve(inBrackets.size());

        for (const auto& str : inBrackets) {
            for (const auto& bab : GetThreeLetterSequences(str)) {
                toFind.push_back(InvertThreeLetters(bab));
            }
        }

        for (const auto& str : outOfBrackets) {
            for (const auto& aba : GetThreeLetterSequences(str)) {
                if (std::find(toFind.cbegin(), toFind.cend(), aba) != toFind.cend()) {
                    return true;
                }
            }
        }

        return false;
    }

    PART(1) {
        return std::count_if(lines.begin(), lines.end(), IsValid);
    }
    PART(2) {
        return std::count_if(lines.begin(), lines.end(), SupportsSsl);
    }

    static_assert(ContainsAbba("abba"));
    static_assert(ContainsAbba("xyyx"));
    static_assert(!ContainsAbba("mnop"));
    static_assert(!ContainsAbba("aaaa"));
    static_assert(ContainsAbba("ioxxoj"));
        
    static_assert(GetThreeLetterSequences({ "zazbz" }).size() == 2);
    static_assert(GetThreeLetterSequences({ "zazbz" })[0] == "zaz");
    static_assert(GetThreeLetterSequences({ "zazbz" })[1] == "zbz");
        
    static_assert(InvertThreeLetters("aba") == "bab");

    static_assert(IsValid("abba[mnop]qrst"));
    static_assert(!IsValid("abcd[bddb]xyyx"));
    static_assert(!IsValid("aaaa[qwer]tyui"));
    static_assert(IsValid("ioxxoj[asdfgh]zxcvbn"));
        
    static_assert(SupportsSsl("aba[bab]xyz"));
    static_assert(!SupportsSsl("xyx[xyx]xyx"));
    static_assert(SupportsSsl("aaa[kek]eke"));
    static_assert(SupportsSsl("zazbz[bzb]cdb"));
}