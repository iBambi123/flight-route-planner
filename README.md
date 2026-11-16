# Flight Route Planner

A C++ command-line program that finds the cheapest flight route between two US airports using Dijkstra's algorithm.

## Example

```
From (or quit): LAX
To: JFK
Cheapest route: LAX -> DEN -> ORD -> JFK
Total cost: $295
```

The direct LAX to JFK flight in the sample data costs $320, so the planner picks the cheaper three-flight route.

## How it works

- **Graph:** airports are stored in an adjacency list, a `std::map` from each airport code to a `std::vector` of its direct routes. Every route is two-way.
- **Dijkstra's algorithm:** a min-heap (`std::priority_queue`) always expands the cheapest airport reached so far. The search stops when the destination comes off the heap, because its cost is final at that point.
- **Path rebuilding:** the search records the previous airport for each airport it reaches, then walks backwards from the destination to list the route.

Running time is O((V + E) log V) for V airports and E routes.

## Build and run

```
g++ -std=c++17 -Wall *.cpp -o planner
./planner
```

To use a different route file:

```
./planner my_routes.txt
```

## Route file format

One route per line, as `FROM,TO,COST`:

```
LAX,DEN,110
DEN,ORD,95
```

Lines that are not in this format are skipped with a message.

The fares in `routes.txt` are sample values for demonstration, not real prices.

## Error handling

- Unknown airport codes are reported
- Airports with no connecting route are reported
- Airport codes can be typed in upper or lower case

## Files

| File | Purpose |
|------|---------|
| `Graph.h`, `Graph.cpp` | Graph storage, file loading, and the Dijkstra search |
| `main.cpp` | Command-line interface |
| `routes.txt` | Sample routes between 10 US airports |
