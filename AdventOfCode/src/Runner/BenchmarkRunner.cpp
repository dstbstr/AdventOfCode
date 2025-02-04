#include "Runner/BenchmarkRunner.h"
#include "Core/Instrumentation/Benchmark/Benchmark.h"
#include "Core/Instrumentation/Logging.h"

#include <format>

namespace {
	constexpr auto ToMb = 1.0 / 1024.0 / 1024.0;
	using namespace std::chrono_literals;
	auto MakeBench(std::chrono::milliseconds duration) {
		return Benchmark::Builder()
			.WithMaxDuration(duration)
			.WithMaxIterations(10'000'000)
			.WithRuntimeResoultion(Benchmark::RuntimeResolution::Micros)
			.WithMonitor({ MonitorType::Cpu, 100ms, 100.0, "%" })
			.WithMonitor({ MonitorType::PMem, 100ms, ToMb, "Mb" })
			.Build();
	}
	
	void LogResult(const auto& result) {
		const auto separator = "+--------------------+--------------------+";
		// These need to match with the WithRuntimeResolution parameter
		auto minRuntime = std::chrono::microseconds(static_cast<u64>(result.RuntimeStats.Min));
		auto maxRuntime = std::chrono::microseconds(static_cast<u64>(result.RuntimeStats.Max));
		auto avgRuntime = std::chrono::microseconds(static_cast<u64>(result.RuntimeStats.Mean));
		Log::Info(separator);
		Log::Info(std::format("|{:^20}|{:^20}|", "Label", "Value"));
		Log::Info(separator);
		Log::Info(std::format("|  {:<18}|  {:<18}|", "Result", result.ReturnValue));
		Log::Info(std::format("|  {:<18}|  {:<18}|", "Iterations", result.RuntimeStats.Count));
		Log::Info(std::format("|  {:<18}|  {:<18}|", "Runtime (Min)", minRuntime));
		Log::Info(std::format("|  {:<18}|  {:<18}|", "Runtime (Max)", maxRuntime));
		Log::Info(std::format("|  {:<18}|  {:<18}|", "Runtime (Mean)", avgRuntime));
		for (const auto& [type, stats] : result.MonitorStats) {
			Log::Info(std::format("|  {:<18}|  {:<18.2f}|", ToString(type), stats.Mean));
		}
		Log::Info(separator);
	}

	void LogResults(const auto& r1, const auto& r2) {
		const auto separator = "+--------------------+--------------------+--------------------+--------------------+";
		auto lhsMinRuntime = std::chrono::microseconds(static_cast<u64>(r1.RuntimeStats.Min));
		auto rhsMinRuntime = std::chrono::microseconds(static_cast<u64>(r2.RuntimeStats.Min));
		auto lhsMaxRuntime = std::chrono::microseconds(static_cast<u64>(r1.RuntimeStats.Max));
		auto rhsMaxRuntime = std::chrono::microseconds(static_cast<u64>(r2.RuntimeStats.Max));
		auto lhsAvgRuntime = std::chrono::microseconds(static_cast<u64>(r1.RuntimeStats.Mean));
		auto rhsAvgRuntime = std::chrono::microseconds(static_cast<u64>(r2.RuntimeStats.Mean));
		Log::Info(separator);
		Log::Info(std::format("|  {:^18}|{:^20}|{:^20}|{:^20}|", "", "First", "Second", "Delta"));
		Log::Info(separator);
		Log::Info(std::format("|  {:<18}|  {:<18}|  {:<18}|  {:<18}|", "Result", r1.ReturnValue, r2.ReturnValue, (r1.ReturnValue == r2.ReturnValue ? "Same" : "Different")));
		Log::Info(std::format("|  {:<18}|  {:<18}|  {:<18}|  {:<18}|", "Iterations", r1.RuntimeStats.Count, r2.RuntimeStats.Count, static_cast<s64>(r1.RuntimeStats.Count) - static_cast<s64>(r2.RuntimeStats.Count)));
		Log::Info(std::format("|  {:<18}|  {:<18}|  {:<18}|  {:<+18.2f}|", "Runtime (Min)", lhsMinRuntime, rhsMinRuntime, r1.RuntimeStats.Min - r2.RuntimeStats.Min));
		Log::Info(std::format("|  {:<18}|  {:<18}|  {:<18}|  {:<+18.2f}|", "Runtime (Max)", lhsMaxRuntime, rhsMaxRuntime, r1.RuntimeStats.Max - r2.RuntimeStats.Max));
		Log::Info(std::format("|  {:<18}|  {:<18}|  {:<18}|  {:<+18.2f}|", "Runtime (Mean)", lhsAvgRuntime, rhsAvgRuntime, r1.RuntimeStats.Mean - r2.RuntimeStats.Mean));
		for(const auto& [type, lhsStats] : r1.MonitorStats) {
			const auto& rhsStats = r2.MonitorStats.at(type);
			Log::Info(std::format("|  {:<18}|  {:<18.2f}|  {:<18.2f}|  {:<+18.2f}|", ToString(type), lhsStats.Mean, rhsStats.Mean, lhsStats.Mean - rhsStats.Mean));
		}
		Log::Info(separator);
	}
}

void BenchmarkRunner::RunSingle(size_t part) 
{
	auto input = m_InputReader->ReadInput(m_Year, m_Day);
	auto func = GetSolutions()[m_Year][m_Day][part];
	auto bench = MakeBench(m_Duration);
	auto result = bench.Run([&]() { return func(input); });

	LogResult(result);
}

std::chrono::microseconds BenchmarkRunner::FindBest(size_t part) {
	auto input = m_InputReader->ReadInput(m_Year, m_Day);
	auto func = GetSolutions()[m_Year][m_Day][part];
	auto bench = MakeBench(m_Duration);
	auto result = bench.Run([&]() { return func(input); });

	return std::chrono::microseconds(static_cast<u64>(result.RuntimeStats.Min));
}

void BenchmarkRunner::Compare(size_t part1, size_t part2) {
	auto input = m_InputReader->ReadInput(m_Year, m_Day);

	Log::Info("Starting first part");
	auto func1 = GetSolutions()[m_Year][m_Day][part1];
	auto bench1 = MakeBench(m_Duration);
	auto result1 = bench1.Run([&]() { return func1(input); });

	Log::Info("Starting second part");
	auto func2 = GetSolutions()[m_Year][m_Day][part2];
	auto bench2 = MakeBench(m_Duration);
	auto result2 = bench2.Run([&]() { return func2(input); });

	LogResults(result1, result2);
}