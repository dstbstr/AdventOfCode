#include "Runner/ExeInputReader.h"
#include "Runner/SolutionRunner.h"
#include "Runner/BenchmarkRunner.h"
#include "Logging/AocLogs.h"
#include "Logging/LogStream.h"

#include <CliParser/CliParser.h>

StdLogWriter stdLogWriter{};
//FileLogWriter fileLogWriter{};

void FindBestTimes(size_t year) {
    using namespace std::chrono_literals;

    std::chrono::microseconds totalTime{};
    for (size_t day = 1; day <= 25; day++) {
        BenchmarkRunner runner{ std::make_unique<ExeInputReader>(), year, day, 5s };
        auto bestTime = runner.FindBest(1);
        totalTime += bestTime;
        Log::Info(std::format("{}-{}-1: {}", year, day, bestTime));
        if (day < 25) {
            bestTime = runner.FindBest(2);
            totalTime += bestTime;
            Log::Info(std::format("{}-{}-2: {}", year, day, bestTime));
        }
    }

    Log::Info(std::format("Best possible time: {}ms", std::chrono::duration_cast<std::chrono::milliseconds>(totalTime).count()));
}


int main(int argc, char** argv) {
    if(argc > 1) {
        auto outErrors = LogStream(Log::Error);
		return CliParser::Run(argc, argv, outErrors);
    } else {
        auto runner = SolutionRunner(2025, 4, std::make_unique<ExeInputReader>());
        runner.Run(RunSettings::RunOne);
		//auto runner = BenchmarkRunner(std::make_unique<ExeInputReader>(), 2015, 6, 3s);
        //runner.Compare(1, 3);
        //runner.Compare(2, 4);
        
         
        //auto runner = SolutionRunner(2024, 16, std::make_unique<ExeInputReader>());
        //runner.Run(RunSettings::RunOne);
        //BenchmarkRunner runner{ std::make_unique<ExeInputReader>(), 2024, 16, 30s };
        //runner.RunSingle(2);
    }
    //runner.Compare(1, 2);

    // auto r2 = SolutionRunner(2024, 1, std::make_unique<ExeInputReader>());
    // r2.Run(RunSettings::RunOne);
    //RunBenchmark(2024, 1, 2);
	//auto runner = SolutionRunner(2024, 1, std::make_unique<ExeInputReader>());
    //runner.Run(runOneSettings);
    /*
    auto runner = [&]{
        if (argc > 1) {
            return RunFromCommandLine(argc, argv, std::make_unique<ExeInputReader>());
        } else {
            //return SolutionRunner(2024, std::make_unique<ExeInputReader>());
            //return SolutionRunner (2024, 25, std::make_unique<ExeInputReader>());
            //return SolutionRunner (2018, 15, std::make_unique<ExeInputReader>());
            return SolutionRunner(std::make_unique<ExeInputReader>(), SolutionRunner::Tests::Include);
        }
    }();
        
	//runner.Run(runOneSettings);
    runner.Run(runAllSettings);
    */
}

