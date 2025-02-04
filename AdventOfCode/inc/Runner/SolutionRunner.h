#pragma once

#include "Runner/IInputReader.h"
#include <vector>
#include <map>
#include <chrono>
#include <functional>
#include <optional>

class SolutionRunner {
public:
	enum struct SortBy {
		RunTime,
		Problem
	};

	enum struct Tests {
		Include,
		Exclude
	};

	struct Settings {
		bool Sync{ false };
		bool PrintTiming{ false };
		bool PrintResults{ false };
		SortBy TimingSort{ SortBy::RunTime };
		std::optional<std::function<bool(std::string_view, std::chrono::microseconds)>> TimingFilter{std::nullopt};
	};

	SolutionRunner(std::unique_ptr<IInputReader>&& inputReader, Tests tests = Tests::Include);
	SolutionRunner(size_t year, std::unique_ptr<IInputReader>&& inputReader);
	SolutionRunner(size_t year, size_t day, std::unique_ptr<IInputReader>&& inputReader);

	void Run(const Settings& settings);

private:
	std::unique_ptr<IInputReader> m_InputReader{nullptr};
	std::map<std::string, std::chrono::microseconds> m_TimingData{};
	std::vector<std::function<void()>> m_ToRun;

	bool CheckTestsPass(size_t year, size_t day);
	void AddSolution(size_t year, size_t day, Tests tests = Tests::Include);
};

namespace TimingFilters {
	bool SlowProblems(std::string_view, std::chrono::microseconds elapsed);
	bool NoTests(std::string_view label, std::chrono::microseconds);
	bool TestsOnly(std::string_view label, std::chrono::microseconds);
	bool All(std::string_view, std::chrono::microseconds);
}

namespace RunSettings {
	extern SolutionRunner::Settings FindSlow;
	extern SolutionRunner::Settings RunAll;
	extern SolutionRunner::Settings RunOne;
}