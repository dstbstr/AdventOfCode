#include "Common.h"
#include "Core/Constexpr/ConstexprUnionFind.h"

SOLUTION(2025, 8) {
	// distance, idx1, idx2
	using Edge = std::tuple<double, size_t, size_t>;
	constexpr Vec3<size_t> ParseLine(const std::string& line) {
		return Vec3<size_t>(line);
	}

	constexpr std::vector<Edge> BuildEdges(const std::vector<Vec3<size_t>>& points) {
		std::vector<Edge> edges;
		for(size_t i = 0; i < points.size(); i++) {
			for(size_t j = i + 1; j < points.size(); j++) {
				auto distance = points[i].Dist(points[j]);
				edges.emplace_back(distance, i, j);
			}
		}
		return edges;
	}
	constexpr size_t Solve(std::vector<Vec3<size_t>> points, size_t count) {
		auto edges = BuildEdges(points);
		std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
			return std::get<0>(a) < std::get<0>(b);
			});

		UnionFind<size_t> uf(points.size());
		for(size_t i = 0; i < count; i++) {
			auto [distance, idx1, idx2] = edges[i];
			uf.Join(idx1, idx2);
		}

		std::vector<size_t> rootCounts(points.size(), 0);
		for(size_t i = 0; i < points.size(); i++) {
			rootCounts[uf.Find(i)]++;
		}
		std::sort(rootCounts.begin(), rootCounts.end(), std::greater<size_t>());
		return rootCounts[0] * rootCounts[1] * rootCounts[2];
	}

	constexpr size_t Solve2(const std::vector<Vec3<size_t>>& points) {
		auto edges = BuildEdges(points);
		std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
			return std::get<0>(a) < std::get<0>(b);
			});
		UnionFind<size_t> uf(points.size());
		size_t result = 0;
		for(auto [distance, id1, id2] : edges) {
			if (uf.Find(id1) == uf.Find(id2)) continue;
			uf.Join(id1, id2);
			result = points[id1].X * points[id2].X;
		}
		
		return result;
	}

	PART(1) {
		auto points = ParseLines(lines, ParseLine);
		return Solve(points, 1000);
	}
	PART(2) {
		auto points = ParseLines(lines, ParseLine);
		return Solve2(points);
	}
	static_assert(Vec3<size_t>("162,817,812").X == 162);

	static constexpr std::vector<std::string> sampleInput() {
		return {
		"162,817,812",
		"57,618,57",
		"906,360,560",
		"592,479,940",
		"352,342,300",
		"466,668,158",
		"542,29,236",
		"431,825,988",
		"739,650,466",
		"52,470,668",
		"216,146,977",
		"819,987,18",
		"117,168,530",
		"805,96,715",
		"346,949,466",
		"970,615,88",
		"941,993,340",
		"862,61,35",
		"984,92,344",
		"425,690,689"
		};
	}

	TEST(1) {
		auto points = ParseLines(sampleInput(), ParseLine);
		auto result = Solve(points, 10);
		return result == 40;
	}

	TEST(2) {
		auto points = ParseLines(sampleInput(), ParseLine);
		auto result = Solve2(points);
		return result == 25272;
	}
}