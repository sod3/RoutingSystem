#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <queue>
#include <climits>
#include <string>

class Graph {
    std::map<int, std::vector<std::pair<int, int>>> adjList;
    std::vector<int> nodes;
    std::map<std::pair<int, int>, bool> blockedRoads;

public:
    Graph();
    void addNode(int nodeId);
    void addEdge(int src, int dest, int weight);
    void updateEdgeWeight(int src, int dest, int newWeight);
    void markRoadBlocked(int src, int dest);
    void markRoadOpen(int src, int dest);
    bool isRoadBlocked(int src, int dest) const;
    std::vector<std::pair<int, int>> getNeighbors(int node);
    std::vector<int> getAllNodes();
    int dijkstra(int start, int end);
    int dijkstraWithBlocked(int start, int end);
    void loadFromFile(const std::string &filename);
    void saveToFile(const std::string &filename);
    void display();
    void displayBlockedRoads();
};

#endif