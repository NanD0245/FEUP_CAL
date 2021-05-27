#include "graph.h"

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
Vertex::Vertex(AirportID in): info(in) {}

void Vertex::addEdge(Edge* e) {
    outgoing.push_back(e);
}

bool Vertex::operator<(Vertex& vertex) const {
    return this->dist < vertex.dist;
}

AirportID Vertex::getInfo() const {
    return this->info;
}

const std::vector<Edge*>& Vertex::getOutgoing() const {
    return this->outgoing;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
bool Vertex::removeEdgeTo(Vertex* d) {
    for (size_t i = 0; i < outgoing.size(); i++) {
        if (outgoing[i]->dest->info == d->info) {
            outgoing.erase(outgoing.begin() + static_cast<long>(i));
            return true;
        }
    }
    return false;
}

/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */


Edge::Edge(Vertex* d, double w): dest{d}, weight{w} {}


double Edge::getWeight() const {
    return this->weight;
}

Vertex* Edge::getDest() const {
    return this->dest;
}

/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */


Vertex* Graph::addVertex(const AirportID& in) {
    Vertex* v = findVertex(in);
    if (v != nullptr)
        return v;
    v = new Vertex(in);
    vertexSet.push_back(v);
    return v;
}

Edge* Graph::addEdge(const AirportID& sourc, const AirportID& dest, double weight) const {
    auto* s = findVertex(sourc);
    auto* d = findVertex(dest);
    if (s == nullptr || d == nullptr)
        return nullptr;
    Edge* e = new Edge(d, weight);
    s->addEdge(e);
    return e;
}


Vertex* Graph::findVertex(const AirportID& inf) const {
    for (auto v : vertexSet)
        if (v->info == inf)
            return v;
    return nullptr;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
bool Graph::removeEdge(const AirportID& sourc, const AirportID& dest) {
    if ((findVertex(sourc)) == nullptr || (findVertex(dest)) == nullptr) {
        return false;
    }
    return findVertex(sourc)->removeEdgeTo(findVertex(dest));
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
bool Graph::removeVertex(const AirportID& in) {
    Vertex* a = findVertex(in);
    if (a == nullptr) {
        return false;
    }

    for (size_t i = 0; i < vertexSet.size(); i++) {
        if (vertexSet[i]->info == a->info) {
            vertexSet.erase(vertexSet.begin() + static_cast<long>(i));
        }
    }

    for (auto& i : vertexSet) {
        i->removeEdgeTo(a);
    }

    return true;
}

void Graph::clear() {
    for (auto* vertex : vertexSet) {
        for (auto* edge : vertex->outgoing) {
            delete edge;
        }
        delete vertex;
    }
    vertexSet.clear();
}

void Graph::dfsVisit(Vertex* v, std::vector<AirportID>& res) const {
    v->visited = true;
    res.push_back(v->info);
    for (Edge* e : v->outgoing) {
        if (!e->dest->visited) {
            dfsVisit(e->dest, res);
        }
    }
}

void Graph::tarjanStronglyConnectedComponents() {
    for (Vertex* vertex : vertexSet) {
        vertex->visited = false;
    }

    stronglyConnectedComponents.clear();
    std::stack<Vertex*> stack;
    size_t index = 0;

    for (Vertex* vertex : vertexSet) {
        if (!vertex->visited) {
            doTarjanSCC(index, stack, vertex);
        }
    }
}

template<typename T> constexpr T pop(std::stack<T>& stack) {
    T val = stack.top();
    stack.pop();
    return val;
}

void Graph::doTarjanSCC(size_t& index, std::stack<Vertex*>& stack, Vertex* vertex) {
    vertex->visited = true;
    vertex->index = index;
    vertex->low_link = index;
    index++;

    stack.push(vertex);
    vertex->on_stack = true;

    for (Edge* edge : vertex->outgoing) {
        Vertex* successor = edge->dest;
        if (!successor->visited) {
            doTarjanSCC(index, stack, successor);
            vertex->low_link = std::min(vertex->low_link, successor->low_link);
        } else if (successor->on_stack) {
            vertex->low_link = std::min(vertex->low_link, successor->index);
        }
    }

    if (vertex->low_link == vertex->index) {
        std::unordered_set<AirportID> scc;
        for (Vertex* w = pop(stack); w != vertex; w = pop(stack)) {
            w->on_stack = false;
            scc.emplace(w->info);
        }
        vertex->on_stack = false;
        scc.emplace(vertex->info);

        stronglyConnectedComponents.push_back(std::move(scc));
    }
}

const std::unordered_set<AirportID>& Graph::findSCC(AirportID id) const {
    return *std::find_if(stronglyConnectedComponents.cbegin(), stronglyConnectedComponents.cend(),
        [id](const auto& set) { return set.contains(id); });
}

void Graph::floydWarshallShortestPath() {
    dist.clear();
    next.clear();

    for (size_t i = 0; i < vertexSet.size(); i++) {
        dist.emplace_back(vertexSet.size(), INF);
        next.emplace_back(vertexSet.size());
        for (size_t j = 0; j < vertexSet.size(); j++) {
            if (i == j) {
                dist[i][j] = 0;
                next[i][j] = i;
                continue;
            }

            for (Edge* edge : vertexSet[i]->outgoing) {
                if (edge->dest == vertexSet[j]) {
                    dist[i][j] = edge->weight;
                    next[i][j] = j;
                    break;
                }
            }
        }
    }

    for (size_t k = 0; k < vertexSet.size(); k++) {
        for (size_t i = 0; i < vertexSet.size(); i++) {
            for (size_t j = 0; j < vertexSet.size(); j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

std::vector<AirportID> Graph::getFloydWarshallPath(AirportID orig, AirportID dest) const {
    std::vector<AirportID> path;
    auto current = static_cast<size_t>(orig);
    auto last = static_cast<size_t>(dest);

    while (current != last) {
        current = next[current][last];
        path.push_back(static_cast<AirportID>(current));
    }
    return path;
}

const std::vector<std::vector<double>>& Graph::getFloydWarshallDistances() const {
    return dist;
}

const std::vector<std::unordered_set<AirportID>>& Graph::getStronglyConnectedComponents() const {
    return stronglyConnectedComponents;
}
