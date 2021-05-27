/*
 * Graph.h.
 * For implementation of the minimum cost flow algorithm.
 * See TODOs for code to add/adapt.
 * FEUP, CAL, 2017/18.
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <unordered_set>
#include <vector>

#include "airport.h"

constexpr auto INF = std::numeric_limits<double>::max();

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */

class Edge;

class Vertex {
    AirportID info;
    std::vector<Edge*> outgoing;

    bool visited;  // for path finding
    double dist;  // for path finding
    size_t queueIndex = 0;  // required by MutablePriorityQueue

    // used by Tarjan's SCC algorithm
    size_t index;
    size_t low_link;
    bool on_stack;

    explicit Vertex(AirportID in);
    void addEdge(Edge* e);
    bool operator<(Vertex& vertex) const;  // required by MutablePriorityQueue
    bool removeEdgeTo(Vertex* d);

public:
    [[nodiscard]] AirportID getInfo() const;
    [[nodiscard]] const std::vector<Edge*>& getOutgoing() const;
    friend class Graph;
    friend class MutablePriorityQueue;
};

/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */

class Edge {
    Vertex* dest;
    double weight;

    Edge(Vertex* dest, double weight = 0);

public:
    friend class Graph;
    friend class Vertex;
    [[nodiscard]] double getWeight() const;
    [[nodiscard]] Vertex* getDest() const;
};


/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */

class Graph {
    std::vector<Vertex*> vertexSet;
    std::vector<std::unordered_set<AirportID>> stronglyConnectedComponents;

    // floyd
    std::vector<std::vector<double>> dist;
    std::vector<std::vector<size_t>> next;

    void dfsVisit(Vertex* v, std::vector<AirportID>& res) const;
    void doTarjanSCC(size_t& index, std::stack<Vertex*>& stack, Vertex* vertex);


public:
    [[nodiscard]] Vertex* findVertex(const AirportID& inf) const;
    Vertex* addVertex(const AirportID& in);
    Edge* addEdge(const AirportID& sourc, const AirportID& dest, double weight) const;
    bool removeVertex(const AirportID& in);
    bool removeEdge(const AirportID& sourc, const AirportID& dest);
    void clear();

    /**
     * Tarjan's algorithm
     */
    void tarjanStronglyConnectedComponents();

    /**
     * @return reference to the strongly connected components
     */
    [[nodiscard]] const std::vector<std::unordered_set<AirportID>>& getStronglyConnectedComponents() const;

    /**
     * Return the strongly connected component set that the specified airport belongs to
     *
     * @param id ID of the airport
     * @return A strongly connected component set
     */
    [[nodiscard]] const std::unordered_set<AirportID>& findSCC(AirportID id) const;

    /**
     * floyd-warshall algorithm
     */
    void floydWarshallShortestPath();

    /**
     * get the shortest path between two airports
     *
     * @param orig
     * @param dest
     * @return the vector that contains the airports of the shortest path
     */
    [[nodiscard]] std::vector<AirportID> getFloydWarshallPath(AirportID orig, AirportID dest) const;

    /**
     * @return reference to the matrix that contains the distances
     */
    [[nodiscard]] const std::vector<std::vector<double>>& getFloydWarshallDistances() const;
};

#endif /* GRAPH_H_ */
