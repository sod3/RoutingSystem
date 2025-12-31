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
}

void Graph::addEdge(int src, int dest, int weight) {
    addNode(src);
    addNode(dest);

    adjList[src].push_back({dest, weight});
    adjList[dest].push_back({src, weight});
}

void Graph::updateEdgeWeight(int src, int dest, int newWeight) {
    bool found = false;

    for (auto &n : adjList[src]) {
        if (n.first == dest) {
            n.second = newWeight;
            found = true;
            break;
        }
    }

    for (auto &n : adjList[dest]) {
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
    blockedRoads[{min(src, dest), max(src, dest)}] = true;
    cout << "Road blocked\n";
}

void Graph::markRoadOpen(int src, int dest) {
    blockedRoads.erase({min(src, dest), max(src, dest)});
    cout << "Road opened\n";
}

bool Graph::isRoadBlocked(int src, int dest) const {
    return blockedRoads.find({min(src, dest), max(src, dest)}) != blockedRoads.end();
}

vector<pair<int, int>> Graph::getNeighbors(int node) {
    return adjList[node];
}

vector<int> Graph::getAllNodes() {
    return nodes;
}

int Graph::dijkstra(int start, int end) {
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
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (u == end)
            return d;

        if (d > dist[u])
            continue;

        for (auto nb : adjList[u]) {
            int v = nb.first;
            int w = nb.second;

            if (d + w < dist[v]) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }

    return INT_MAX;
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
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (u == end)
            return d;

        if (d > dist[u])
            continue;

        for (auto nb : adjList[u]) {
            int v = nb.first;
            int w = nb.second;

            if (isRoadBlocked(u, v))
                continue;

            if (d + w < dist[v]) {
                dist[v] = d + w;
                pq.push({dist[v], v});
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

    if (!blockedRoads.empty()) {
        cout << "Blocked: ";
        for (auto r : blockedRoads)
            cout << r.first.first << "-" << r.first.second << " ";
        cout << endl;
    }

    cout << "Roads:\n";

    map<pair<int, int>, bool> shown;
    for (int n : nodes) {
        for (auto nb : adjList[n]) {
            int a = min(n, nb.first);
            int b = max(n, nb.first);

            if (!shown[{a, b}]) {
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
