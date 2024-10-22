#include "Runner/ExeInputReader.h"
#include "Runner/SolutionRunner.h"

#include "Core/Constexpr/ConstexprStrUtils.h"
#include "Core/Instrumentation/Logging.h"
#include "Core/Instrumentation/ScopedTimer.h"
#include "Core/Instrumentation/ISink.h"
#include "Core/Utilities/TimeUtils.h"

#include <string>
#include <chrono>
#include <map>
#include <iostream>

SolutionRunner RunFromCommandLine(int argc, char** argv, std::unique_ptr<IInputReader>&& inputReader, bool sync) {
    if(argv[1][0] == '*') {
        return SolutionRunner(std::move(inputReader), sync);
	}
	else {
		size_t year;
		Constexpr::ParseNumber(argv[1], year);
		if (argc > 2) {
			if (argv[2][0] == '*') {
				return SolutionRunner(year, std::move(inputReader), sync);
			}
			else {
				size_t day;
				Constexpr::ParseNumber(argv[2], day);
				return SolutionRunner(year, day, std::move(inputReader), sync);
			}
		}
		else {
			return SolutionRunner(year, std::move(inputReader), sync);
		}
    }
}


struct LogWriter : public Log::ISink {
    LogWriter(Log::Filter filter = {}) : ISink(filter) {
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
    LogWriter logWriter{};
    
    bool sync = false;
    auto runner = [&]{
        if (argc > 1) {
            return RunFromCommandLine(argc, argv, std::make_unique<ExeInputReader>(), sync);
        } else {
            return SolutionRunner (2017, std::make_unique<ExeInputReader>(), sync);
            //return SolutionRunner(std::make_unique<ExeInputReader>(), sync);
        }
    }();
        
    {
		ScopedTimer timer("Total Runtime", [](std::string_view label, std::chrono::microseconds elapsed) {
            Log::Info(std::format("{}: {}", label, TimeUtils::DurationToString(elapsed, TimeUtils::TimeUnit::SECOND)));
        });
        runner.Run();
        Log::Info("");
    }

    //runner.LogResults();
    //runner.LogTimingData();
}
