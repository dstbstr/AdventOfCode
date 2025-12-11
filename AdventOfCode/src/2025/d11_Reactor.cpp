#include "Common.h"

SOLUTION(2025, 11) {
	using Reactor = Constexpr::BigMap<std::string, std::vector<std::string>, 5'000>;
	using Cache = Constexpr::BigMap<std::string, size_t, 5'000>;
	constexpr Reactor MakeReactors(const std::vector<std::string>&lines) {
		Reactor reactors;
		for (const auto& line : lines) {
			auto s1 = Constexpr::Split(line, ": ");
			auto parts = Constexpr::Split(s1[1], " ");
			std::vector<std::string> strs{};
			for (auto str : parts) {
				strs.push_back(std::string(str));
			}
			reactors.emplace(std::string(s1[0]), strs);
		}
		return reactors;
	}

	constexpr size_t CountPaths(const std::string& from, const std::string& to, const Reactor& reactors, Cache& cache) {
		if(from == to) {
			return 1;
		} else if(from == "out") {
			return 0;
		}
		if (cache.contains(from)) {
			return cache[from];
		}
		size_t total = 0;
		for (const auto& child : reactors.at(from)) {
			total += CountPaths(child, to, reactors, cache);
		}
		cache[from] = total;
		return total;
	}

	constexpr size_t Solve(const std::vector<std::string>& lines, const std::vector<std::string>& path) {
		Cache cache;
		auto reactors = MakeReactors(lines);
		size_t result = 1;
		for(size_t i = 0; i < path.size() - 1; i++) {
			auto from = path[i];
			auto to = path[i + 1];
			cache.clear();
			size_t count = 0;
			for(auto child : reactors.at(from)) {
				count += CountPaths(child, to, reactors, cache);
			}
			result *= count;
		}
		return result;
	}

	PART(1) {
		return Solve(lines, {"you", "out"});
	}
	PART(2) {
		auto reactor = MakeReactors(lines);
		return Solve(lines, {"svr", "fft", "dac", "out"});
	}

	TEST(1) {
		std::vector<std::string> input = {
			"aaa: you hhh",
			"you: bbb ccc",
			"bbb: ddd eee",
			"ccc: ddd eee fff",
			"ddd: ggg",
			"eee: out",
			"fff: out",
			"ggg: out",
			"hhh: ccc fff iii",
			"iii: out"
		};

		return Solve(input, {"you", "out"}) == 5;
	}

	TEST(2) {
		std::vector<std::string> input = {
		"svr: aaa bbb",
		"aaa: fft",
		"fft: ccc",
		"bbb: tty",
		"tty: ccc",
		"ccc: ddd eee",
		"ddd: hub",
		"hub: fff",
		"eee: dac",
		"dac: fff",
		"fff: ggg hhh",
		"ggg: out",
		"hhh: out"
		};

		return Solve(input, {"svr", "fft", "dac", "out"}) == 2;
	}
}