#pragma once

#include <functional>
#include <unordered_map>
#include <set>
#include <string>
#include <variant>

#include "Core/Platform/Types.h"
#include "Core/Constexpr/ConstexprStrUtils.h"


using ResultType = std::variant<std::string, u32, u64, s32, s64>;
using SolutionFunc = std::function<std::string(const std::vector<std::string>&)>;
using TestFunc = std::function<bool()>;

//surprisingly, this is slower than std::function
//using SolutionFunc = std::string(*)(const std::vector<std::string>&);
//using TestFunc = bool(*)();

std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, SolutionFunc>>>& GetSolutions();
std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, TestFunc>>>& GetTests();

struct SolutionRegistrar {
	SolutionRegistrar(size_t year, size_t day, size_t part, std::function<ResultType(const std::vector<std::string>&)> solution) {
		GetSolutions()[year][day][part] = [solution](const std::vector<std::string>& lines) {
			auto result = solution(lines);
			auto visitor = [](auto&& arg) -> std::string {
				return Constexpr::ToString(arg);
			};
			return std::visit(visitor, result);
		};
	}
};

struct TestRegistrar {
	TestRegistrar(size_t year, size_t day, size_t testNum, TestFunc testFunc) {
		GetTests()[year][day][testNum] = testFunc;
	}
};

#define SOLUTION(_year, _day) namespace Year##_year##Day##_day { \
	constexpr size_t _CurrentYear = _year; \
	constexpr size_t _CurrentDay = _day; \
}; \
namespace Year##_year##Day##_day

#define PART(_PartNum) constexpr ResultType Part##_PartNum(const std::vector<std::string>& lines); \
	SolutionRegistrar reg_Part##_PartNum{_CurrentYear, _CurrentDay, _PartNum, Part##_PartNum}; \
    constexpr ResultType Part##_PartNum(const std::vector<std::string>& lines)

#define TEST(_TestNum) constexpr bool Test##_TestNum(); \
	TestRegistrar reg_Test##_TestNum{_CurrentYear, _CurrentDay, _TestNum, Test##_TestNum}; \
	constexpr bool Test##_TestNum()
