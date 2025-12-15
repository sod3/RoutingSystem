#include "Incident.h"
#include "utils.h"
#include "Graph.h"
#include <iostream>
#include <fstream>
#include <algorithm> // for find

using namespace std;

Incident::Incident(int loc, const string &pri, const string &desc) {
    static int nextId = 1;
    id = nextId++;
    location = loc;
    priority = pri;
    description = desc;
    resolved = false;
}

int Incident::getId() const { return id; }
int Incident::getLocation() const { return location; }
string Incident::getPriority() const { return priority; }
string Incident::getDescription() const { return description; }
bool Incident::isResolved() const { return resolved; }

int Incident::getPriorityValue() const {
    if (priority == "HIGH") return 3;
    if (priority == "MEDIUM") return 2;
    return 1;
}

void Incident::resolve() { resolved = true; }

void Incident::display() const {
    cout << "Incident #" << id << " at Node " << location 
         << " [" << priority << "]: " << description 
         << (resolved ? " (RESOLVED)" : " (ACTIVE)") << endl;
}

bool CompareIncidentPriority::operator()(const Incident* a, const Incident* b) {
    return a->getPriorityValue() < b->getPriorityValue(); // Higher priority first
}

IncidentQueue::IncidentQueue() {}

void IncidentQueue::addIncident(int location, const string &priority, const string &description) {
    Incident* incident = new Incident(location, priority, description);
    pq.push(incident);
    allIncidents.push_back(incident);
    cout << "Added: ";
    incident->display();
}

void IncidentQueue::reAddIncident(Incident* inc) {
    pq.push(inc);
}

Incident* IncidentQueue::getNextIncident() {
    if (pq.empty()) return nullptr;
    
    Incident* next = pq.top();
    pq.pop();
    return next;
}

bool IncidentQueue::isEmpty() const {
    return pq.empty();
}

int IncidentQueue::size() const {
    return pq.size();
}

void IncidentQueue::displayAll() const {
    cout << "\n=== All Incidents ===" << endl;
    if (allIncidents.empty()) {
        cout << "No incidents recorded." << endl;
        return;
    }
    
    for (Incident* incident : allIncidents) {
        incident->display();
    }
    cout << "Total: " << allIncidents.size() << " incidents" << endl;
}

void IncidentQueue::loadFromFile(const string &filename, Graph &graph) {
    ifstream file(filename);
    string line;
    
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return;
    }
    
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        vector<string> parts = split(line, ',');
        if (parts.size() >= 3) {
            int location = stoi(parts[0]);
            string priority = parts[1];
            string description = parts[2];
            
            // Check if location exists in graph
            vector<int> nodes = graph.getAllNodes();
            if (find(nodes.begin(), nodes.end(), location) != nodes.end()) {
                addIncident(location, priority, description);
            } else {
                cout << "Warning: Node " << location << " not in graph. Skipping incident." << endl;
            }
        }
    }
    
    file.close();
    cout << "Incidents loaded from " << filename << endl;
}