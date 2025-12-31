#ifndef INCIDENT_H
#define INCIDENT_H

#include <queue>
#include <vector>
#include <string>
using namespace std;

class Graph;

class Incident {
    int id;
    int location;
    string priority;
    string description;
    bool resolved;

public:
    Incident(int loc, const string &pri, const string &desc);
    
    int getId() const;
    int getLocation() const;
    string getPriority() const;
    string getDescription() const;
    bool isResolved() const;
    int getPriorityValue() const;
    
    void resolve();
    void display() const;
};

struct CompareIncidentPriority {
    bool operator()(const Incident* a, const Incident* b);
};

class IncidentQueue {
    priority_queue<Incident*, vector<Incident*>, CompareIncidentPriority> pq;
    vector<Incident*> allIncidents;
    
public:
    IncidentQueue();
    ~IncidentQueue();
    
    void addIncident(int location, const string &priority, const string &description);
    void reAddIncident(Incident* inc);
    Incident* getNextIncident();
    bool isEmpty() const;
    int size() const;
    int getActiveCount() const;
    void displayAll() const;
    void loadFromFile(const string &filename, Graph &graph);
    void saveToFile(const string &filename) const;
    void generateTestIncidents(int count, Graph &graph);
    void clearAll();
};

#endif