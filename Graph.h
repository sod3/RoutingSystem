#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <queue>
#include <climits>
#include <string>
using namespace std;

class Graph {
    map<int, vector<pair<int, int>>> adjList;
    vector<int> nodes;
    map<pair<int, int>, bool> blockedRoads;

public:
    Graph();
    void addNode(int nodeId);
    void addEdge(int src, int dest, int weight);
    void updateEdgeWeight(int src, int dest, int newWeight);
    void markRoadBlocked(int src, int dest);
    void markRoadOpen(int src, int dest);
    bool isRoadBlocked(int src, int dest) const;
    vector<pair<int, int>> getNeighbors(int node);
    vector<int> getAllNodes();
    int dijkstra(int start, int end);
    int dijkstraWithBlocked(int start, int end);
    void loadFromFile(const string &filename);
    void saveToFile(const string &filename);
    void display();
    void displayBlockedRoads();
};

#endif