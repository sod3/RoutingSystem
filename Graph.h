// Graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <queue>
#include <climits>
#include <string>
#include <algorithm> // for find

using namespace std;

class Graph {
private:
    // node -> list of (neighbor, weight)
    map<int, vector<pair<int, int>>> adjList;
    
    // Store all node IDs
    vector<int> nodes;

public:
    Graph();
    
    // Add node to graph
    void addNode(int nodeId);
    
    // Add edge between two nodes
    void addEdge(int src, int dest, int weight);
    
    // Get neighbors of a node
    vector<pair<int, int>> getNeighbors(int node);
    
    // Get all nodes
    vector<int> getAllNodes();
    
    // Dijkstra's Algorithm - returns shortest path distance
    int dijkstra(int start, int end);
    
    // Load graph from file
    void loadFromFile(const string &filename);
    
    // Display graph
    void display();
};

#endif