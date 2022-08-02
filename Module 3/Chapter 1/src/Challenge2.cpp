#include <iostream>
#include <map>
#include <queue>

#include "Challenge2.h"
#include "Utilities.h"

std::string Challenge2::details() const
{
	return "Reads graph coordinates from a CSV file and uses A* to find paths requested by the user.";
}

void Challenge2::run()
{
	std::cout << "Enter the name of the file containing the graph: ";

	std::string filename;
	std::cin >> filename;

	graph::Parser parser(filename);
	std::unique_ptr<graph::Graph> graphPtr = parser.parse();

	if (!graphPtr)
	{
		std::cerr << "Fatal error occurred while parsing graph file. Exiting..." << std::endl;
		return;
	}

	// Print out the graph
	std::cout << "Read the following graph from " << filename << "..." << std::endl;
	graphPtr->print();

	bool doPrompt = true;

	while (doPrompt)
	{
		// Ask the user to select a path
		std::cout << "Give me two nodes, and I'll tell you if it's possible to get there, "
			<< " and if so, give you the cheapest way to get there!" << std::endl;

		char start;
		std::cout << "Enter the first node: ";
		std::cin >> start;

		char end;
		std::cout << "Enter the second node: ";
		std::cin >> end;

		auto result = findMinPath(*graphPtr, start, end);
		if (!result)
		{
			std::cout << "\nSorry, but there is no valid path from " << start << " to " << end << "!" << std::endl;
		}
		else
		{
			std::cout << "\nThere is a path from " << start << " to " << end << "!" << std::endl;
			std::cout << "Cost: " << result->cost << std::endl;
			std::cout << "Path: [ ";
			for (auto& nodeId : result->nodePath)
			{
				std::cout << nodeId << " ";
			}
			std::cout << "]" << std::endl;
		}

		std::cout << "\nWould you like to do another search? (Press 'y' for yes, or any other key for no): ";

		char option;
		std::cin >> option;

		if (option != 'y')
		{
			doPrompt = false;
		}
	}

	std::cout << "\nThank you, goodbye!" << std::endl;
}

int Challenge2::computeHeuristic(const graph::Graph& aGraph, char nodeId, char goalId) const
{
	// Return H(n) as the node's smallest cost to the next node
	// (Note that connections are pre-sorted by cost, so that the 
	// smallest cost is the first in the connections list)
	return aGraph.nodes.at(nodeId).connections.begin()->cost;
}

struct AStarNode
{
	AStarNode* parentPtr {nullptr};
	int  fn {0};
	int  gn {0};
	int  hn {0};
	char id;
};

// Wrapper for pointer so that we can compare. -_-
struct AStarNodePtr
{
	AStarNodePtr(AStarNode* aPtr) : ptr(aPtr) {}
	friend bool operator<(AStarNodePtr const& left, AStarNodePtr const& right)
	{
		// Prioritize lower cost nodes
		return left.ptr->fn < right.ptr->fn;
	}
	AStarNode* ptr;
};

enum AStarNodeState
{
	eOpen,
	eClosed
};

