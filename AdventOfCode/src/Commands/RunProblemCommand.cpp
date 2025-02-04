#include "Runner/ExeInputReader.h"
#include "Runner/SolutionRunner.h"

#include <CliParser/CommandRegistry.h>

namespace {
	SolutionRunner GetRunner(std::optional<size_t> year, std::optional<size_t> day) {
		if(!year.has_value() || year == 0) {
			return SolutionRunner(std::make_unique<ExeInputReader>());
		}
		else if (!day.has_value() || day == 0) {
			return SolutionRunner(year.value(), std::make_unique<ExeInputReader>());
		}
		else {
			return SolutionRunner(year.value(), day.value(), std::make_unique<ExeInputReader>());
		}
	}
}

enum struct RunType {
	FindSlow,
	Detailed,
	Summary
};

namespace CliParser::ArgParse {
	bool TryParse(std::string_view str, RunType& outResult, std::ostream& outErrors) {
		auto lowered = str | std::views::transform([](char c) { return static_cast<char>(tolower(c)); }) | std::ranges::to<std::string>();
		if (lowered == "findslow") outResult = RunType::FindSlow;
		else if (lowered == "detailed") outResult = RunType::Detailed;
		else if (lowered == "summary") outResult = RunType::Summary;
		else {
			outErrors << str << " is not a valid RunType";
			return false;
		}
		return true;
	}
	std::string ToString(const RunType& val) {
		switch (val) {
		case RunType::FindSlow: return "FindSlow";
		case RunType::Detailed: return "Detailed";
		case RunType::Summary: return "Summary";
		}
		return "Unknown";
	}
}

struct RunProblemArgs : CliParser::IArgs {
	OPTION(std::optional<size_t>, 'y', Year, "Year of the problem", 0);
	OPTION(std::optional<size_t>, 'd', Day, "Day of the problem", 0);
	OPTION(RunType, 't', Type, "Type of run", RunType::Summary);
};

DEFAULT_COMMAND(RunProblemArgs, args) {
	auto runner = GetRunner(args.Year, args.Day);

	switch(args.Type) {
	case RunType::FindSlow: runner.Run(RunSettings::FindSlow); break;
	case RunType::Detailed: runner.Run(RunSettings::RunOne); break;
	case RunType::Summary: runner.Run(RunSettings::RunAll); break;
	}
}