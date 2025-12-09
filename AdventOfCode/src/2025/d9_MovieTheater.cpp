#include "Common.h"

SOLUTION(2025, 9) {
	constexpr size_t Area(UCoord a, UCoord b) {
		return (1 + Constexpr::AbsDistance(a.X, b.X)) * (1 + Constexpr::AbsDistance(a.Y, b.Y));
	}
	constexpr UCoord ParseLine(const std::string& line) {
		return UCoord(line);
	}
	struct Candidate {
		UCoord a, b;
		size_t area;
	};

	struct Edge { 
		UCoord a, b;
	};

	constexpr std::vector<Edge> MakeEdges(const std::vector<UCoord>& points) {
		std::vector<Edge> edges;
		for (size_t i = 0; i < points.size(); i++) {
			Edge e{ points[i], points[(i + 1) % points.size()] };
			edges.push_back(e);
		}
		return edges;
	}

	constexpr std::vector<Candidate> MakeCandidates(const std::vector<UCoord>& points) {
		std::vector<Candidate> rects;
		rects.reserve(points.size() * points.size());
		for (size_t i = 0; i < points.size(); i++) {
			for (size_t j = i + 1; j < points.size(); j++) {
				rects.emplace_back(Candidate{ points[i], points[j], Area(points[i], points[j]) });
			}
		}
		rects.emplace_back(Candidate{ points.back(), points.front(), Area(points.back(), points.front()) });
		return rects;
	}

	constexpr bool IsValidRect(const Candidate& rect, const std::vector<Edge>& edges) {
		auto [minRow, maxRow] = std::minmax(rect.a.Y, rect.b.Y);
		auto [minCol, maxCol] = std::minmax(rect.a.X, rect.b.X);
		for (auto e : edges) {
			auto [eMinRow, eMaxRow] = std::minmax(e.a.Y, e.b.Y);
			auto [eMinCol, eMaxCol] = std::minmax(e.a.X, e.b.X);
			if(minRow < eMaxRow && maxRow > eMinRow &&
			   minCol < eMaxCol && maxCol > eMinCol) {
				return false;
			}
		}
		return true;
	}

	constexpr size_t Solve(const std::vector<std::string>& lines, auto validator) {
		auto points = ParseLines(lines, ParseLine);
		auto edges = MakeEdges(points);
		auto rects = MakeCandidates(points);

		std::sort(rects.begin(), rects.end(), [](const Candidate& a, const Candidate& b) {
			return a.area > b.area;
		});
		for (const auto& rect : rects) {
			if(validator(rect, edges)) {
				return rect.area;
			}
			if(IsValidRect(rect, edges)) {
				return rect.area;
			}
		}
		return 0;
	}
	PART(1) {
		auto alwaysTrue = [](const Candidate&, const std::vector<Edge>&) { return true; };
		return Solve(lines, alwaysTrue);
	}

	PART(2) {
		return Solve(lines, IsValidRect);
	}

	static_assert(Area({ 2,5 }, { 11,1 }) == 50);

	TEST(1) { 
		std::vector<std::string> input = {
			"7,1",
			"11,1",
			"11,7",
			"9,7",
			"9,5",
			"2,5",
			"2,3",
			"7,3"
		};

		return Solve(input, IsValidRect) == 24;
	}
}