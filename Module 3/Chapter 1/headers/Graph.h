#pragma once
#include <memory>
#include <set>
#include <unordered_map>

namespace graph
{
	struct Node; // pre-declared to use in Edge

	// =============================================================
	// Defines an Edge as an object that points
	// to another node, and has some cost.
	struct Edge
	{
		Edge(Node* aNodePtr, int aCost)
			: cost(aCost)
			, otherNodePtr(aNodePtr)
		{}
		friend bool operator<(Edge const& left, Edge const& right)
		{
			return left.cost < right.cost;
		}

		int cost;
		Node* otherNodePtr;
	};

	// =============================================================
	// Defines a Node as a named object
	// that has a set of edge connections.
	struct Node
	{
		char name;
		std::multiset<Edge> connections;
	};

	// =============================================================
	// Defines Graph as a collection of nodes
	// with character identifiers.
	struct Graph
	{
		void print() const;
		std::unordered_map<char, Node> nodes;
	};

	// =============================================================
	// Defines a Parser that can generate a graph from
	// an input CSV file.
	class Parser
	{
	public:
		Parser(const std::string& aFile);
		~Parser() = default;

		void setFile(const std::string& aFile);
		std::unique_ptr<Graph> parse();

	protected:
		std::string mFile;

		void tryAddNode(Graph& aGraph,
			            char aNode);

		void updateGraph(Graph& aGraph,
			             char aStartNode,
			             char aEndNode,
			             int aCost);

		bool parseStringAsChar(const std::string& aString,
			                   char& aChar) const;

		bool parseStringAsInt(const std::string& aString,
			                  int& aInt) const;

		void printParseError_Generic(size_t aLineNumber,
			                         const std::string& aError) const;

		void printParseError_BadValue(size_t aLineNumber,
			                          const std::string& aBadValue,
			                          const std::string& aExpected) const;
	};
}