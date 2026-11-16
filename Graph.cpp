#include "Graph.h"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <utility>

void Graph::addRoute(const std::string& from, const std::string& to,
                     int cost) {
    // Flights go both ways, so store the route under both airports.
    routes_[from].push_back({to, cost});
    routes_[to].push_back({from, cost});
}

bool Graph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        return false;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        if (line.empty()) {
            continue;
        }

        // Split the line on commas: FROM,TO,COST
        std::stringstream parts(line);
        std::string from, to, costText;
        std::getline(parts, from, ',');
        std::getline(parts, to, ',');
        std::getline(parts, costText, ',');

        try {
            int cost = std::stoi(costText);
            if (from.empty() || to.empty() || cost < 0) {
                throw std::invalid_argument("bad route");
            }
            addRoute(from, to, cost);
        } catch (const std::exception&) {
            std::cout << "Skipping line " << lineNumber << ": " << line
                      << std::endl;
        }
    }
    return true;
}

bool Graph::hasAirport(const std::string& airport) const {
    return routes_.find(airport) != routes_.end();
}

TripResult Graph::findCheapestRoute(const std::string& from,
                                    const std::string& to) const {
    TripResult result;
    if (!hasAirport(from) || !hasAirport(to)) {
        return result;
    }

    // cheapest[x] = lowest known cost to reach airport x from the start.
    std::map<std::string, int> cheapest;
    // previous[x] = the airport we came from on the cheapest way to x.
    std::map<std::string, std::string> previous;

    // Min-heap of (cost so far, airport): the cheapest option is always on top.
    typedef std::pair<int, std::string> Entry;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> toVisit;

    cheapest[from] = 0;
    toVisit.push({0, from});

    while (!toVisit.empty()) {
        int costSoFar = toVisit.top().first;
        std::string airport = toVisit.top().second;
        toVisit.pop();

        // A cheaper way to this airport was already processed.
        if (costSoFar > cheapest[airport]) {
            continue;
        }
        // The first time the destination comes off the heap, its cost is final.
        if (airport == to) {
            break;
        }

        for (const Route& route : routes_.at(airport)) {
            int newCost = costSoFar + route.cost;
            bool firstVisit =
                cheapest.find(route.destination) == cheapest.end();
            if (firstVisit || newCost < cheapest[route.destination]) {
                cheapest[route.destination] = newCost;
                previous[route.destination] = airport;
                toVisit.push({newCost, route.destination});
            }
        }
    }

    // The destination was never reached.
    if (cheapest.find(to) == cheapest.end()) {
        return result;
    }

    // Walk backwards from the destination to rebuild the path.
    std::string airport = to;
    while (airport != from) {
        result.path.push_back(airport);
        airport = previous[airport];
    }
    result.path.push_back(from);
    std::reverse(result.path.begin(), result.path.end());

    result.found = true;
    result.totalCost = cheapest[to];
    return result;
}

void Graph::print() const {
    for (const auto& entry : routes_) {
        std::cout << entry.first << ": ";
        const std::vector<Route>& routes = entry.second;
        for (size_t i = 0; i < routes.size(); ++i) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << routes[i].destination << " ($" << routes[i].cost
                      << ")";
        }
        std::cout << std::endl;
    }
}
