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

namespace {
    std::string MakeKey(size_t year, size_t day, size_t part) {
		return Constexpr::ToString(year) + "_" + Constexpr::ToString(day) + "_" + Constexpr::ToString(part);
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
}

bool SolutionRunner::CheckTestsPass(size_t year, size_t day) {
    if (!GetTests().contains(year) || !GetTests()[year].contains(day)) {
        return true;
    }
    size_t passedTests = 0;
	for (const auto& [testNum, testFunc] : GetTests()[year][day]) {
		auto testTime = ScopedTimer(MakeKey(year, day, testNum), GatherTiming(m_TimingData));
		if (!testFunc()) {
			Log::Info(std::format("{}_{} Test {} Fail", year, day, testNum));
			return false;
		}
        passedTests++;
	}
    return true;
}

void SolutionRunner::AddSolution(size_t year, size_t day) {
    Results[year][day][1] = "";
	Results[year][day][2] = "";
	m_ToRun.emplace_back([=]() {
        if (!CheckTestsPass(year, day)) return;

        for (const auto& [part, func] : GetSolutions()[year][day]) {
            auto input = m_InputReader->ReadInput(year, day);
            {
                auto partTime = ScopedTimer(MakeKey(year, day, part), GatherTiming(m_TimingData));
                auto result = func(input);
                Results.at(year).at(day).at(part) = result;
            }
            WriteLogs();
        }
    });
}

SolutionRunner::SolutionRunner(std::unique_ptr<IInputReader>&& inputReader, bool sync) 
    : m_InputReader(std::move(inputReader))
    , m_Sync(sync)
{
	for (const auto& [year, days] : GetSolutions()) {
		for (const auto& [day, parts] : days) {
            AddSolution(year, day);
		}
	}
}

SolutionRunner::SolutionRunner(size_t year, std::unique_ptr<IInputReader>&& inputReader, bool sync) 
    : m_InputReader(std::move(inputReader))
    , m_Sync(sync)
{
	for (const auto& [day, parts] : GetSolutions()[year]) {
		AddSolution(year, day);
	}
}

SolutionRunner::SolutionRunner(size_t year, size_t day, std::unique_ptr<IInputReader>&& inputReader, bool sync) 
    : m_InputReader(std::move(inputReader))
    , m_Sync(sync)
{
    AddSolution(year, day);
}

void SolutionRunner::Run() {
    if(m_Sync) {
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

void SolutionRunner::LogTimingData(size_t maxResults, std::chrono::microseconds minElapsed) const {
    std::vector<std::pair<std::string, std::chrono::microseconds>> copy(m_TimingData.begin(), m_TimingData.end());
    std::sort(copy.begin(), copy.end(), [](auto lhs, auto rhs) { return rhs.second < lhs.second; });

    size_t resultId = 0;
    for (auto [key, elapsed] : copy) {
        if (minElapsed > std::chrono::microseconds(0) && elapsed < minElapsed) break;
        if (maxResults > 0 && resultId > maxResults) break;

        Log::Info(std::format("{}: {}", key, TimeUtils::DurationToString(elapsed)));
    }
}

void SolutionRunner::LogResults() const {
    Log::Info("");
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
