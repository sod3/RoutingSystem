// Graph.cpp
#include "Graph.h"
#include "utils.h"
#include <iostream>
#include <fstream>

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
    adjList[src].push_back(make_pair(dest, weight));
    adjList[dest].push_back(make_pair(src, weight)); // undirected graph
}

vector<pair<int, int>> Graph::getNeighbors(int node) {
    return adjList[node];
}

vector<int> Graph::getAllNodes() {
    return nodes;
}

int Graph::dijkstra(int start, int end) {
    if (start == end) return 0;
    
    // Priority queue: (distance, node)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    // Distance map
    map<int, int> dist;
    for (int node : nodes) {
        dist[node] = INT_MAX;
    }
    
    // Start node
    dist[start] = 0;
    pq.push(make_pair(0, start));
    
    while (!pq.empty()) {
        int currentDist = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();
        
        if (currentNode == end) {
            return currentDist;
        }
        
        if (currentDist > dist[currentNode]) {
            continue;
        }
        
        // Check all neighbors
        for (auto neighbor : adjList[currentNode]) {
            int nextNode = neighbor.first;
            int weight = neighbor.second;
            
            int newDist = currentDist + weight;
            
            if (newDist < dist[nextNode]) {
                dist[nextNode] = newDist;
                pq.push(make_pair(newDist, nextNode));
            }
        }
    }
    
    return INT_MAX; // No path found
}

void Graph::loadFromFile(const string &filename) {
    ifstream file(filename);
    string line;
    
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return;
    }
    
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        vector<string> parts = split(line, ' ');
        if (parts.size() >= 3) {
            int src = stoi(parts[0]);
            int dest = stoi(parts[1]);
            int weight = stoi(parts[2]);
            
            addEdge(src, dest, weight);
        }
    }
    
    file.close();
    cout << "Graph loaded from " << filename << " with " << nodes.size() << " nodes." << endl;
}

void Graph::display() {
    cout << "\n=== Graph Structure ===" << endl;
    cout << "Total Nodes: " << nodes.size() << endl;
    cout << "\nEdges:" << endl;
    
    for (int node : nodes) {
        cout << "Node " << node << " -> ";
        for (auto neighbor : adjList[node]) {
            cout << neighbor.first << "(" << neighbor.second << ") ";
        }
        cout << endl;
    }
}