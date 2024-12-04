#pragma once

#include "Runner/IInputReader.h"
#include <vector>
#include <map>
#include <chrono>
#include <functional>

class SolutionRunner {
public:
	struct Settings {
		bool Sync{ false };
		bool SkipTests{ false };
		bool SlowFirst{ false };
	};
	enum struct SortBy {
		RunTime,
		Problem
	};


	SolutionRunner(std::unique_ptr<IInputReader>&& inputReader, Settings settings);
	SolutionRunner(size_t year, std::unique_ptr<IInputReader>&& inputReader, Settings settings);
	SolutionRunner(size_t year, size_t day, std::unique_ptr<IInputReader>&& inputReader, Settings settings);

	void Run();
	void LogTimingData(SortBy sortBy, size_t maxResults = 0, std::chrono::microseconds minElapsed = std::chrono::microseconds(0)) const;
	void LogResults() const;

private:
	std::unique_ptr<IInputReader> m_InputReader{nullptr};
	bool m_Sync{false};
	bool m_SkipTests{ false };
	bool m_SlowFirst{ false };
	std::map<std::string, std::chrono::microseconds> m_TimingData{};
	std::vector<std::function<void()>> m_ToRun;

	bool CheckTestsPass(size_t year, size_t day);
	void AddSolution(size_t year, size_t day);
};