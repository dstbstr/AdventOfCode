#include "Common.h"
SOLUTION(2021, 16) {

    /*
    Packet:
        Version:3
        Type:3
            Literal (4)
                Values[]:5
            Operator (!4)
                Indicator:1
                    BitCount(0)
                        Length:15
                    PacketCount(1)
                        Count:11
    */

    constexpr std::array<const char*, 16> Bits = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };

    constexpr std::string HexToBits(std::string_view hex) {
        std::string result;
        result.reserve(hex.size() * 4);
        for (auto c : hex) {
            if (c >= '0' && c <= '9') {
                result += Bits[c - '0'];
            }
            else {
                result += Bits[c - 'A' + 10];
            }
        }
        return result;
    }

    template<typename T>
    constexpr T BitsToValue(std::string_view bits) {
        T result = 0;
        T multiplier = 1;
        for (int i = static_cast<int>(bits.size() - 1); i >= 0; i--) {
            result += (bits[i] - '0') * multiplier;
            multiplier *= 2;
        }
        return result;
    }

    struct Packet {
        u8 Version = 0;
        u8 Type = 0;
        u64 Value = 0;
        std::vector<Packet> SubPackets;

        constexpr u64 Evaluate() const {
            u64 result = 0;
            switch (Type) {
            case 0: { //Sum
                for (const auto& p : SubPackets) {
                    result += p.Evaluate();
                }
                break;
            }
            case 1: { //Product
                result = 1;
                for (const auto& p : SubPackets) {
                    result *= p.Evaluate();
                }
                break;
            }
            case 2: { //Min
                result = std::numeric_limits<s64>::max();
                for (const auto& p : SubPackets) {
                    result = std::min(result, p.Evaluate());
                }
                break;
            }
            case 3: { //Max
                for (const auto& p : SubPackets) {
                    result = std::max(result, p.Evaluate());
                }
                break;
            }
            case 4: result = Value; break;
            case 5: { //Greater
                result = SubPackets[0].Evaluate() > SubPackets[1].Evaluate() ? 1 : 0;
                break;
            }
            case 6: { //Less
                result = SubPackets[0].Evaluate() < SubPackets[1].Evaluate() ? 1 : 0;
                break;
            }
            case 7: { //Equal
                result = SubPackets[0].Evaluate() == SubPackets[1].Evaluate() ? 1 : 0;
                break;
            }
            default: throw "Unknown Packet Type";
            }
            return result;
        }
    };

    constexpr u64 ParseValue(std::string_view bits, size_t & index) {
        std::string bitString;
        while (true) {
            auto chunk = bits.substr(index, 5);
            index += 5;
            bitString += chunk.substr(1);
            if (chunk[0] == '0') break;
        }
        return BitsToValue<u64>(bitString);
    }

    constexpr Packet ParsePacket(const std::string & bits, size_t & index) {
        Packet p;
        p.Version = BitsToValue<u8>(bits.substr(index, 3));
        index += 3;
        p.Type = BitsToValue<u8>(bits.substr(index, 3));
        index += 3;

        if (p.Type == 4) {
            p.Value = ParseValue(bits, index);
        }
        else {
            auto lengthTypeId = bits[index];
            index++;
            if (lengthTypeId == '0') {
                auto lengthInBits = BitsToValue<u32>(bits.substr(index, 15));
                index += 15;
                auto startingBits = index;
                while (index - startingBits != lengthInBits) {
                    p.SubPackets.push_back(ParsePacket(bits, index));
                }
            }
            else {
                auto packetCount = BitsToValue<u32>(bits.substr(index, 11));
                index += 11;
                for (u32 i = 0; i < packetCount; i++) {
                    p.SubPackets.push_back(ParsePacket(bits, index));
                }
            }
        }

        return p;
    }

    constexpr Packet GetPacket(std::string_view line) {
        auto bits = HexToBits(line);
        size_t index = 0;
        return ParsePacket(bits, index);
    }

    constexpr size_t SolvePartOne(std::string_view line) {
        auto packet = GetPacket(line);

        size_t result = 0;
        std::vector<Packet> remaining;
        remaining.push_back(packet);
        while (!remaining.empty()) {
            auto current = remaining.back();
            remaining.pop_back();
            result += current.Version;
            std::copy(current.SubPackets.begin(), current.SubPackets.end(), std::back_inserter(remaining));
        }

        return result;
    }

    PART(1) {
        return SolvePartOne(lines[0]);
    }

    PART(2) {
        auto packet = GetPacket(lines[0]);
        return packet.Evaluate();
    }

    constexpr bool TestParseValue() {
        size_t index = 0;
        auto result = ParseValue("101111111000101", index);
        return result == 2021;
    }

    constexpr bool TestParsePacket() {
        std::string bits = "110100101111111000101000";
        size_t index = 0;
        auto packet = ParsePacket(bits, index);
        if (packet.Version != 6) return false;
        if (packet.Type != 4) return false;
        if (packet.Value != 2021) return false;
        if (!packet.SubPackets.empty()) return false;

        return true;
    }

    static_assert(HexToBits("3") == "0011");
    static_assert(HexToBits("A") == "1010");
    static_assert(HexToBits("D2FE28") == "110100101111111000101000");

    static_assert(BitsToValue<u8>("0011") == u8(3));
    static_assert(BitsToValue<u32>("1111") == u32(15));

    static_assert(TestParseValue());

    static_assert(TestParsePacket());
        
    static_assert(SolvePartOne("8A004A801A8002F478") == 16);
    static_assert(SolvePartOne("620080001611562C8802118E34") == 12);
    static_assert(SolvePartOne("C0015000016115A2E0802F182340") == 23);
    static_assert(SolvePartOne("A0016C880162017C3686B18A3D4780") == 31);
 
    static_assert(GetPacket("C200B40A82").Evaluate() == 3);
    static_assert(GetPacket("04005AC33890").Evaluate() == 54);
    static_assert(GetPacket("880086C3E88112").Evaluate() == 7);
    static_assert(GetPacket("CE00C43D881120").Evaluate() == 9);
    static_assert(GetPacket("D8005AC2A8F0").Evaluate() == 1);
    static_assert(GetPacket("F600BC2D8F").Evaluate() == 0);
    static_assert(GetPacket("9C005AC2F8F0").Evaluate() == 0);
    static_assert(GetPacket("9C0141080250320F1802104A08").Evaluate() == 1);
}