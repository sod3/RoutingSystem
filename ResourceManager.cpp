#include "ResourceManager.h"
#include "utils.h"
#include "Graph.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

ResourceManager::ResourceManager() {}

void ResourceManager::addAmbulance(int id, int location) {
    Ambulance* ambulance = new Ambulance(id, location);
    ambulances.push_back(ambulance);
}

Ambulance* ResourceManager::findNearestAmbulance(int incidentLocation, Graph &graph) {
    Ambulance* nearest = nullptr;
    int minDistance = INT_MAX;
    
    for (Ambulance* ambulance : ambulances) {
        if (ambulance->isAvailable()) {
            int distance = graph.dijkstra(ambulance->getLocation(), incidentLocation);
            
            if (distance < minDistance) {
                minDistance = distance;
                nearest = ambulance;
            }
        }
    }
    
    return nearest;
}

vector<Ambulance*> ResourceManager::getAllAmbulances() {
    return ambulances;
}

vector<Ambulance*> ResourceManager::getAvailableAmbulances() {
    vector<Ambulance*> available;
    for (Ambulance* ambulance : ambulances) {
        if (ambulance->isAvailable()) {
            available.push_back(ambulance);
        }
    }
    return available;
}

int ResourceManager::getAvailableCount() {
    int count = 0;
    for (Ambulance* ambulance : ambulances) {
        if (ambulance->isAvailable()) {
            count++;
        }
    }
    return count;
}

void ResourceManager::displayAll() {
    cout << "\n=== Ambulance Status ===" << endl;
    if (ambulances.empty()) {
        cout << "No ambulances registered." << endl;
        return;
    }
    
    for (Ambulance* ambulance : ambulances) {
        ambulance->display();
    }
    cout << "Available: " << getAvailableCount() << "/" << ambulances.size() << endl;
}

void ResourceManager::loadFromFile(const string &filename) {
    ifstream file(filename);
    string line;
    
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return;
    }
    
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        vector<string> parts = split(line, ' ');
        if (parts.size() >= 2) {
            int id = stoi(parts[0]);
            int location = stoi(parts[1]);
            
            addAmbulance(id, location);
        }
    }
    
    file.close();
    cout << "Ambulances loaded from " << filename << endl;
}