#include "ResourceManager.h"
#include "utils.h"
#include "Graph.h"
#include "Incident.h"
#include <iostream>
#include <fstream>
#include <climits>
#include <algorithm>
#include <limits>

using namespace std;

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
            cout << "Ambulance already exists\n";
            return;
        }
    }

    ambulances.push_back(new Ambulance(id, location));
    cout << "Ambulance added\n";
}
// Adds new ambulance to the fleet
// Checks if ambulance ID already exists (no duplicates)
// Creates new Ambulance object with given ID and location
// Adds it to the list

void ResourceManager::addAmbulanceInteractive() {
    int id, location;
    
    cout << "Enter ID: ";
    while (!(cin >> id)) {
        cout << "Invalid input! Please enter a valid number for ID: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Enter location: ";
    while (!(cin >> location)) {
        cout << "Invalid input! Please enter a valid number for location: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    addAmbulance(id, location);
}
// Interactive version asks user for input and adds ambulance

bool ResourceManager::removeAmbulance(int id) {
    for (auto it = ambulances.begin(); it != ambulances.end(); it++) {
        if ((*it)->getId() == id) {
            if (!(*it)->isAvailable()) {
                cout << "Ambulance is busy\n";
                return false;
            }

            delete *it;
            ambulances.erase(it);
            cout << "Ambulance removed\n";
            return true;
        }
    }

    cout << "Ambulance not found\n";
    return false;
}
// Removes an ambulance from the system
// Won't remove if ambulance is busy (on a call)

Ambulance* ResourceManager::findNearestAmbulance(int incidentLocation, Graph &graph) {
    Ambulance* nearest = nullptr;
    int minDist = INT_MAX;

    for (auto amb : ambulances) {
        if (amb->isAvailable()) {
            int dist = graph.dijkstra(amb->getLocation(), incidentLocation);
            if (dist < minDist) {
                minDist = dist;
                nearest = amb;
            }
        }
    }

    return nearest;
}
// Finds the closest available ambulance to an emergency
// Uses Dijkstra's algorithm (from Graph class) to find shortest path distance

Ambulance* ResourceManager::findAmbulanceById(int id) {
    for (auto amb : ambulances) {
        if (amb->getId() == id)
            return amb;
    }
    return nullptr;
}
// Finds ambulance by its ID number


bool ResourceManager::dispatchAmbulance(int ambulanceId, int incidentId, int incidentLocation) {
    Ambulance* amb = findAmbulanceById(ambulanceId);

    if (!amb) {
        cout << "Ambulance not found\n";
        return false;
    }

    if (!amb->isAvailable()) {
        cout << "Ambulance not available\n";
        return false;
    }

    amb->dispatchTo(incidentId);  // Dispatch ambulance to incident function in Ambulance class
    cout << "Ambulance dispatched\n";
    return true;
}
// Sends a specific ambulance to a specific incident

void ResourceManager::completeAssignment(int ambulanceId) {
    Ambulance* amb = findAmbulanceById(ambulanceId);

    if (amb) {
        amb->setAvailable();
        cout << "Assignment completed\n";
    }
}
// Marks an ambulance as available after completing its job

void ResourceManager::reassignAmbulances(IncidentQueue &incidents, Graph &graph) {
    cout << "\nReassigning...\n";

    if (incidents.isEmpty()) {
        cout << "No incidents\n";
        return;
    }

    vector<Incident*> temp;
    int count = 0;

    while (!incidents.isEmpty()) {
        Incident* inc = incidents.getNextIncident(); // Get next incident from the queue
        if (inc && !inc->isResolved()) {
            temp.push_back(inc);

            Ambulance* amb = findNearestAmbulance(inc->getLocation(), graph);
            if (amb) {
                amb->dispatchTo(inc->getId());
                amb->setLocation(inc->getLocation());

                reassignmentLog.push_back({amb->getId(), inc->getId()});
                count++;
            }
        }
    }

    for (auto inc : temp)
        incidents.reAddIncident(inc);

    cout << "Done (" << count << " reassigned)\n";
}
// Reassigns all ambulances to optimize response to all pending incidents
vector<Ambulance*> ResourceManager::getAllAmbulances() {
    return ambulances;
}

vector<Ambulance*> ResourceManager::getAvailableAmbulances() {
    vector<Ambulance*> list;
    for (auto amb : ambulances) {
        if (amb->isAvailable())
            list.push_back(amb);
    }
    return list;
}

int ResourceManager::getAvailableCount() {
    int c = 0;
    for (auto amb : ambulances) {
        if (amb->isAvailable())
            c++;
    }
    return c;
}

void ResourceManager::displayAll() {
    cout << "\nAmbulances:\n";

    if (ambulances.empty()) {
        cout << "None\n";
        return;
    }

    for (auto amb : ambulances) // Display each ambulance's details
        amb->display();

    cout << "Available: " << getAvailableCount() << "/" << ambulances.size() << endl;
}

void ResourceManager::displayReassignmentLog() {
    cout << "\nReassignments:\n";

    if (reassignmentLog.empty()) {
        cout << "No history\n";
        return;
    }

    for (auto log : reassignmentLog) {
        cout << log.first << " -> " << log.second << endl;
    }
}

void ResourceManager::loadFromFile(const string &filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "File error\n";
        return;
    }

    for (auto amb : ambulances)
        delete amb;
    ambulances.clear();

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        auto parts = split(line, ' ');
        if (parts.size() >= 2) {
            addAmbulance(stoi(parts[0]), stoi(parts[1]));
        }
    }

    file.close();
    cout << "Loaded from file\n";
}

void ResourceManager::saveToFile(const string &filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Save failed\n";
        return;
    }

    for (auto amb : ambulances) {
        file << amb->getId() << " " << amb->getLocation() << endl;
    }

    file.close();
    cout << "Saved\n";
}

void ResourceManager::clearReassignmentLog() {
    reassignmentLog.clear();
    cout << "Log cleared\n";
}
