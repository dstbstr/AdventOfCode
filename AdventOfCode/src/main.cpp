#include "Runner/ExeInputReader.h"
#include "Runner/SolutionRunner.h"

#include "Core/Constexpr/ConstexprStrUtils.h"
#include "Core/Instrumentation/Logging.h"
#include "Core/Instrumentation/ScopedTimer.h"
#include "Core/Instrumentation/ISink.h"
#include "Core/Utilities/TimeUtils.h"

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <string_view>

SolutionRunner RunFromCommandLine(int argc, char** argv, std::unique_ptr<IInputReader>&& inputReader, SolutionRunner::Settings settings) {
    if(argv[1][0] == '*') {
        return SolutionRunner(std::move(inputReader), settings);
	}
	else {
		size_t year;
		Constexpr::ParseNumber(argv[1], year);
		if (argc > 2) {
			if (argv[2][0] == '*') {
				return SolutionRunner(year, std::move(inputReader), settings);
			}
			else {
				size_t day;
				Constexpr::ParseNumber(argv[2], day);
				return SolutionRunner(year, day, std::move(inputReader), settings);
			}
		}
		else {
			return SolutionRunner(year, std::move(inputReader), settings);
		}
    }
}

struct FileLogWriter : public Log::ISink {
    std::ofstream m_File{};

    FileLogWriter(Log::Filter filter = {}) : ISink(filter) {
        auto fileName = TimeUtils::TodayNowLocalToString("%Y-%m-%d_%H-%M-%S");
        fileName += ".log";
        m_File.open(fileName);
        if (!m_File.is_open()) {
            std::cerr << "Failed to open log file" << fileName << "\n";
            std::abort();
        }
    }
    void Write(const Log::Entry& entry) override {
        m_File << entry.Message << '\n';
    }
};

struct StdLogWriter : public Log::ISink {
    StdLogWriter(Log::Filter filter = {}) : ISink(filter) {
		// Disable synchronization with C stdio
        // No interop with C
		std::ios_base::sync_with_stdio(false);
		
        // Untie stdio streams from C++ streams
        // Not using cin, so no need to synchronize with cout
        std::cin.tie(nullptr);
    }
    void Write(const Log::Entry& entry) override {
        std::cout << entry.Message << '\n';
    }
};

int main(int argc, char** argv) {
    StdLogWriter stdLogWriter{};
    //FileLogWriter fileLogWriter{};
    
    SolutionRunner::Settings runSettings{
        .Sync = true,
        .SkipTests = false,
        .SlowFirst = false
    };
    auto runner = [&]{
        if (argc > 1) {
            return RunFromCommandLine(argc, argv, std::make_unique<ExeInputReader>(), runSettings);
        } else {
            //return SolutionRunner(2024, std::make_unique<ExeInputReader>(), runSettings);
            return SolutionRunner (2024, 4, std::make_unique<ExeInputReader>(), runSettings);
            //return SolutionRunner(std::make_unique<ExeInputReader>(), runSettings);
        }
    }();
        
    {
		// ScopedTimer timer("Total Runtime", [](std::string_view label, std::chrono::microseconds elapsed) {
        //     Log::Info(std::format("{}: {}", label, TimeUtils::DurationToString(elapsed)));
        // });
        runner.Run();
    }

    runner.LogResults();
    Log::Info("");
    runner.LogTimingData(SolutionRunner::SortBy::Problem);
}
