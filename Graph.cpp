#include "Graph.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <algorithm>

Graph::Graph() {}

void Graph::addNode(int nodeId) {
    if (std::find(nodes.begin(), nodes.end(), nodeId) == nodes.end()) {
        nodes.push_back(nodeId);
    }
}

void Graph::addEdge(int src, int dest, int weight) {
    addNode(src);
    addNode(dest);
    adjList[src].push_back(std::make_pair(dest, weight));
    adjList[dest].push_back(std::make_pair(src, weight));
}

void Graph::updateEdgeWeight(int src, int dest, int newWeight) {
    bool updated = false;
    
    for (auto& neighbor : adjList[src]) {
        if (neighbor.first == dest) {
            neighbor.second = newWeight;
            updated = true;
            break;
        }
    }
    
    for (auto& neighbor : adjList[dest]) {
        if (neighbor.first == src) {
            neighbor.second = newWeight;
            break;
        }
    }
    
    if (updated) {
        std::cout << "Updated road weight between " << src << " and " << dest 
                  << " to " << newWeight << std::endl;
    } else {
        std::cout << "Road between " << src << " and " << dest 
                  << " not found!" << std::endl;
    }
}

void Graph::markRoadBlocked(int src, int dest) {
    blockedRoads[std::make_pair(std::min(src, dest), std::max(src, dest))] = true;
    std::cout << "Road between " << src << " and " << dest << " marked as BLOCKED" << std::endl;
}

void Graph::markRoadOpen(int src, int dest) {
    auto key = std::make_pair(std::min(src, dest), std::max(src, dest));
    blockedRoads.erase(key);
    std::cout << "Road between " << src << " and " << dest << " marked as OPEN" << std::endl;
}

bool Graph::isRoadBlocked(int src, int dest) const {
    auto key = std::make_pair(std::min(src, dest), std::max(src, dest));
    return blockedRoads.find(key) != blockedRoads.end();
}

std::vector<std::pair<int, int>> Graph::getNeighbors(int node) {
    return adjList[node];
}

std::vector<int> Graph::getAllNodes() {
    return nodes;
}

int Graph::dijkstra(int start, int end) {
    if (start == end) return 0;
    
    std::priority_queue<std::pair<int, int>, 
                        std::vector<std::pair<int, int>>, 
                        std::greater<std::pair<int, int>>> pq;
    
    std::map<int, int> dist;
    for (int node : nodes) {
        dist[node] = INT_MAX;
    }
    
    dist[start] = 0;
    pq.push(std::make_pair(0, start));
    
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
        
        for (auto neighbor : adjList[currentNode]) {
            int nextNode = neighbor.first;
            int weight = neighbor.second;
            
            int newDist = currentDist + weight;
            
            if (newDist < dist[nextNode]) {
                dist[nextNode] = newDist;
                pq.push(std::make_pair(newDist, nextNode));
            }
        }
    }
    
    return INT_MAX;
}

int Graph::dijkstraWithBlocked(int start, int end) {
    if (start == end) return 0;
    
    std::priority_queue<std::pair<int, int>, 
                        std::vector<std::pair<int, int>>, 
                        std::greater<std::pair<int, int>>> pq;
    
    std::map<int, int> dist;
    for (int node : nodes) {
        dist[node] = INT_MAX;
    }
    
    dist[start] = 0;
    pq.push(std::make_pair(0, start));
    
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
        
        for (auto neighbor : adjList[currentNode]) {
            int nextNode = neighbor.first;
            int weight = neighbor.second;
            
            if (isRoadBlocked(currentNode, nextNode)) {
                continue;
            }
            
            int newDist = currentDist + weight;
            
            if (newDist < dist[nextNode]) {
                dist[nextNode] = newDist;
                pq.push(std::make_pair(newDist, nextNode));
            }
        }
    }
    
    return INT_MAX;
}

void Graph::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cout << "Error: Cannot open file " << filename << std::endl;
        return;
    }
    
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::vector<std::string> parts = split(line, ' ');
        if (parts.size() >= 3) {
            int src = std::stoi(parts[0]);
            int dest = std::stoi(parts[1]);
            int weight = std::stoi(parts[2]);
            
            addEdge(src, dest, weight);
        }
    }
    
    file.close();
    std::cout << "Graph loaded from " << filename << " with " << nodes.size() << " nodes." << std::endl;
}

void Graph::saveToFile(const std::string &filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "Error: Cannot create file " << filename << std::endl;
        return;
    }
    
    file << "# Graph data: source destination weight" << std::endl;
    file << "# Saved by Emergency Routing System" << std::endl;
    
    std::map<std::pair<int, int>, bool> writtenEdges;
    
    for (int node : nodes) {
        for (auto neighbor : adjList[node]) {
            int src = std::min(node, neighbor.first);
            int dest = std::max(node, neighbor.first);
            auto edgeKey = std::make_pair(src, dest);
            
            if (writtenEdges.find(edgeKey) == writtenEdges.end()) {
                file << src << " " << dest << " " << neighbor.second << std::endl;
                writtenEdges[edgeKey] = true;
            }
        }
    }
    
    file.close();
    std::cout << "Graph saved to " << filename << std::endl;
}

void Graph::display() {
    std::cout << "\nGraph Structure" << std::endl;
    std::cout << "Total Nodes: " << nodes.size() << std::endl;
    
    if (!blockedRoads.empty()) {
        std::cout << "Blocked Roads: ";
        for (const auto& road : blockedRoads) {
            std::cout << road.first.first << "-" << road.first.second << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "\nEdges:" << std::endl;
    
    std::map<std::pair<int, int>, bool> displayedEdges;
    
    for (int node : nodes) {
        for (auto neighbor : adjList[node]) {
            int src = std::min(node, neighbor.first);
            int dest = std::max(node, neighbor.first);
            auto edgeKey = std::make_pair(src, dest);
            
            if (displayedEdges.find(edgeKey) == displayedEdges.end()) {
                std::cout << "Road " << src << " <-> " << dest 
                         << " (weight: " << neighbor.second << ")";
                if (isRoadBlocked(src, dest)) {
                    std::cout << " [BLOCKED]";
                }
                std::cout << std::endl;
                displayedEdges[edgeKey] = true;
            }
        }
    }
}

void Graph::displayBlockedRoads() {
    std::cout << "\nBlocked Roads" << std::endl;
    if (blockedRoads.empty()) {
        std::cout << "No roads are currently blocked." << std::endl;
        return;
    }
    
    for (const auto& road : blockedRoads) {
        std::cout << "Road between " << road.first.first 
                  << " and " << road.first.second << " is BLOCKED" << std::endl;
    }
}