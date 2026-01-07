#include "Graph.h"
#include "Incident.h"
#include "ResourceManager.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <sstream>

using namespace std;

// Helper function to clear invalid input
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Helper function to get integer input with validation
int getIntegerInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        
        if (cin.fail()) {
            cout << "Invalid input! Please enter a valid number.\n";
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

// Helper function to get string input
string getStringInput(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

// Helper function to get valid priority
string getPriorityInput() {
    string pri;
    while (true) {
        cout << "Enter priority (HIGH/MEDIUM/LOW): ";
        getline(cin, pri);
        
        // Convert to uppercase for comparison
        for (char &c : pri) {
            c = toupper(c);
        }
        
        if (pri == "HIGH" || pri == "MEDIUM" || pri == "LOW") {
            return pri;
        } else {
            cout << "Invalid priority! Please enter HIGH, MEDIUM, or LOW.\n";
        }
    }
}

void runDemo() {
    cout << "EMERGENCY ROUTING SYSTEM - LIVE DEMO" << endl;
    
    cout << "\n1. LOADING CITY MAP..." << endl;
    Graph cityGraph;
    cityGraph.loadFromFile("map_small.txt");
    cityGraph.display();
    
    cout << "\n2. LOADING AMBULANCES..." << endl;
    ResourceManager rm;
    rm.loadFromFile("ambulances.txt");
    rm.displayAll();
    
    cout << "\n3. LOADING INCIDENTS..." << endl;
    IncidentQueue incidents;
    incidents.loadFromFile("incidents.txt", cityGraph);
    
    cout << "\n4. DEMO: SHORTEST PATH CALCULATION" << endl;
    cout << "Calculating shortest path from Node 0 to Node 3..." << endl;
    int distance = cityGraph.dijkstra(0, 3);
    cout << "Shortest distance: " << distance << " units" << endl;
    
    cout << "\n5. DEMO: BLOCKED ROAD SCENARIO" << endl;
    cout << "Blocking road between Node 0 and Node 1..." << endl;
    cityGraph.markRoadBlocked(0, 1);
    cityGraph.displayBlockedRoads();
    
    cout << "\nRecalculating path with blocked road..." << endl;
    distance = cityGraph.dijkstraWithBlocked(0, 3);
    if (distance == INT_MAX) {
        cout << "No path available with current road closures!" << endl;
    } else {
        cout << "Shortest distance (with blocked roads): " << distance << " units" << endl;
    }

    cout << "\n6. DEMO: NEAREST AMBULANCE LOOKUP" << endl;
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
    
    cout << "\n7. DEMO: DYNAMIC REASSIGNMENT" << endl;
    cout << "Reassigning ambulances based on current incidents..." << endl;
    rm.reassignAmbulances(incidents, cityGraph);
    
    cout << "\n8. DEMO: PROCESSING INCIDENT QUEUE" << endl;
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
    
    cout << "\n9. DEMO: TEST INCIDENT GENERATION" << endl;
    cout << "Generating 5 test incidents..." << endl;
    incidents.generateTestIncidents(5, cityGraph);
    
    cout << "\n10. DEMO: ADMIN FUNCTIONS" << endl;
    cout << "Saving current configurations..." << endl;
    cityGraph.saveToFile("map_backup.txt");
    rm.saveToFile("ambulances_backup.txt");
    incidents.saveToFile("incidents_backup.txt");
    
    cout << "Adding new ambulance..." << endl;
    rm.addAmbulance(999, 4);
    
    cout << "Updating road conditions..." << endl;
    cityGraph.updateEdgeWeight(2, 3, 15);
    
    cout << "\n11. FINAL SYSTEM STATUS" << endl;
    cityGraph.display();
    rm.displayAll();
    rm.displayReassignmentLog();
    incidents.displayAll();
    
    cout << "Active incidents: " << incidents.getActiveCount() << endl;
    
    cout << "DEMO COMPLETE!" << endl;
}

void adminMenu(ResourceManager &rm, Graph &cityGraph, IncidentQueue &incidents) {
    int choice;
    
    do {
        cout << "\nADMINISTRATOR MENU" << endl;
        cout << "1. Add New Ambulance" << endl;
        cout << "2. Remove Ambulance" << endl;
        cout << "3. Update Road Weight" << endl;
        cout << "4. Block/Unblock Road" << endl;
        cout << "5. Generate Test Incidents" << endl;
        cout << "6. Save All Configurations" << endl;
        cout << "7. Reassign All Ambulances" << endl;
        cout << "8. View Reassignment Log" << endl;
        cout << "9. Clear All Incidents" << endl;
        cout << "10. Back to Main Menu" << endl;
        cout << "Choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number between 1 and 10.\n";
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1:
                rm.addAmbulanceInteractive();
                break;
                
            case 2: {
                int id = getIntegerInput("Enter ambulance ID to remove: ");
                rm.removeAmbulance(id);
                break;
            }
                
            case 3: {
                int src = getIntegerInput("Enter source node: ");
                int dest = getIntegerInput("Enter destination node: ");
                int weight = getIntegerInput("Enter new weight: ");
                cityGraph.updateEdgeWeight(src, dest, weight);
                break;
            }
                
            case 4: {
                int src = getIntegerInput("Enter source node: ");
                int dest = getIntegerInput("Enter destination node: ");
                cout << "1. Block Road" << endl;
                cout << "2. Open Road" << endl;
                
                int action;
                if (!(cin >> action)) {
                    cout << "Invalid input!\n";
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();
                
                if (action == 1) {
                    cityGraph.markRoadBlocked(src, dest);
                } else if (action == 2) {
                    cityGraph.markRoadOpen(src, dest);
                } else {
                    cout << "Invalid choice! Please enter 1 or 2.\n";
                }
                break;
            }
                
            case 5: {
                int count = getIntegerInput("Enter number of test incidents to generate: ");
                incidents.generateTestIncidents(count, cityGraph);
                break;
            }
                
            case 6:
                cityGraph.saveToFile("map_saved.txt");
                rm.saveToFile("ambulances_saved.txt");
                incidents.saveToFile("incidents_saved.txt");
                cout << "All configurations saved!" << endl;
                break;
                
            case 7:
                rm.reassignAmbulances(incidents, cityGraph);
                break;
                
            case 8:
                rm.displayReassignmentLog();
                break;
                
            case 9:
                incidents.clearAll();
                break;
                
            case 10:
                cout << "Returning to main menu..." << endl;
                break;
                
            default:
                cout << "Invalid choice! Please enter a number between 1 and 10.\n";
        }
        
    } while (choice != 10);
}

void dispatcherMenu(Graph &cityGraph, ResourceManager &rm, IncidentQueue &incidents) {
    int choice;
    
    do {
        cout << "\nDISPATCHER MENU" << endl;
        cout << "1. Report New Incident" << endl;
        cout << "2. Find Nearest Ambulance" << endl;
        cout << "3. Dispatch Ambulance to Incident" << endl;
        cout << "4. Mark Assignment Complete" << endl;
        cout << "5. Check Road Conditions" << endl;
        cout << "6. View System Status" << endl;
        cout << "7. Save Current State" << endl;
        cout << "8. Back to Main Menu" << endl;
        cout << "Choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number between 1 and 8.\n";
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1: {
                int loc = getIntegerInput("Enter location (node): ");
                string pri = getPriorityInput();
                string desc = getStringInput("Enter description: ");
                incidents.addIncident(loc, pri, desc);
                break;
            }
                
            case 2: {
                int location = getIntegerInput("Enter incident location (node): ");
                Ambulance* nearest = rm.findNearestAmbulance(location, cityGraph);
                if (nearest) {
                    cout << "Nearest available ambulance: ";
                    nearest->display();
                    int dist = cityGraph.dijkstra(nearest->getLocation(), location);
                    cout << "Estimated travel time: " << dist << " units" << endl;
                } else {
                    cout << "No available ambulances!" << endl;
                }
                break;
            }
                
            case 3: {
                int ambId = getIntegerInput("Enter ambulance ID: ");
                int incId = getIntegerInput("Enter incident ID: ");
                int loc = getIntegerInput("Enter incident location (node): ");
                
                if (rm.dispatchAmbulance(ambId, incId, loc)) {
                    cout << "Dispatch confirmed!" << endl;
                }
                break;
            }
                
            case 4: {
                int ambId = getIntegerInput("Enter ambulance ID to mark complete: ");
                rm.completeAssignment(ambId);
                break;
            }
                
            case 5:
                cityGraph.displayBlockedRoads();
                break;
                
            case 6:
                cityGraph.display();
                rm.displayAll();
                incidents.displayAll();
                cout << "Active incidents: " << incidents.getActiveCount() << endl;
                break;
                
            case 7:
                incidents.saveToFile("incidents_current.txt");
                cout << "Current incidents saved to file." << endl;
                break;
                
            case 8:
                cout << "Returning to main menu..." << endl;
                break;
                
            default:
                cout << "Invalid choice! Please enter a number between 1 and 8.\n";
        }
        
    } while (choice != 8);
}

void interactiveMenu() {
    Graph cityGraph;
    ResourceManager rm;
    IncidentQueue incidents;
    
    int roleChoice;
    
    do {
        cout << "\nEMERGENCY ROUTING SYSTEM" << endl;
        cout << "Select Role:" << endl;
        cout << "1. Dispatcher (Handle emergencies)" << endl;
        cout << "2. Administrator (System setup)" << endl;
        cout << "3. View Only (System status)" << endl;
        cout << "0. Exit System" << endl;
        cout << "Choice: ";
        
        if (!(cin >> roleChoice)) {
            cout << "Invalid input! Please enter a number between 0 and 3.\n";
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        switch(roleChoice) {
            case 1: {
                cout << "\nDISPATCHER MODE" << endl;
                cout << "Loading default configurations..." << endl;
                cityGraph.loadFromFile("map_small.txt");
                rm.loadFromFile("ambulances.txt");
                incidents.loadFromFile("incidents.txt", cityGraph);
                dispatcherMenu(cityGraph, rm, incidents);
                break;
            }
                
            case 2: {
                cout << "\nADMINISTRATOR MODE" << endl;
                cout << "Loading default configurations..." << endl;
                cityGraph.loadFromFile("map_small.txt");
                rm.loadFromFile("ambulances.txt");
                adminMenu(rm, cityGraph, incidents);
                break;
            }
                
            case 3: {
                cout << "\nSYSTEM STATUS" << endl;
                cityGraph.loadFromFile("map_small.txt");
                rm.loadFromFile("ambulances.txt");
                incidents.loadFromFile("incidents.txt", cityGraph);
                
                cityGraph.display();
                rm.displayAll();
                incidents.displayAll();
                cout << "Active incidents: " << incidents.getActiveCount() << endl;
                
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
                
            case 0:
                cout << "Exiting system..." << endl;
                break;
                
            default:
                cout << "Invalid choice! Please enter a number between 0 and 3.\n";
        }
        
    } while (roleChoice != 0);
}

int main() {
    srand(time(0));
    
    cout << "  EMERGENCY ROUTING & RESOURCE SYSTEM   " << endl;
    cout << "        DSA Project - Week 12 Demo      " << endl;
    
    int choice;
    
    while (true) {
        cout << "\nChoose mode:" << endl;
        cout << "1. Role-Based Interactive Menu" << endl;
        cout << "2. Automated Demo (Presentation)" << endl;
        cout << "Choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter 1 or 2.\n";
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        if (choice == 1 || choice == 2) {
            break;
        } else {
            cout << "Invalid choice! Please enter 1 or 2.\n";
        }
    }
    
    if (choice == 1) {
        interactiveMenu();
    } else {
        runDemo();
    }
    
    return 0;
}