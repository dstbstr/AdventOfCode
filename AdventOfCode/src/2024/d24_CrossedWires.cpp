#include "Common.h"
#include "Core/BigInt.h"

SOLUTION(2024, 24) {
	using System = Constexpr::BigMap<std::string, bool, 10'000>;

	constexpr System GetInitialState(const std::vector<std::string>& lines) {
		System system;
		for(const auto& line : lines) {
			auto s = Constexpr::Split(line, ": ");
			system[std::string(s[0])] = s[1] == "1";
		}

		return system;
	}

	constexpr bool TryEval(System& system, const std::string& wire) {
		auto s = Constexpr::Split(wire, " ");
		auto lhs = std::string(s[0]);
		auto rhs = std::string(s[2]);
		if (!system.contains(lhs)) return false;
		if (!system.contains(rhs)) return false;
		auto op = std::string(s[1]);
		auto& output = system[std::string(s[4])];
		switch (s[1][0]) {
		case 'A': output = system[lhs] && system[rhs]; break;
		case 'O': output = system[lhs] || system[rhs]; break;
		case 'X': output = system[lhs] ^ system[rhs]; break;
		}
		return true;
	}

	constexpr void RunSystem(System& system, const std::vector<std::string>& wires) {
		std::vector<std::string> remaining = wires;
		std::vector<std::string> next;
		while(!remaining.empty()) {
			for (const auto& wire : remaining) {
				if (!TryEval(system, wire)) {
					next.push_back(wire);
				}
			}
			std::swap(remaining, next);
			next.clear();
		}
	}

	constexpr u64 GetBinary(const System& system, char c) {
		std::vector<std::pair<u32, bool>> outputs;
		for (const auto& [key, value] : system.GetAllEntries()) {
			if (key[0] == c) {
				u32 num;
				Constexpr::ParseNumber(key.substr(1), num);
				outputs.emplace_back(num, value);
			}
		}
		std::sort(outputs.begin(), outputs.end(), [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });
		std::reverse(outputs.begin(), outputs.end());
		auto binary = std::views::transform(outputs, [](const auto& val) { return val.second ? '1' : '0'; })
			| std::ranges::to<std::string>();
		return BigInt::FromBinary(binary).to_ull();
	}

	constexpr u64 SolvePart1(const std::vector<std::string>& lines) {
		auto groups = SplitInputIntoGroups(lines);
		auto system = GetInitialState(groups[0]);
		RunSystem(system, groups[1]);
		return GetBinary(system, 'z');
	}

	PART(1) {
		return SolvePart1(lines);
	}

	struct Wire {
		std::string i1, i2, op, o;
		constexpr bool operator==(const Wire& other) const {
			return i1 == other.i1 && i2 == other.i2 && op == other.op && o == other.o;
		}

		constexpr Wire(std::string_view line) {
			auto s = Constexpr::Split(line, " ");
			i1 = std::min(s[0], s[2]);
			i2 = std::max(s[0], s[2]);
			op = std::string(s[1]);
			o = std::string(s[4]);
		}
	};

	//lovingly borrowed
	constexpr std::string SolvePart2(const std::vector<std::string>& lines) {
		auto groups = SplitInputIntoGroups(lines);
		auto wires = groups[1]
			| std::views::transform([](const auto& line) { return Wire(line); })
			| std::ranges::to<std::vector>();
		std::vector<std::string> result;
		for(const auto& wire : wires) {
			//what does the z45 represent?
			if(wire.o[0] == 'z' && wire.op != "XOR" && wire.o != "z45") {
				result.push_back(wire.o);
			}
			if(wire.o[0] != 'z' && wire.op == "XOR" && wire.i1[0] !='x') {
				result.push_back(wire.o);
			}
			if(wire.op == "AND" && wire.i1 != "x00") {
				auto feeds = wires | std::views::filter([&wire](const auto& w) { return w.i1 == wire.o || w.i2 == wire.o; });
				for(const auto& feed : feeds) {
					if(feed.op != "OR") {
						result.push_back(wire.o);
						break;
					}
				}
			}
			if(wire.op == "OR") {
				auto lhs = std::find_if(wires.begin(), wires.end(), [&wire](const auto& w) { return w.o == wire.i1; });
				if(lhs->op != "AND") {
					result.push_back(lhs->o);
				}
				auto rhs = std::find_if(wires.begin(), wires.end(), [&wire](const auto& w) { return w.o == wire.i2; });
				if (rhs->op != "AND") {
					result.push_back(rhs->o);
				}
			}
		}
		std::sort(result.begin(), result.end());
		result.erase(std::unique(result.begin(), result.end()), result.end());
		return Constexpr::JoinVec(",", result);
	}

	PART(2) {
		return SolvePart2(lines);
	}
	TEST(1) {
		std::vector<std::string> lines = {
			"x00: 1",
			"x01: 1",
			"x02: 1",
			"y00: 0",
			"y01: 1",
			"y02: 0",
			"",
			"x00 AND y00 -> z00",
			"x01 XOR y01 -> z01",
			"x02 OR y02 -> z02"
		};

		return SolvePart1(lines) == 4;
	}

	TEST(2) {
		std::vector<std::string> lines = {
			"x00: 1",
			"x01: 0",
			"x02: 1",
			"x03: 1",
			"x04: 0",
			"y00: 1",
			"y01: 1",
			"y02: 1",
			"y03: 1",
			"y04: 1",
			"",
			"ntg XOR fgs -> mjb",
			"y02 OR x01 -> tnw",
			"kwq OR kpj -> z05",
			"x00 OR x03 -> fst",
			"tgd XOR rvg -> z01",
			"vdt OR tnw -> bfw",
			"bfw AND frj -> z10",
			"ffh OR nrd -> bqk",
			"y00 AND y03 -> djm",
			"y03 OR y00 -> psh",
			"bqk OR frj -> z08",
			"tnw OR fst -> frj",
			"gnj AND tgd -> z11",
			"bfw XOR mjb -> z00",
			"x03 OR x00 -> vdt",
			"gnj AND wpb -> z02",
			"x04 AND y00 -> kjc",
			"djm OR pbm -> qhw",
			"nrd AND vdt -> hwm",
			"kjc AND fst -> rvg",
			"y04 OR y02 -> fgs",
			"y01 AND x02 -> pbm",
			"ntg OR kjc -> kwq",
			"psh XOR fgs -> tgd",
			"qhw XOR tgd -> z09",
			"pbm OR djm -> kpj",
			"x03 XOR y03 -> ffh",
			"x00 XOR y04 -> ntg",
			"bfw OR bqk -> z06",
			"nrd XOR fgs -> wpb",
			"frj XOR qhw -> z04",
			"bqk OR frj -> z07",
			"y03 OR x01 -> nrd",
			"hwm AND bqk -> z03",
			"tgd XOR rvg -> z12",
			"tnw OR pbm -> gnj"
		};

		return SolvePart1(lines) == 2024;
	}
}