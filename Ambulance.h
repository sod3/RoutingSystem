#ifndef AMBULANCE_H
#define AMBULANCE_H

#include <string>

using namespace std;

class Ambulance {
private:
    int id;
    int location;  // current node ID
    string status; // "AVAILABLE", "BUSY", "MAINTENANCE"
    int assignedIncidentId; // -1 if not assigned
    
public:
    Ambulance(int ambId, int loc);
    
    // Getters
    int getId() const;
    int getLocation() const;
    string getStatus() const;
    int getAssignedIncident() const;
    
    bool isAvailable() const;
    
    void dispatchTo(int incidentId);
    
    void setAvailable();
    
    void setLocation(int loc);
    
    void display() const;
};

#endif