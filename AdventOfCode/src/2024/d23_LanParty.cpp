#include "Common.h"

SOLUTION(2024, 23) {
	using Graph = Constexpr::BigMap<std::string_view, std::vector<std::string_view>, 1000>;
	constexpr Graph GraphConnections(const std::vector<std::string>& lines) {
		Graph graph;
		for(const auto& line : lines) {
			auto s = Constexpr::Split(line, "-");
			graph[s[0]].push_back(s[1]);
			graph[s[1]].push_back(s[0]);
		}
		return graph;
	}

	constexpr bool IsConnected(const Graph& graph, const std::vector<std::string_view>& nodes) {
		for (size_t i = 0; i < nodes.size(); i++) {
			const auto& lhs = nodes[i];
			for (size_t j = i + 1; j < nodes.size(); j++) {
				const auto& rhs = nodes[j];
				auto rhsConnections = graph.at(rhs);
				if (std::find(rhsConnections.begin(), rhsConnections.end(), lhs) == rhsConnections.end()) {
					return false;
				}
			}
		}
		return true;
	}

	constexpr std::vector<std::vector<std::string_view>> SubGraphs(const Graph& graph) {
		std::vector<std::vector<std::string_view>> subGraphs;
		for (const auto& [node, connections] : graph.GetAllEntries()) {
			//for each connection, check if it has 2 neighbors that connect to each other
			for (size_t i = 0; i < connections.size(); i++) {
				const auto& lhs = connections[i];
				std::vector<std::string_view> subGraph = { lhs };
				subGraph.reserve(3);
				for (size_t j = i + 1; j < connections.size(); j++) {
					auto rhs = connections[j];
					subGraph.emplace_back(rhs);
					if (IsConnected(graph, {lhs, rhs})) {
						subGraph.push_back(node);
						subGraphs.push_back(subGraph);
						subGraph.pop_back();
					}
					subGraph.pop_back();
				}
			}
		}

		for (auto& subGraph : subGraphs) {
			std::sort(subGraph.begin(), subGraph.end());
		}
		std::sort(subGraphs.begin(), subGraphs.end());
		subGraphs.erase(std::unique(subGraphs.begin(), subGraphs.end()), subGraphs.end());

		return subGraphs;
	}

	constexpr std::vector<std::vector<std::string_view>> SubGraphsWithT(const std::vector<std::vector<std::string_view>>& subGraphs) {
		return subGraphs
			| std::views::filter([](const auto& connections) { return std::ranges::any_of(connections, [](const auto& node) { return node[0] == 't'; }); })
			| std::ranges::to<std::vector>();
	}

	PART(1) {
		auto graph = GraphConnections(lines);
		auto subGraphs = SubGraphs(graph);
		auto subGraphsWithT = SubGraphsWithT(subGraphs);
		return subGraphsWithT.size();
	}

	constexpr std::vector<std::string_view> Recurse(const Graph& graph, const std::vector<std::string_view>& subGraph, std::vector<std::string_view> remaining, size_t bestSoFar) {
		// for each connection in the node graph
		// either add it to the subgraph or skip
		// when adding, check if all nodes are connected
		if (remaining.empty()) return subGraph;
		if (subGraph.size() + remaining.size() <= bestSoFar) return subGraph;

		std::vector<std::string_view> result;

		while(!remaining.empty()) {
			auto withNode = subGraph;
			auto withoutNode = subGraph;
			withNode.emplace_back(remaining.back());
			remaining.pop_back();
			if (IsConnected(graph, withNode)) {
				auto next = Recurse(graph, withNode, remaining, bestSoFar);
				if (next.size() > result.size()) {
					result = next;
				}
			}
			auto next = Recurse(graph, withoutNode, remaining, bestSoFar);
			if (next.size() > result.size()) {
				result = next;
			}
		}

		return result;
	}

	constexpr std::vector<std::string_view> FindLargestSubgraph(const Graph& graph) {
		std::vector<std::string_view> result;
		for(const auto& [node, connections] : graph.GetAllEntries()) {
			std::vector<std::string_view> subGraph = { node };
			auto maxSubgraph = Recurse(graph, subGraph, connections, result.size());
			if (maxSubgraph.size() > result.size()) {
				result = maxSubgraph;
			}
		}
		return result;
	}

	PART(2) {
		auto graph = GraphConnections(lines);
		auto largest = FindLargestSubgraph(graph);
		std::sort(largest.begin(), largest.end());
		return Constexpr::JoinVec(",", largest);
	}
	TEST(1) {
		std::vector<std::string> lines = {
			"kh-tc",
			"qp-kh",
			"de-cg",
			"ka-co",
			"yn-aq",
			"qp-ub",
			"cg-tb",
			"vc-aq",
			"tb-ka",
			"wh-tc",
			"yn-cg",
			"kh-ub",
			"ta-co",
			"de-co",
			"tc-td",
			"tb-wq",
			"wh-td",
			"ta-ka",
			"td-qp",
			"aq-cg",
			"wq-ub",
			"ub-vc",
			"de-ta",
			"wq-aq",
			"wq-vc",
			"wh-yn",
			"ka-de",
			"kh-ta",
			"co-tc",
			"wh-qp",
			"tb-vc",
			"td-yn"
		};

		return std::get<size_t>(Part1(lines)) == 7
			&& std::get<std::string>(Part2(lines)) == "co,de,ka,ta";
	}
}