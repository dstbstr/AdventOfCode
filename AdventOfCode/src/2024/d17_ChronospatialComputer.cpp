#include "Common.h"

SOLUTION(2024, 17) {
	constexpr std::string Solve(u64 regA, u64 regB, u64 regC, const std::vector<u8>& instructions) {
		u64 ip = 0;

		//0-3 literal
		//4 reg A
		//5 reg B
		//6 reg C
		//7 invalid
		auto combo = [&](u64 val) -> u64 {
			if (val < 4) return val;
			switch (val) {
			case 4: return regA;
			case 5: return regB;
			case 6: return regC;
			}
			throw "Wat";
			};

		std::vector<u64> outputs;

		while (ip < instructions.size()) {
			auto op = instructions[ip];
			auto val = instructions[ip + 1];
			//adv = 0 (division reg A/2^Combo) truncated, written to reg A
			//bxl = 1 bitwise XOR b^Literal, written to B
			//bst = 2 Combo % 8, written to B
			//jnz = 3 jump if Reg A not zero, ip = literal (no increment)
			//bxc = 4 bitwise XOR b^c, stored in B (ignores operand)
			//out = 5 Combo % 8, written to output (separated by commas)
			//bdv = 6 division A/2^Combo written to RegB
			//cdv = 7 division A/2^Combo written to RegC

			switch (op) {
			break; case 0: regA = regA / (1ull << combo(val));
			break; case 1: regB = regB ^ val;
			break; case 2: regB = combo(val) % 8;
			break; case 3: if (regA != 0) ip = val - 2;
			break; case 4: regB = regB ^ regC;
			break; case 5: outputs.emplace_back(combo(val) % 8);
			break; case 6: regB = regA / (1ull << combo(val));
			break; case 7: regC = regA / (1ull << combo(val));
			}
			ip += 2;
		}

		return Constexpr::JoinVec(",", outputs);
	}

	PART(1) {
		auto s1 = Constexpr::Split(lines[0], ": ");
		auto s2 = Constexpr::Split(lines[1], ": ");
		auto s3 = Constexpr::Split(lines[2], ": ");

		u64 regA, regB, regC;
		Constexpr::ParseNumber(s1[1], regA);
		Constexpr::ParseNumber(s2[1], regB);
		Constexpr::ParseNumber(s3[1], regC);

		auto instructions = ParseLineAsNumbers<u8>(lines[4].substr(9));

		return Solve(regA, regB, regC, instructions);
	}
	PART(2) {
		auto instructions = ParseLineAsNumbers<u8>(lines[4].substr(9));
		auto target = Constexpr::JoinVec(",", instructions);
		u64 regA = 1ull << (target.size() * 3 / 2);

		auto result = Solve(regA, 0, 0, instructions);
		auto factor = 1.01;
		size_t offset = target.size() - 1;
		u64 prev = regA;
		while(offset > 6) {
			result = Solve(regA, 0, 0, instructions);
			if (result.substr(offset) != target.substr(offset)) {
				prev = regA;
				regA = static_cast<u64>(regA * factor);
			}
			else {
				regA = prev;
				factor = 1 + (factor - 1) * 0.1;
				offset -= 2;
			}
		}

		while(result != target) {
			regA++;
			result = Solve(regA, 0, 0, instructions);
		}

		return regA;
	}

	static_assert(Solve(10, 0, 0, { 5,0,5,1,5,4 }) == "0,1,2");
	static_assert(Solve(2024, 0, 0, { 0,1,5,4,3,0 }) == "4,2,5,6,7,7,7,7,3,1,0");
	static_assert(Solve(729, 0, 0, { 0,1,5,4,3,0 }) == "4,6,3,5,6,3,5,2,1,0");

	TEST(1) {
		return std::get<u64>(Part2({ "", "", "", "", "Program: 0,3,5,4,3,0" })) == 117440;
	}
}