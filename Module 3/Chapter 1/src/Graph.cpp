#include <iostream>
#include <fstream>
#include <sstream>

#include "Graph.h"
#include "Utilities.h"

using namespace graph;

// =============================================================
void Graph::print() const
{
	std::cout << "Graph contains " << nodes.size() << " nodes" << std::endl;
	for (auto& n : nodes)
	{
		std::cout << "\tNode " << n.second.name << " has connections [ ";
		for (auto& c : n.second.connections)
		{
			std::cout << "(" << c.otherNodePtr->name << ":" << c.cost << ") ";
		}
		std::cout << "]" << std::endl;
	}
	std::cout << std::endl;
}

// =============================================================
Parser::Parser(const std::string& aFile)
	: mFile(aFile)
{ /* no-op */ }

void Parser::setFile(const std::string& aFile)
{
	mFile = aFile;
}

std::unique_ptr<Graph> Parser::parse()
{
	// Try to open file
	std::ifstream in(mFile);
	if (in.fail())
	{
		std::cerr << "ERROR: Could not open file: " << mFile << std::endl;
		return nullptr;
	}

	auto graphPtr = std::make_unique<Graph>();

	// Read in graph from the file
	size_t lineId = 1;
	std::string line;
	while (std::getline(in, line))
	{
		char startNodeId;
		char endNodeId;
		int  cost;

		std::stringstream ss(line);
		std::string token;

		static const int cEXPECTED_TOKENS_PER_LINE = 3;
		int tokenCount = 0;
		while (std::getline(ss, token, ','))
		{
			if (tokenCount == 0)
			{
				if (!parseStringAsChar(token, startNodeId))
				{
					printParseError_BadValue(lineId, token, "Expected character for starting node id");
					return nullptr;
				}
			}
			else if (tokenCount == 1)
			{
				if (!parseStringAsChar(token, endNodeId))
				{
					printParseError_BadValue(lineId, token, "Expected character for ending node id");
					return nullptr;
				}
			}
			else if (tokenCount == 2)
			{
				if (!parseStringAsInt(token, cost))
				{
					printParseError_BadValue(lineId, token, "Expected numeric value for edge cost");
					return nullptr;
				}
			}
			else
			{
				printParseError_Generic(lineId, "An unexpected tokenCount value has been found");
				return nullptr;
			}
			++tokenCount;
			++lineId;
		}

		if (tokenCount != cEXPECTED_TOKENS_PER_LINE)
		{
			printParseError_Generic(lineId,
				"Expected 3 comma-separated values, but found " + std::to_string(tokenCount));
			return nullptr;
		}

		// We got a good line! Let's add a node to the graph.
		updateGraph(*graphPtr, startNodeId, endNodeId, cost);
	}

	// Error if no nodes found
	if (graphPtr->nodes.empty())
	{
		std::cerr << "ERROR: Did not find any nodes in graph file: " << mFile << std::endl;
		return nullptr;
	}

	return graphPtr;
}

void Parser::tryAddNode(Graph& aGraph, char aNode)
{
	auto iter = aGraph.nodes.find(aNode);
	if (iter == aGraph.nodes.end())
	{
		// Create and add the node if it is not in aGraph
		Node newNode;
		newNode.name = aNode;

		aGraph.nodes[aNode] = newNode;
	}
}

void Parser::updateGraph(graph::Graph& aGraph, char aStartNode, char aEndNode, int aCost)
{
	tryAddNode(aGraph, aStartNode);
	tryAddNode(aGraph, aEndNode);

	// Add the edge to each node
	Node* start = &aGraph.nodes[aStartNode];
	Node* end = &aGraph.nodes[aEndNode];

	start->connections.insert(Edge(end, aCost));
	end->connections.insert(Edge(start, aCost));
}

bool Parser::parseStringAsChar(const std::string& aString, char& aChar) const
{
	if (aString.length() == 1)
	{
		aChar = aString[0];
		return true;
	}
	return false;
}

bool Parser::parseStringAsInt(const std::string& aString, int& aInt) const
{
	if (util::isInteger(aString))
	{
		aInt = std::stoi(aString);
		return true;
	}
	return false;
}

void Parser::printParseError_Generic(size_t aLineNumber, const std::string& aError) const
{
	std::cerr << "Error parsing file at line " << aLineNumber << ": " << aError << std::endl;
}

void Parser::printParseError_BadValue(size_t aLineNumber, const std::string& aBadValue, const std::string& aExpected) const
{
	std::cerr << "Bad value \"" << aBadValue << "\""
		<< " at line " << aLineNumber << ": " << aExpected << std::endl;
}
