#include "Graph.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

Graph::Graph() {}

void Graph::addNode(int nodeId) {
    if (find(nodes.begin(), nodes.end(), nodeId) == nodes.end()) {
        nodes.push_back(nodeId);
    }
} // adds a new location to map, avoid  duplicates using find()

void Graph::addEdge(int src, int dest, int weight) {
    addNode(src);
    addNode(dest);

    adjList[src].push_back({dest, weight}); // adds road from src to dest
    adjList[dest].push_back({src, weight}); // adds road from dest to src
}
// Adds a bidirectional road between two locations

void Graph::updateEdgeWeight(int src, int dest, int newWeight) {
    bool found = false;

    for (auto &n : adjList[src]) { // Update src→dest direction
        if (n.first == dest) {
            n.second = newWeight;
            found = true;
            break;
        }
    }

    for (auto &n : adjList[dest]) { // Update dest→src direction
        if (n.first == src) {
            n.second = newWeight;
            break;
        }
    }

    if (found)
        cout << "Road updated\n";
    else
        cout << "Road not found\n";
}

void Graph::markRoadBlocked(int src, int dest) {
    blockedRoads[{min(src, dest), max(src, dest)}] = true; // mark road as blocked in both directions
    cout << "Road blocked\n";
}


void Graph::markRoadOpen(int src, int dest) {
    blockedRoads.erase({min(src, dest), max(src, dest)});
    cout << "Road opened\n";
}

bool Graph::isRoadBlocked(int src, int dest) const {
    return blockedRoads.find({min(src, dest), max(src, dest)}) != blockedRoads.end();
}
// Checks if road is blocked returns: true if road is in blockedRoads map


vector<pair<int, int>> Graph::getNeighbors(int node) {
    return adjList[node];
} // Returns all roads connected to a node

vector<int> Graph::getAllNodes() {
    return nodes;
}

int Graph::dijkstra(int start, int end) {
    if (start == end)
        return 0;

    // like a to do list values will be pushed and sorted
    // in <int, int> first int is distance, second is node id
    priority_queue<pair<int, int>, // what we store
                   vector<pair<int, int>>, // How to store it
                   greater<pair<int, int>>> pq; // how to sort smallest first

    map<int, int> dist; // map to store distances
    for (int n : nodes)
        dist[n] = INT_MAX; // Initialize all distances to infinity

    dist[start] = 0; // Distance to start node is 0
    pq.push({0, start});

while (!pq.empty()) {
    // 5. Take the CLOSEST place from to-do list
    int currentDist = pq.top().first;   // Time to get here
    int currentNode = pq.top().second;  // Where we are
    pq.pop();  // Remove from to-do list
    
    // 6. Found destination? Return the time!
    if (currentNode == end)
        return currentDist;
    
    // 7. Skip if we found a better path already
    if (currentDist > dist[currentNode])
        continue;

    // 8. Check all roads from current location
    for (auto neighbor : adjList[currentNode]) {
        int nextNode = neighbor.first;   // The neighbor
        int roadTime = neighbor.second;  // Time to travel this road
        
        // 9. Calculate: time to current + time to neighbor
        int totalTime = currentDist + roadTime;
        
        // 10. Is this FASTER than previous best?
        if (totalTime < dist[nextNode]) {
            // YES! Found a better route
            dist[nextNode] = totalTime;           // Update diary
            pq.push({totalTime, nextNode});       // Add to to-do list
        }
    }
}
// 11. If loop ends without finding destination
return INT_MAX;  // Means "can't reach there"
}

int Graph::dijkstraWithBlocked(int start, int end) {
    if (start == end)
        return 0;

    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<pair<int, int>>> pq;

    map<int, int> dist;
    for (int n : nodes)
        dist[n] = INT_MAX;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int currentDist = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (currentNode == end)
            return currentDist;

        if (currentDist > dist[currentNode])
            continue;

        for (auto neighbor : adjList[currentNode]) {
            int nextNode = neighbor.first;
            int roadWeight = neighbor.second;

            if (isRoadBlocked(currentNode, nextNode))
                continue;

            int totalDist = currentDist + roadWeight;

            if (totalDist < dist[nextNode]) {
                dist[nextNode] = totalDist;
                pq.push({totalDist, nextNode});
            }
        }
    }
    return INT_MAX;
}

void Graph::loadFromFile(const string &filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "File error\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        auto parts = split(line, ' ');
        if (parts.size() >= 3) {
            addEdge(stoi(parts[0]), stoi(parts[1]), stoi(parts[2]));
        }
    }

    file.close();
    cout << "Graph loaded\n";
}

void Graph::saveToFile(const string &filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Save failed\n";
        return;
    }

    map<pair<int, int>, bool> done;

    for (int n : nodes) {
        for (auto nb : adjList[n]) {
            int a = min(n, nb.first);
            int b = max(n, nb.first);

            if (!done[{a, b}]) {
                file << a << " " << b << " " << nb.second << endl;
                done[{a, b}] = true;
            }
        }
    }

    file.close();
    cout << "Graph saved\n";
}

void Graph::display() {
    cout << "\nGraph\n";
    cout << "Nodes: " << nodes.size() << endl;

    if (!blockedRoads.empty()) { // If there are blocked roads
        cout << "Blocked: ";
        for (auto r : blockedRoads)
            cout << r.first.first << "-" << r.first.second << " ";
        cout << endl;
    }

    cout << "Roads:\n";

    map<pair<int, int>, bool> shown; // Tracks which roads we've already shown
    for (int n : nodes) {
        for (auto nb : adjList[n]) {
            int a = min(n, nb.first);
            int b = max(n, nb.first);
            // To normalize road representation, road 1-2 and Road 2-1 become the same: (1, 2), This prevents duplicates!

            if (!shown[{a, b}]) { // // Haven't shown this road yet
                cout << a << " <-> " << b << " (" << nb.second << ")";
                if (isRoadBlocked(a, b))
                    cout << " X";
                cout << endl;

                shown[{a, b}] = true;
            }
        }
    }
}

void Graph::displayBlockedRoads() {
    cout << "\nBlocked Roads\n";

    if (blockedRoads.empty()) {
        cout << "None\n";
        return;
    }

    for (auto r : blockedRoads) {
        cout << r.first.first << " - " << r.first.second << endl;
    }
}
