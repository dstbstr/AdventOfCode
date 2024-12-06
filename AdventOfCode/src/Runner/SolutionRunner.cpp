#include "Runner/SolutionRunner.h"
#include "Common.h"

#include "Core/Constexpr/ConstexprStrUtils.h"
#include "Core/Instrumentation/Logging.h"
#include "Core/Instrumentation/ScopedTimer.h"
#include "Core/Threading/Tasks.h"
#include "Core/Utilities/TimeUtils.h"

#include <chrono>
#include <format>
#include <future>
#include <iostream> // for progress bar
#include <string_view>

namespace {
    std::string MakeKey(size_t year, size_t day, size_t part, bool isTest) {
		return (isTest ? "Test_" : "") + Constexpr::ToString(year) + "_" + Constexpr::ToString(day) + "_" + Constexpr::ToString(part);
	}

    std::map<size_t, std::map<size_t, std::map<size_t, std::string>>> Results{};

    void WriteLogs() {
        if (!GET_LOGS().empty()) Log::Info("## Logs ##");
        for (const auto& log : GET_LOGS()) {
            Log::Info(log);
        }
        GET_LOGS().clear();
    }

    auto GatherTiming(std::map<std::string, std::chrono::microseconds>& timingData) {
        return [&](std::string_view key, std::chrono::microseconds elapsed) {
            timingData[std::string(key)] = elapsed;
        };
    };

	bool SortByProblem(const std::pair<std::string, std::chrono::microseconds>& lhs, const std::pair<std::string, std::chrono::microseconds>& rhs) {
		auto parseParts = [](const std::string& str) -> std::tuple<s32, s32, s32, bool> {
			auto parts = Constexpr::Split(str, "_");
			s32 a, b, c;
			if (parts.size() == 3) {
				Constexpr::ParseNumber(parts[0], a);
				Constexpr::ParseNumber(parts[1], b);
				Constexpr::ParseNumber(parts[2], c);
				return { a, b, c, false };
			}
			else {
				Constexpr::ParseNumber(parts[1], a);
				Constexpr::ParseNumber(parts[2], b);
				Constexpr::ParseNumber(parts[3], c);
				return { a, b, c, true };
			}
			};
		auto [lhsYear, lhsDay, lhsPart, lhsTest] = parseParts(lhs.first);
		auto [rhsYear, rhsDay, rhsPart, rhsTest] = parseParts(rhs.first);

		if (lhsYear != rhsYear) return lhsYear < rhsYear;
		if (lhsDay != rhsDay) return lhsDay < rhsDay;
		if (lhsTest != rhsTest) return rhsTest;
		return lhsPart < rhsPart;
	}

	void LogTimingData(std::map<std::string, std::chrono::microseconds> timingData, SolutionRunner::SortBy sortBy, std::optional<std::chrono::microseconds> minElapsed = std::nullopt) {
		std::vector<std::pair<std::string, std::chrono::microseconds>> copy(timingData.begin(), timingData.end());
		if (sortBy == SolutionRunner::SortBy::RunTime) {
			std::sort(copy.begin(), copy.end(), [](auto lhs, auto rhs) { return rhs.second < lhs.second; });
		}
		else {
			std::sort(copy.begin(), copy.end(), SortByProblem);
		}

		for (auto [key, elapsed] : copy) {
			if (minElapsed.has_value() && elapsed < minElapsed) break;

			Log::Info(std::format("{}: {}", key, TimeUtils::DurationToString(elapsed)));
		}
	}

	void LogResults() {
		for (const auto& [year, days] : Results) {
			Log::Info(std::format("\n### {} ###", year));
			for (const auto& [day, parts] : days) {
				Log::Info(std::format("\nDay {}", day));
				for (const auto& [part, result] : parts) {
					if (result.empty()) continue;
					Log::Info(std::format("Part {}: {}", part, result));
				}
			}
		}
	}
}

bool SolutionRunner::CheckTestsPass(size_t year, size_t day) {
    if (!GetTests().contains(year) || !GetTests()[year].contains(day)) {
        return true;
    }
	for (const auto& [testNum, testFunc] : GetTests()[year][day]) {
		auto testTime = ScopedTimer(MakeKey(year, day, testNum, true), GatherTiming(m_TimingData));
		if (!testFunc()) [[unlikely]] {
			Log::Info(std::format("{}_{} Test {} Fail", year, day, testNum));
			WriteLogs();
			return false;
		}
	}
    return true;
}

void SolutionRunner::AddSolution(size_t year, size_t day) {
    Results[year][day][1] = "";
	Results[year][day][2] = "";
    auto func = [this, year, day]() {
		if (!CheckTestsPass(year, day)) [[unlikely]] return;

        for (const auto& [part, func] : GetSolutions()[year][day]) {
            auto input = m_InputReader->ReadInput(year, day);
            {
                auto partTime = ScopedTimer(MakeKey(year, day, part, false), GatherTiming(m_TimingData));
                auto result = func(input);
                Results.at(year).at(day).at(part) = result;
            }
            WriteLogs();
        }
    };

	m_ToRun.emplace_back(func);
}

SolutionRunner::SolutionRunner(std::unique_ptr<IInputReader>&& inputReader) 
    : m_InputReader(std::move(inputReader))
{
	for (const auto& [year, days] : GetSolutions()) {
		for (const auto& [day, parts] : days) {
            AddSolution(year, day);
		}
	}
}

SolutionRunner::SolutionRunner(size_t year, std::unique_ptr<IInputReader>&& inputReader) 
    : m_InputReader(std::move(inputReader))
{
	for (const auto& [day, parts] : GetSolutions()[year]) {
		AddSolution(year, day);
	}
}

SolutionRunner::SolutionRunner(size_t year, size_t day, std::unique_ptr<IInputReader>&& inputReader) 
    : m_InputReader(std::move(inputReader))
{
    AddSolution(year, day);
}

void SolutionRunner::Run(const SolutionRunner::Settings& settings) {
	{
		ScopedTimer timer("Total Runtime", [](std::string_view label, std::chrono::microseconds elapsed) {
			Log::Info(std::format("{}: {}", label, TimeUtils::DurationToString(elapsed)));
			});

		if(settings.Sync) {
			for (const auto& run : m_ToRun) {
				run();
			}
		}
		else {
			auto progressBar = [](size_t, size_t, double percentage) {
				std::cout << "\r" << std::format("{:.2f}%", percentage * 100);
			};
			WhenAll(m_ToRun, progressBar).wait();
		}
	}
	if(settings.Sync) {
		Log::Info("");
	}
	if (settings.PrintResults) {
		LogResults();
		Log::Info("");
	}
	if (settings.PrintTiming) {
		LogTimingData(m_TimingData, settings.TimingSort);
	}
}