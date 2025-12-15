#ifndef INCIDENT_H
#define INCIDENT_H

#include <queue>
#include <vector>
#include <string>

using namespace std;

class Graph;

class Incident {
private:
    int id;
    int location;  // node ID
    string priority; // "HIGH", "MEDIUM", "LOW"
    string description;
    bool resolved;

public:
    Incident(int loc, const string &pri, const string &desc);
    
    // Getters
    int getId() const;
    int getLocation() const;
    string getPriority() const;
    string getDescription() const;
    bool isResolved() const;
    
    // Priority comparison (HIGH > MEDIUM > LOW)
    int getPriorityValue() const;
    
    void resolve();
    
    void display() const;
};

// Custom comparator for priority queue
struct CompareIncidentPriority {
    bool operator()(const Incident* a, const Incident* b);
};

class IncidentQueue {
private:
    priority_queue<Incident*, vector<Incident*>, CompareIncidentPriority> pq;
    vector<Incident*> allIncidents;
    
public:
    IncidentQueue();
    
    void addIncident(int location, const string &priority, const string &description);
    
    void reAddIncident(Incident* inc);
    
    Incident* getNextIncident();
    
    bool isEmpty() const;
    
    int size() const;
    
    void displayAll() const;
    
    void loadFromFile(const string &filename, Graph &graph);
};

#endif