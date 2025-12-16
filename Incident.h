#ifndef INCIDENT_H
#define INCIDENT_H

#include <queue>
#include <vector>
#include <string>

class Graph;

class Incident {
    int id;
    int location;
    std::string priority;
    std::string description;
    bool resolved;

public:
    Incident(int loc, const std::string &pri, const std::string &desc);
    
    int getId() const;
    int getLocation() const;
    std::string getPriority() const;
    std::string getDescription() const;
    bool isResolved() const;
    int getPriorityValue() const;
    
    void resolve();
    void display() const;
};

struct CompareIncidentPriority {
    bool operator()(const Incident* a, const Incident* b);
};

class IncidentQueue {
    std::priority_queue<Incident*, std::vector<Incident*>, CompareIncidentPriority> pq;
    std::vector<Incident*> allIncidents;
    
public:
    IncidentQueue();
    ~IncidentQueue();
    
    void addIncident(int location, const std::string &priority, const std::string &description);
    void reAddIncident(Incident* inc);
    Incident* getNextIncident();
    bool isEmpty() const;
    int size() const;
    int getActiveCount() const;
    void displayAll() const;
    void loadFromFile(const std::string &filename, Graph &graph);
    void saveToFile(const std::string &filename) const;
    void generateTestIncidents(int count, Graph &graph);
    void clearAll();
};

#endif