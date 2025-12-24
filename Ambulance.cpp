#include "Ambulance.h"
#include <iostream>
using namespace std;

Ambulance::Ambulance(int ambId, int loc) {
    id = ambId;
    location = loc;
    status = "AVAILABLE";
    assignedIncidentId = -1;
}

int Ambulance::getId() const { 
    return id; 
}

int Ambulance::getLocation() const { 
    return location; 
}

std::string Ambulance::getStatus() const { 
    return status; 
}

int Ambulance::getAssignedIncident() const { 
    return assignedIncidentId; 
}

bool Ambulance::isAvailable() const { 
    return status == "AVAILABLE"; 
}

void Ambulance::dispatchTo(int incidentId) {
    assignedIncidentId = incidentId;
    status = "BUSY";
}

void Ambulance::setAvailable() {
    assignedIncidentId = -1;
    status = "AVAILABLE";
}

void Ambulance::setLocation(int loc) {
    location = loc;
}

void Ambulance::display() const {
    cout << "Ambulance #" << id << ", Location: " << location << ", Status: " << status << endl;
}