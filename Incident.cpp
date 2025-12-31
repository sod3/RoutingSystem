#include "Incident.h"
#include "utils.h"
#include "Graph.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

using namespace std;

/* ---------- Incident ---------- */

Incident::Incident(int loc, const string &pri, const string &desc) {
    static int nextId = 1;
    id = nextId++;
    location = loc;
    priority = pri;
    description = desc;
    resolved = false;
}

int Incident::getId() const {
    return id;
}

int Incident::getLocation() const {
    return location;
}

string Incident::getPriority() const {
    return priority;
}

string Incident::getDescription() const {
    return description;
}

bool Incident::isResolved() const {
    return resolved;
}

int Incident::getPriorityValue() const {
    if (priority == "HIGH") return 3;
    if (priority == "MEDIUM") return 2;
    return 1;
}

void Incident::resolve() {
    resolved = true;
}

void Incident::display() const {
    cout << "Incident " << id
         << " | Node " << location
         << " | " << priority
         << " | " << description;

    if (resolved)
        cout << " (done)";
    else
        cout << " (active)";

    cout << endl;
}

/* ---------- Priority Compare ---------- */

bool CompareIncidentPriority::operator()(const Incident* a, const Incident* b) {
    return a->getPriorityValue() < b->getPriorityValue();
}

/* ---------- Incident Queue ---------- */

IncidentQueue::IncidentQueue() {
    srand(time(0));
}

IncidentQueue::~IncidentQueue() {
    clearAll();
}

void IncidentQueue::addIncident(int location, const string &priority, const string &description) {
    Incident* inc = new Incident(location, priority, description);
    pq.push(inc);
    allIncidents.push_back(inc);

    cout << "Incident added\n";
}

void IncidentQueue::reAddIncident(Incident* inc) {
    pq.push(inc);
}

Incident* IncidentQueue::getNextIncident() {
    if (pq.empty())
        return nullptr;

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

int IncidentQueue::getActiveCount() const {
    int count = 0;
    for (auto inc : allIncidents) {
        if (!inc->isResolved())
            count++;
    }
    return count;
}

void IncidentQueue::displayAll() const {
    cout << "\nIncidents:\n";

    if (allIncidents.empty()) {
        cout << "None\n";
        return;
    }

    int active = 0, done = 0;

    for (auto inc : allIncidents) {
        inc->display();
        if (inc->isResolved())
            done++;
        else
            active++;
    }

    cout << "Total: " << allIncidents.size()
         << " | Active: " << active
         << " | Resolved: " << done << endl;
}

void IncidentQueue::loadFromFile(const string &filename, Graph &graph) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "File error\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        auto parts = split(line, ',');
        if (parts.size() >= 3) {
            int loc = stoi(parts[0]);
            string pri = parts[1];
            string desc = parts[2];

            auto nodes = graph.getAllNodes();
            if (find(nodes.begin(), nodes.end(), loc) != nodes.end()) {
                addIncident(loc, pri, desc);
            }
        }
    }

    file.close();
    cout << "Incidents loaded\n";
}

void IncidentQueue::saveToFile(const string &filename) const {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Save failed\n";
        return;
    }

    for (auto inc : allIncidents) {
        if (!inc->isResolved()) {
            file << inc->getLocation() << ","
                 << inc->getPriority() << ","
                 << inc->getDescription() << endl;
        }
    }

    file.close();
    cout << "Saved\n";
}

void IncidentQueue::generateTestIncidents(int count, Graph &graph) {
    if (count <= 0) {
        cout << "Invalid count\n";
        return;
    }

    auto nodes = graph.getAllNodes();
    if (nodes.empty()) {
        cout << "No map loaded\n";
        return;
    }

    vector<string> priorities = {"HIGH", "MEDIUM", "LOW"};
    vector<string> types = {
        "Accident", "Medical", "Fire",
        "Gas Leak", "Flood", "Rescue"
    };

    for (int i = 0; i < count; i++) {
        int loc = nodes[rand() % nodes.size()];
        string pri = priorities[rand() % priorities.size()];
        string type = types[rand() % types.size()];

        string desc = type + " case";
        addIncident(loc, pri, desc);
    }

    cout << count << " test incidents added\n";
}

void IncidentQueue::clearAll() {
    while (!pq.empty())
        pq.pop();

    for (auto inc : allIncidents)
        delete inc;

    allIncidents.clear();
    cout << "Incidents cleared\n";
}
