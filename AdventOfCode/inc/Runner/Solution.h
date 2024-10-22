#pragma once

#include <functional>
#include <unordered_map>
#include <string>

using SolutionFunc = std::function<std::string(const std::vector<std::string>&)>;
using TestFunc = std::function<bool()>;

//surprisingly, this is slower than std::function
//using SolutionFunc = std::string(*)(const std::vector<std::string>&);
//using TestFunc = bool(*)();

std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, SolutionFunc>>>& GetSolutions();
std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, TestFunc>>>& GetTests();

struct SolutionRegistrar {
	SolutionRegistrar(size_t year, size_t day, size_t part, SolutionFunc solution) {
		GetSolutions()[year][day][part] = solution;
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

#define PART(_PartNum) constexpr std::string Part##_PartNum(const std::vector<std::string>& lines); \
	SolutionRegistrar reg_Part##_PartNum{_CurrentYear, _CurrentDay, _PartNum, Part##_PartNum}; \
    constexpr std::string Part##_PartNum(const std::vector<std::string>& lines)

#define TEST(_TestNum) constexpr bool Test##_TestNum(); \
	TestRegistrar reg_Test##_TestNum{_CurrentYear, _CurrentDay, _TestNum, Test##_TestNum}; \
	constexpr bool Test##_TestNum()
