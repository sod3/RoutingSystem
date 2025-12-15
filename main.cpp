#include "Graph.h"
#include "Incident.h"
#include "ResourceManager.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <queue>

using namespace std;

// Demo function showing core features
void runDemo() {
    cout << "========================================" << endl;
    cout << "EMERGENCY ROUTING SYSTEM - LIVE DEMO" << endl;
    cout << "========================================" << endl;
    
    // 1. Load and display graph
    cout << "\n1. LOADING CITY MAP..." << endl;
    Graph cityGraph;
    cityGraph.loadFromFile("map_small.txt");
    cityGraph.display();
    
    // 2. Load and display ambulances
    cout << "\n2. LOADING AMBULANCES..." << endl;
    ResourceManager rm;
    rm.loadFromFile("ambulances.txt");
    rm.displayAll();
    
    // 3. Create incident queue and load incidents
    cout << "\n3. LOADING INCIDENTS..." << endl;
    IncidentQueue incidents;
    incidents.loadFromFile("incidents.txt", cityGraph);
    
    // 4. DEMO: Route calculation
    cout << "\n4. DEMO: SHORTEST PATH CALCULATION" << endl;
    cout << "Calculating shortest path from Node 0 to Node 3..." << endl;
    int distance = cityGraph.dijkstra(0, 3);
    cout << "Shortest distance: " << distance << " units" << endl;
    
    // 5. DEMO: Find nearest ambulance
    cout << "\n5. DEMO: NEAREST AMBULANCE LOOKUP" << endl;
    cout << "Looking for nearest ambulance to Node 2..." << endl;
    Ambulance* nearest = rm.findNearestAmbulance(2, cityGraph);
    if (nearest) {
        cout << "Found: ";
        nearest->display();
        int distToIncident = cityGraph.dijkstra(nearest->getLocation(), 2);
        cout << "Distance to incident: " << distToIncident << " units" << endl;
    } else {
        cout << "No available ambulances found!" << endl;
    }
    
    // 6. DEMO: Process incidents from queue
    cout << "\n6. DEMO: PROCESSING INCIDENT QUEUE" << endl;
    int processed = 0;
    while (!incidents.isEmpty() && processed < 2) {
        if (rm.getAvailableCount() == 0) {
            cout << "No more available ambulances to assign." << endl;
            break;
        }
        Incident* nextIncident = incidents.getNextIncident();
        if (nextIncident) {
            cout << "\nProcessing ";
            nextIncident->display();
            
            Ambulance* assigned = rm.findNearestAmbulance(nextIncident->getLocation(), cityGraph);
            if (assigned) {
                assigned->dispatchTo(nextIncident->getId());
                int dist = cityGraph.dijkstra(assigned->getLocation(), nextIncident->getLocation());
                cout << "Assigned Ambulance #" << assigned->getId() 
                     << " (distance: " << dist << " units)" << endl;
                assigned->setLocation(nextIncident->getLocation());
                nextIncident->resolve();
                processed++;
            } else {
                cout << "No ambulances available. Incident will wait." << endl;
                incidents.reAddIncident(nextIncident);
            }
        }
    }
    
    // 7. Show final status
    cout << "\n7. FINAL SYSTEM STATUS" << endl;
    rm.displayAll();
    incidents.displayAll();
    
    cout << "\n========================================" << endl;
    cout << "DEMO COMPLETE!" << endl;
    cout << "========================================" << endl;
}

// Interactive menu for manual testing
void interactiveMenu() {
    Graph cityGraph;
    ResourceManager rm;
    IncidentQueue incidents;
    
    int choice;
    
    do {
        cout << "\n=== EMERGENCY ROUTING SYSTEM ===" << endl;
        cout << "1. Load City Map" << endl;
        cout << "2. Load Ambulances" << endl;
        cout << "3. Load Incidents" << endl;
        cout << "4. Calculate Shortest Path" << endl;
        cout << "5. Find Nearest Ambulance" << endl;
        cout << "6. Process Next Incident" << endl;
        cout << "7. Display System Status" << endl;
        cout << "8. Run Full Demo" << endl;
        cout << "9. Add Test Incident" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                cityGraph.loadFromFile("map_small.txt");
                cityGraph.display();
                break;
                
            case 2:
                rm.loadFromFile("ambulances.txt");
                rm.displayAll();
                break;
                
            case 3:
                incidents.loadFromFile("incidents.txt", cityGraph);
                break;
                
            case 4: {
                int start, end;
                cout << "Enter start node: ";
                cin >> start;
                cout << "Enter end node: ";
                cin >> end;
                int dist = cityGraph.dijkstra(start, end);
                if (dist == INT_MAX) {
                    cout << "No path exists between nodes " << start << " and " << end << endl;
                } else {
                    cout << "Shortest distance: " << dist << " units" << endl;
                }
                break;
            }
                
            case 5: {
                int location;
                cout << "Enter incident location (node): ";
                cin >> location;
                Ambulance* nearest = rm.findNearestAmbulance(location, cityGraph);
                if (nearest) {
                    cout << "Nearest available ambulance: ";
                    nearest->display();
                } else {
                    cout << "No available ambulances!" << endl;
                }
                break;
            }
                
            case 6:
                if (incidents.isEmpty()) {
                    cout << "No incidents in queue!" << endl;
                } else {
                    Incident* next = incidents.getNextIncident();
                    cout << "Processing: ";
                    next->display();
                    
                    Ambulance* assigned = rm.findNearestAmbulance(next->getLocation(), cityGraph);
                    if (assigned) {
                        assigned->dispatchTo(next->getId());
                        cout << "Assigned Ambulance #" << assigned->getId() << endl;
                        assigned->setLocation(next->getLocation());
                        next->resolve();
                    } else {
                        cout << "No ambulances available. Incident added back to queue." << endl;
                        incidents.reAddIncident(next);
                    }
                }
                break;
                
            case 7:
                cityGraph.display();
                rm.displayAll();
                incidents.displayAll();
                break;
                
            case 8:
                runDemo();
                break;
                
            case 9: {
                int loc;
                string pri, desc;
                cout << "Enter location (node): ";
                cin >> loc;
                cout << "Enter priority (HIGH/MEDIUM/LOW): ";
                cin >> pri;
                cout << "Enter description: ";
                cin.ignore();
                getline(cin, desc);
                incidents.addIncident(loc, pri, desc);
                break;
            }
                
            case 0:
                cout << "Exiting..." << endl;
                break;
                
            default:
                cout << "Invalid choice!" << endl;
        }
        
    } while (choice != 0);
}

int main() {
    cout << "========================================" << endl;
    cout << "  EMERGENCY ROUTING & RESOURCE SYSTEM   " << endl;
    cout << "        DSA Project - Week 12 Demo      " << endl;
    cout << "========================================" << endl;
    
    int choice;
    cout << "\nChoose mode:" << endl;
    cout << "1. Interactive Menu (Test features)" << endl;
    cout << "2. Automated Demo (Presentation ready)" << endl;
    cout << "Choice: ";
    cin >> choice;
    
    if (choice == 1) {
        interactiveMenu();
    } else {
        runDemo();
    }
    
    return 0;
}