std::unique_ptr<Challenge2::SearchResult> Challenge2::findMinPath(const graph::Graph& aGraph, char startId, char goalId) const
{
	// All the nodes we've created while working through the algorithm
	std::unordered_map<char, AStarNode> aStarNodes;

	// Quick access to know if an astar node is in the open/closed list
	// If the node id is not in the map, it has not been identified yet.
	std::unordered_map<char, AStarNodeState> aStarNodeState;

	// OPEN contains nodes that have been visited but not been fully explored
	std::multiset<AStarNodePtr> open;

	// CLOSED contains nodes that have been visited and fully explored
	std::multiset<AStarNodePtr> closed;

	// Put start node in the open list with an initial total cost (fn) = hn
	AStarNode start;
	start.id = startId;                                    // Set id
	start.parentPtr = nullptr;                             // No parent
	start.gn = 0;                                          // No paths traveled yet
	start.hn = computeHeuristic(aGraph, startId, goalId);  // Set heuristic
	start.fn = start.gn + start.hn;                        // Compute fn = gn + hn
	aStarNodes[start.id] = start;                          // Add to map of astar nodes
	open.insert(&start);                                 // Insert into open node list

	aStarNodeState[start.id] = eOpen;

	// While the open list is not empty
	AStarNode* currentNodePtr = nullptr;
	while (!open.empty())
	{
		// Grab the node from the open list with the lowest f(n)
		// Because OPEN is a sorted list, it will be at the front
		currentNodePtr = (*open.begin()).ptr;
		open.erase(open.begin());

		// If current node is node goal- break
		if (currentNodePtr->id == goalId)
		{
			break;
		}

		// For each successor (connection) to the open node
		for (auto& edge : aGraph.nodes.at(currentNodePtr->id).connections)
		{
			AStarNode* successorPtr (nullptr);
			auto successorIter = aStarNodes.find(edge.otherNodePtr->name);
			if (successorIter == aStarNodes.end())
			{
				// Create an A* node for this graph node
				// where the initial cost is just the heuristic value for the node
				AStarNode newNode;
				newNode.id = edge.otherNodePtr->name;
				newNode.hn = computeHeuristic(aGraph, newNode.id, goalId);
				newNode.fn = newNode.hn;
				aStarNodes[newNode.id] = newNode;
				successorPtr = &aStarNodes[newNode.id];
			}
			else
			{
				successorPtr = &(successorIter->second);
			}

			// If successor is parent ptr, don't look at it.
			if (currentNodePtr->parentPtr && (successorPtr->id == currentNodePtr->parentPtr->id))
			{
				continue;
			}

			// Calculate f(n) = g(n) + h(n)
			// Where:
			// f(n) is the total cost to travel from the current open node to the next node
			// g(n) is the sum of edge costs to travel from start to this successor node
			// h(n) is the heuristic value for this successor node to the goal node
			int gn = currentNodePtr->gn + edge.cost;
			int currentSuccessorCost = gn + successorPtr->hn;

			auto successorStateIter = aStarNodeState.find(successorPtr->id);
			bool nodeVisited = (successorStateIter != aStarNodeState.end());
			if (nodeVisited && successorStateIter->second == AStarNodeState::eOpen)
			{
				// If successor is in the OPEN list,
				// If g(successor) <= successor_current_cost, continue
				if (successorPtr->fn <= currentSuccessorCost)
				{
					continue;
				}
				// Else, we move down to set new, lower cost with this new path
			}
			else if (nodeVisited && successorStateIter->second == AStarNodeState::eClosed)
			{
				// Else if successor is in CLOSED list,
				// if g(successor) <= successor_current_cost, continue
				if (successorPtr->fn <= currentSuccessorCost)
				{
					continue;
				}
				else
				{
					// Move node_successor from CLOSED to OPEN
					aStarNodeState[successorPtr->id] = eOpen;
					open.insert(successorPtr);
					closed.erase(successorPtr);
					// We move down to set new, lower cost with this new path
				}
			}

			// Set costs for this successor
			successorPtr->fn = currentSuccessorCost;
			successorPtr->gn = gn;

			if (!nodeVisited)
			{
				// If this is a newly discovered node, add successor to the open list
				open.insert(successorPtr);
				aStarNodeState[successorPtr->id] = eOpen;
			}

			// Set parent of successor to current node
			successorPtr->parentPtr = currentNodePtr;
		}

		// Move current node to closed list
		closed.insert(currentNodePtr);
		aStarNodeState[currentNodePtr->id] = eClosed;
	}

	if (currentNodePtr->id != goalId)
	{
		// Unable to find a path!
		return nullptr;
	}

	// Build result from
	auto result = std::make_unique<SearchResult>();
	result->cost = currentNodePtr->gn;

	AStarNode* pathNodePtr = currentNodePtr;
	while (pathNodePtr != nullptr)
	{
		result->nodePath.push_front(pathNodePtr->id);
		pathNodePtr = pathNodePtr->parentPtr;
	}
	return std::move(result);
}
