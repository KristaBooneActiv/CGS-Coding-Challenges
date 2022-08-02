#pragma once
#include <deque>

#include "Graph.h"
#include "IChallenge.h"

class Challenge2 : public IChallenge
{
public:
	Challenge2() = default;
	~Challenge2() = default;

	std::string details() const override;
	void run() override;

protected:
	struct SearchResult
	{
		std::deque<char> nodePath;
		int cost;
	};
	std::unique_ptr<SearchResult> findMinPath(const graph::Graph& aGraph,
		                                      char from,
		                                      char to) const;

	int computeHeuristic(const graph::Graph& aGraph,
		                 char nodeId,
		                 char goalId) const;
};