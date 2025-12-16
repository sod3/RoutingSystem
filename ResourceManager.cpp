#include "ResourceManager.h"
#include "utils.h"
#include "Graph.h"
#include "Incident.h"
#include <iostream>
#include <fstream>
#include <climits>
#include <algorithm>

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
    for (auto amb : ambulances) {
        delete amb;
    }
    ambulances.clear();
}

void ResourceManager::addAmbulance(int id, int location) {
    for (auto amb : ambulances) {
        if (amb->getId() == id) {
            std::cout << "Ambulance #" << id << " already exists!" << std::endl;
            return;
        }
    }
    ambulances.push_back(new Ambulance(id, location));
    std::cout << "Added Ambulance #" << id << " at Node " << location << std::endl;
}

void ResourceManager::addAmbulanceInteractive() {
    int id, location;
    
    std::cout << "Enter ambulance ID: ";
    std::cin >> id;
    
    std::cout << "Enter starting location (node): ";
    std::cin >> location;
    
    addAmbulance(id, location);
}

bool ResourceManager::removeAmbulance(int id) {
    for (auto it = ambulances.begin(); it != ambulances.end(); ++it) {
        if ((*it)->getId() == id) {
            if (!(*it)->isAvailable()) {
                std::cout << "Cannot remove Ambulance #" << id << " - currently assigned to Incident #" 
                          << (*it)->getAssignedIncident() << std::endl;
                return false;
            }
            
            delete *it;
            ambulances.erase(it);
            std::cout << "Removed Ambulance #" << id << std::endl;
            return true;
        }
    }
    
    std::cout << "Ambulance #" << id << " not found!" << std::endl;
    return false;
}

Ambulance* ResourceManager::findNearestAmbulance(int incidentLocation, Graph &graph) {
    Ambulance* nearest = nullptr;
    int closest = INT_MAX;
    
    for (auto amb : ambulances) {
        if (amb->isAvailable()) {
            int dist = graph.dijkstra(amb->getLocation(), incidentLocation);
            if (dist < closest) {
                closest = dist;
                nearest = amb;
            }
        }
    }
    
    return nearest;
}

Ambulance* ResourceManager::findAmbulanceById(int id) {
    for (auto amb : ambulances) {
        if (amb->getId() == id) {
            return amb;
        }
    }
    return nullptr;
}

bool ResourceManager::dispatchAmbulance(int ambulanceId, int incidentId, int incidentLocation) {
    Ambulance* ambulance = findAmbulanceById(ambulanceId);
    if (!ambulance) {
        std::cout << "Ambulance #" << ambulanceId << " not found!" << std::endl;
        return false;
    }
    
    if (!ambulance->isAvailable()) {
        std::cout << "Ambulance #" << ambulanceId << " is not available!" << std::endl;
        return false;
    }
    
    ambulance->dispatchTo(incidentId);
    std::cout << "Dispatched Ambulance #" << ambulanceId << " to Incident #" << incidentId << std::endl;
    return true;
}

void ResourceManager::completeAssignment(int ambulanceId) {
    Ambulance* ambulance = findAmbulanceById(ambulanceId);
    if (ambulance) {
        int incidentId = ambulance->getAssignedIncident();
        ambulance->setAvailable();
        std::cout << "Ambulance #" << ambulanceId << " completed assignment to Incident #" 
                  << incidentId << std::endl;
    }
}

void ResourceManager::reassignAmbulances(IncidentQueue &incidents, Graph &graph) {
    std::cout << "\nReassigning Ambulances" << std::endl;
    
    std::vector<Incident*> activeIncidents;
    std::vector<Ambulance*> busyAmbulances;
    
    if (incidents.isEmpty()) {
        std::cout << "No incidents to reassign." << std::endl;
        return;
    }
    
    int reassignments = 0;
    
    while (!incidents.isEmpty()) {
        Incident* incident = incidents.getNextIncident();
        if (incident && !incident->isResolved()) {
            activeIncidents.push_back(incident);
            
            Ambulance* nearest = findNearestAmbulance(incident->getLocation(), graph);
            if (nearest) {
                int oldAssignment = nearest->getAssignedIncident();
                nearest->dispatchTo(incident->getId());
                nearest->setLocation(incident->getLocation());
                
                std::cout << "Reassigned Ambulance #" << nearest->getId() 
                         << " to Incident #" << incident->getId() << " at Node " 
                         << incident->getLocation() << std::endl;
                
                reassignmentLog.push_back(std::make_pair(nearest->getId(), incident->getId()));
                reassignments++;
                
                if (oldAssignment != -1) {
                    std::cout << "  (Was assigned to Incident #" << oldAssignment << ")" << std::endl;
                }
            }
        }
    }
    
    for (auto incident : activeIncidents) {
        incidents.reAddIncident(incident);
    }
    
    std::cout << "Completed " << reassignments << " reassignments." << std::endl;
}

std::vector<Ambulance*> ResourceManager::getAllAmbulances() {
    return ambulances;
}

std::vector<Ambulance*> ResourceManager::getAvailableAmbulances() {
    std::vector<Ambulance*> available;
    for (auto amb : ambulances) {
        if (amb->isAvailable()) {
            available.push_back(amb);
        }
    }
    return available;
}

int ResourceManager::getAvailableCount() {
    int count = 0;
    for (auto amb : ambulances) {
        if (amb->isAvailable()) {
            count++;
        }
    }
    return count;
}

void ResourceManager::displayAll() {
    std::cout << "\nAmbulance Status" << std::endl;
    if (ambulances.empty()) {
        std::cout << "No ambulances registered." << std::endl;
        return;
    }
    
    for (auto amb : ambulances) {
        amb->display();
    }
    std::cout << "Available: " << getAvailableCount() << "/" << ambulances.size() << std::endl;
}

void ResourceManager::displayReassignmentLog() {
    std::cout << "\nReassignment History" << std::endl;
    if (reassignmentLog.empty()) {
        std::cout << "No reassignments recorded." << std::endl;
        return;
    }
    
    for (const auto& log : reassignmentLog) {
        std::cout << "Ambulance #" << log.first << " -> Incident #" << log.second << std::endl;
    }
    std::cout << "Total reassignments: " << reassignmentLog.size() << std::endl;
}

void ResourceManager::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cout << "Can't open " << filename << std::endl;
        return;
    }
    
    for (auto amb : ambulances) {
        delete amb;
    }
    ambulances.clear();
    
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        auto parts = split(line, ' ');
        if (parts.size() >= 2) {
            int id = std::stoi(parts[0]);
            int location = std::stoi(parts[1]);
            addAmbulance(id, location);
        }
    }
    
    file.close();
    std::cout << "Ambulances loaded from " << filename << std::endl;
}

void ResourceManager::saveToFile(const std::string &filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "Can't create " << filename << std::endl;
        return;
    }
    
    file << "# Ambulance data: ID Location" << std::endl;
    file << "# Saved by Emergency Routing System" << std::endl;
    
    for (auto amb : ambulances) {
        file << amb->getId() << " " << amb->getLocation() << std::endl;
    }
    
    file.close();
    std::cout << "Ambulance fleet saved to " << filename << std::endl;
}

void ResourceManager::clearReassignmentLog() {
    reassignmentLog.clear();
    std::cout << "Reassignment log cleared." << std::endl;
}