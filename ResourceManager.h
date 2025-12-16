#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <vector>
#include <string>
#include "Ambulance.h"

class Graph;
class IncidentQueue;

class ResourceManager {
    std::vector<Ambulance*> ambulances;
    std::vector<std::pair<int, int>> reassignmentLog;
    
public:
    ResourceManager();
    ~ResourceManager();
    
    void addAmbulance(int id, int location);
    void addAmbulanceInteractive();
    bool removeAmbulance(int id);
    Ambulance* findNearestAmbulance(int incidentLocation, Graph &graph);
    Ambulance* findAmbulanceById(int id);
    
    void reassignAmbulances(IncidentQueue &incidents, Graph &graph);
    bool dispatchAmbulance(int ambulanceId, int incidentId, int incidentLocation);
    void completeAssignment(int ambulanceId);
    
    std::vector<Ambulance*> getAllAmbulances();
    std::vector<Ambulance*> getAvailableAmbulances();
    int getAvailableCount();
    
    void displayAll();
    void displayReassignmentLog();
    void loadFromFile(const std::string &filename);
    void saveToFile(const std::string &filename);
    
    void clearReassignmentLog();
};

#endif