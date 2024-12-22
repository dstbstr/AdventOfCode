#include "Common.h"

SOLUTION(2024, 22) {
	//multiply by 64, mix, prune
	//divide by 32, mix, prune
	//multiply by 2048, mix, prune

	//mix: XOR value with secret
	//prune: secret % 16777216

	constexpr u64 Mix(u64 value, u64 secret) {
		return value ^ secret;
	}
	static_assert(Mix(42, 15) == 37);

	constexpr u64 Prune(u64 value) {
		return value % 16777216u;
	}
	static_assert(Prune(100000000) == 16113920);

	constexpr u64 MixPrune(u64 value, u64 secret) {
		return Prune(Mix(value, secret));
	}

	//static_assert(MixPrune(123 * 64, 123) == 15887950);

	constexpr u64 NextSecret(u64 secret) {
		auto r1 = [](u64 secret) { return MixPrune(secret * 64, secret); };
		auto r2 = [](u64 secret) { return MixPrune(secret / 32, secret); };
		auto r3 = [](u64 secret) { return MixPrune(secret * 2048, secret); };

		return r3(r2(r1(secret)));
	}

	static_assert(NextSecret(123) == 15887950);
	

	PART(1) {
		auto initials = ParseLinesAsNumbers<u64>(lines);
		u64 result = 0;
		for(auto secret : initials) {
			for(auto i = 0; i < 2000; i++) {
				secret = NextSecret(secret);
			}
			result += secret;
		}

		return result;
	}

	//convert 4 deltas to a u32
	//numbers 0-20 require 5 bits, 5 bits * 4 = 20 bits, fits in a u32
	constexpr u32 ToU32(const std::array<u8, 4>& deltas) {
		u32 result = 0;
		for (auto i = 0; i < 4; i++) {
			result |= (deltas[i] & 0b1111) << (i * 5);
		}
		return result;
	}

	PART(2) {
		auto initials = ParseLinesAsNumbers<u64>(lines);
		Constexpr::BigMap<u32, std::vector<u8>, 100'000> salePrices{};

		for(auto secret : initials) {
			Constexpr::BigSet<u32, 2'000> seen{};
			std::array<u8, 4> window;
			u8 prev = secret % 10;
			for(auto i = 0; i < 2000; i++) {
				secret = NextSecret(secret);
				u8 current = secret % 10;
				window.back() = current + 10 - prev; // add 10 to avoid negatives
				std::rotate(window.begin(), window.begin() + 1, window.end());
				auto key = ToU32(window);
				if(seen.insert(key)) {
					salePrices[ToU32(window)].emplace_back(current);
				}

				prev = current;
			}
		}

		u64 best = 0;
		for(const auto& prices : salePrices.GetValues()) {
			best = std::max(best, std::accumulate(prices.begin(), prices.end(), 0ull));
		}

		return best;
	}

	TEST(1) {
		std::vector<std::string> lines = {
			"1",
			"10",
			"100",
			"2024"
		};

		return std::get<u64>(Part1(lines)) == 37327623;
	}

	TEST(2) {
		std::vector<std::string> lines = {
			"1",
			"10",
			"100",
			"2024"
		};

		return std::get<u64>(Part2(lines)) == 23;
	}
}