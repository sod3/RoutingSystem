#ifndef AMBULANCE_H
#define AMBULANCE_H

#include <string>

class Ambulance {
    int id;
    int location;
    std::string status;
    int assignedIncidentId;
    
public:
    Ambulance(int ambId, int loc);
    
    int getId() const;
    int getLocation() const;
    std::string getStatus() const;
    int getAssignedIncident() const;
    
    bool isAvailable() const;
    
    void dispatchTo(int incidentId);
    void setAvailable();
    void setLocation(int loc);
    
    void display() const;
};

#endif