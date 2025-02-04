#pragma once
#include "Runner/IInputReader.h"

#include <memory>
#include <chrono>

class BenchmarkRunner {
public:
	template<typename Rep, typename Period>
	BenchmarkRunner(std::unique_ptr<IInputReader>&& inputReader, size_t year, size_t day, std::chrono::duration<Rep, Period> duration = std::chrono::seconds(5))
		: m_InputReader(std::move(inputReader))
		, m_Year(year)
		, m_Day(day)
		, m_Duration(std::chrono::duration_cast<std::chrono::milliseconds>(duration))
	{}

	void RunSingle(size_t part);
	void Compare(size_t part1, size_t part2); // TODO: make vector?
	std::chrono::microseconds FindBest(size_t part);
private:
	std::unique_ptr<IInputReader> m_InputReader{ nullptr };
	size_t m_Year{};
	size_t m_Day{};
	std::chrono::milliseconds m_Duration{};
};