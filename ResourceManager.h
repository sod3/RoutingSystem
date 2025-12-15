#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <vector>
#include <climits>
#include <string>
#include "Ambulance.h"

class Graph;

class ResourceManager {
private:
    std::vector<Ambulance*> ambulances;
    
public:
    ResourceManager();
    
    void addAmbulance(int id, int location);
    
    // Find nearest available ambulance to incident location
    Ambulance* findNearestAmbulance(int incidentLocation, Graph &graph);
    
    std::vector<Ambulance*> getAllAmbulances();
    
    std::vector<Ambulance*> getAvailableAmbulances();
    
    int getAvailableCount();
    
    void displayAll();
    
    void loadFromFile(const std::string &filename);
};

#endif