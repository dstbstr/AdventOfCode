#include "Runner/ExeInputReader.h"
#include "Runner/BenchmarkRunner.h"

#include <Core/Instrumentation/Logging.h>
#include <CliParser/CommandRegistry.h>

struct RunBenchmarkArgs : CliParser::IArgs {
	OPTION(size_t, 'y', Year, "Year of the problem");
	OPTION(size_t, 'd', Day, "Day of the problem");
	OPTION(u32, 's', Seconds, "How many seconds to run the benchmark");
	OPTION(std::vector<size_t>, 'p', Parts, "Parts to run/compare");
};

COMMAND(Benchmark, RunBenchmarkArgs, args) {
	auto runner = BenchmarkRunner(std::make_unique<ExeInputReader>(), args.Year, args.Day, std::chrono::seconds(args.Seconds));
	if (args.Parts.size() == 1) {
		runner.RunSingle(args.Parts[0]);
	}
	else if (args.Parts.size() == 2) {
		runner.Compare(args.Parts[0], args.Parts[1]);
	} else {
		Log::Error("Expected either 1 or 2 parts to run");
		outResult = 1;
	}
}