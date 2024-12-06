#pragma once

#include "Runner/IInputReader.h"
#include <vector>
#include <map>
#include <chrono>
#include <functional>

class SolutionRunner {
public:
	enum struct SortBy {
		RunTime,
		Problem
	};

	struct Settings {
		bool Sync{ false };
		bool PrintTiming{ false };
		bool PrintResults{ false };
		SortBy TimingSort{ SortBy::RunTime };
	};

	SolutionRunner(std::unique_ptr<IInputReader>&& inputReader);
	SolutionRunner(size_t year, std::unique_ptr<IInputReader>&& inputReader);
	SolutionRunner(size_t year, size_t day, std::unique_ptr<IInputReader>&& inputReader);

	void Run(const Settings& settings);

private:
	std::unique_ptr<IInputReader> m_InputReader{nullptr};
	std::map<std::string, std::chrono::microseconds> m_TimingData{};
	std::vector<std::function<void()>> m_ToRun;

	bool CheckTestsPass(size_t year, size_t day);
	void AddSolution(size_t year, size_t day);
};