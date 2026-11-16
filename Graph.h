#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <string>
#include <vector>

// One direct flight out of an airport.
struct Route {
    std::string destination;
    int cost;
};

// The answer to a cheapest-route search.
struct TripResult {
    bool found = false;             // false if no route connects the airports
    int totalCost = 0;              // total fare in dollars
    std::vector<std::string> path;  // airports in order, start to destination
};

// A network of airports connected by two-way flights.
class Graph {
public:
    // Adds a two-way route between two airports with a cost in dollars.
    void addRoute(const std::string& from, const std::string& to, int cost);

    // Reads routes from a file with one "FROM,TO,COST" entry per line.
    // Returns false if the file cannot be opened.
    bool loadFromFile(const std::string& filename);

    // Returns true if the airport is in the graph.
    bool hasAirport(const std::string& airport) const;

    // Finds the cheapest route between two airports with Dijkstra's algorithm.
    TripResult findCheapestRoute(const std::string& from,
                                 const std::string& to) const;

    // Prints every airport followed by its direct routes and costs.
    void print() const;

private:
    // Adjacency list: each airport code maps to its list of direct routes.
    std::map<std::string, std::vector<Route>> routes_;
};

#endif
