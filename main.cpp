#include <cctype>
#include <iostream>
#include <string>

#include "Graph.h"

// Converts an airport code to upper case, so "lax" matches "LAX".
std::string toUpper(std::string text) {
    for (char& c : text) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return text;
}

int main(int argc, char* argv[]) {
    // Use the file named on the command line, or routes.txt by default.
    std::string filename = "routes.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    Graph graph;
    if (!graph.loadFromFile(filename)) {
        std::cout << "Could not open " << filename << std::endl;
        return 1;
    }

    std::cout << "Flight Route Planner" << std::endl;
    std::cout << "Airports and direct routes:" << std::endl;
    graph.print();
    std::cout << std::endl;

    while (true) {
        std::string from, to;

        std::cout << "From (or quit): ";
        if (!(std::cin >> from) || toUpper(from) == "QUIT") {
            break;
        }
        std::cout << "To: ";
        if (!(std::cin >> to)) {
            break;
        }
        from = toUpper(from);
        to = toUpper(to);

        if (!graph.hasAirport(from)) {
            std::cout << "Unknown airport: " << from << std::endl << std::endl;
            continue;
        }
        if (!graph.hasAirport(to)) {
            std::cout << "Unknown airport: " << to << std::endl << std::endl;
            continue;
        }

        TripResult trip = graph.findCheapestRoute(from, to);
        if (!trip.found) {
            std::cout << "No route from " << from << " to " << to << std::endl
                      << std::endl;
            continue;
        }

        std::cout << "Cheapest route: ";
        for (size_t i = 0; i < trip.path.size(); ++i) {
            if (i > 0) {
                std::cout << " -> ";
            }
            std::cout << trip.path[i];
        }
        std::cout << std::endl;
        std::cout << "Total cost: $" << trip.totalCost << std::endl
                  << std::endl;
    }

    std::cout << "Goodbye" << std::endl;
    return 0;
}
