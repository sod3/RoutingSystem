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

void runDemo() {
    std::cout << "EMERGENCY ROUTING SYSTEM - LIVE DEMO" << std::endl;
    
    
    std::cout << "\n1. LOADING CITY MAP..." << std::endl;
    Graph cityGraph;
    cityGraph.loadFromFile("map_small.txt");
    cityGraph.display();
    
    std::cout << "\n2. LOADING AMBULANCES..." << std::endl;
    ResourceManager rm;
    rm.loadFromFile("ambulances.txt");
    rm.displayAll();
    
    std::cout << "\n3. LOADING INCIDENTS..." << std::endl;
    IncidentQueue incidents;
    incidents.loadFromFile("incidents.txt", cityGraph);

    // Loads the city map (road network)
    // Loads ambulance fleet
    // Loads emergency incidents
    
    std::cout << "\n4. DEMO: SHORTEST PATH CALCULATION" << std::endl;
    std::cout << "Calculating shortest path from Node 0 to Node 3..." << std::endl;
    int distance = cityGraph.dijkstra(0, 3);
    std::cout << "Shortest distance: " << distance << " units" << std::endl;
    // Calculates shortest route between two locations
    // Shows: Dijkstra's algorithm in action
    // Output: "Shortest distance: X units"
    
    std::cout << "\n5. DEMO: BLOCKED ROAD SCENARIO" << std::endl;
    std::cout << "Blocking road between Node 0 and Node 1..." << std::endl;
    cityGraph.markRoadBlocked(0, 1);
    cityGraph.displayBlockedRoads();
    // Simulates: Real-world road closures (accidents, construction)
    // Teaches: System adapts to changing conditions
    
    std::cout << "\nRecalculating path with blocked road..." << std::endl;
    distance = cityGraph.dijkstraWithBlocked(0, 3);
    if (distance == INT_MAX) {
        std::cout << "No path available with current road closures!" << std::endl;
    } else {
        std::cout << "Shortest distance (with blocked roads): " << distance << " units" << std::endl;
    }

    std::cout << "\n6. DEMO: NEAREST AMBULANCE LOOKUP" << std::endl;
    std::cout << "Looking for nearest ambulance to Node 2..." << std::endl;
    Ambulance* nearest = rm.findNearestAmbulance(2, cityGraph);
    if (nearest) {
        std::cout << "Found: ";
        nearest->display();
        int distToIncident = cityGraph.dijkstra(nearest->getLocation(), 2);
        std::cout << "Distance to incident: " << distToIncident << " units" << std::endl;
    } else {
        std::cout << "No available ambulances found!" << std::endl;
    }
    
    std::cout << "\n7. DEMO: DYNAMIC REASSIGNMENT" << std::endl;
    std::cout << "Reassigning ambulances based on current incidents..." << std::endl;
    rm.reassignAmbulances(incidents, cityGraph);
    
    std::cout << "\n8. DEMO: PROCESSING INCIDENT QUEUE" << std::endl;
    int processed = 0;
    while (!incidents.isEmpty() && processed < 2) {
        if (rm.getAvailableCount() == 0) {
            std::cout << "No more available ambulances to assign." << std::endl;
            break;
        }
        Incident* nextIncident = incidents.getNextIncident();
        if (nextIncident) {
            std::cout << "\nProcessing ";
            nextIncident->display();
            
            Ambulance* assigned = rm.findNearestAmbulance(nextIncident->getLocation(), cityGraph);
            if (assigned) {
                assigned->dispatchTo(nextIncident->getId());
                int dist = cityGraph.dijkstra(assigned->getLocation(), nextIncident->getLocation());
                std::cout << "Assigned Ambulance #" << assigned->getId() 
                         << " (distance: " << dist << " units)" << std::endl;
                assigned->setLocation(nextIncident->getLocation());
                nextIncident->resolve();
                processed++;
            } else {
                std::cout << "No ambulances available. Incident will wait." << std::endl;
                incidents.reAddIncident(nextIncident);
            }
        }
    }
// Handles emergencies from queue
// Assigns ambulances
// Marks incidents as resolved
// Handles when no ambulances available
    
    std::cout << "\n9. DEMO: TEST INCIDENT GENERATION" << std::endl;
    std::cout << "Generating 5 test incidents..." << std::endl;
    incidents.generateTestIncidents(5, cityGraph);
    
    std::cout << "\n10. DEMO: ADMIN FUNCTIONS" << std::endl;
    std::cout << "Saving current configurations..." << std::endl;
    cityGraph.saveToFile("map_backup.txt");
    rm.saveToFile("ambulances_backup.txt");
    incidents.saveToFile("incidents_backup.txt");
    
    std::cout << "Adding new ambulance..." << std::endl;
    rm.addAmbulance(999, 4);
    
    std::cout << "Updating road conditions..." << std::endl;
    cityGraph.updateEdgeWeight(2, 3, 15);
    
    std::cout << "\n11. FINAL SYSTEM STATUS" << std::endl;
    cityGraph.display();
    rm.displayAll();
    rm.displayReassignmentLog();
    incidents.displayAll();
    
    std::cout << "Active incidents: " << incidents.getActiveCount() << std::endl;

    // Generates random test incidents
    // Saves all data to backup files
    // Shows final system status
    
    std::cout << "DEMO COMPLETE!" << std::endl;
    
}

void adminMenu(ResourceManager &rm, Graph &cityGraph, IncidentQueue &incidents) {
    int choice;
    
    do {
        std::cout << "\nADMINISTRATOR MENU" << std::endl;
        std::cout << "1. Add New Ambulance" << std::endl;
        std::cout << "2. Remove Ambulance" << std::endl;
        std::cout << "3. Update Road Weight" << std::endl;
        std::cout << "4. Block/Unblock Road" << std::endl;
        std::cout << "5. Generate Test Incidents" << std::endl;
        std::cout << "6. Save All Configurations" << std::endl;
        std::cout << "7. Reassign All Ambulances" << std::endl;
        std::cout << "8. View Reassignment Log" << std::endl;
        std::cout << "9. Clear All Incidents" << std::endl;
        std::cout << "10. Back to Main Menu" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1:
                rm.addAmbulanceInteractive();
                break;
                
            case 2: {
                int id;
                std::cout << "Enter ambulance ID to remove: ";
                std::cin >> id;
                rm.removeAmbulance(id);
                break;
            }
                
            case 3: {
                int src, dest, weight;
                std::cout << "Enter source node: ";
                std::cin >> src;
                std::cout << "Enter destination node: ";
                std::cin >> dest;
                std::cout << "Enter new weight: ";
                std::cin >> weight;
                cityGraph.updateEdgeWeight(src, dest, weight);
                break;
            }
                
            case 4: {
                int src, dest;
                std::cout << "Enter source node: ";
                std::cin >> src;
                std::cout << "Enter destination node: ";
                std::cin >> dest;
                std::cout << "1. Block Road" << std::endl;
                std::cout << "2. Open Road" << std::endl;
                int action;
                std::cin >> action;
                if (action == 1) {
                    cityGraph.markRoadBlocked(src, dest);
                } else {
                    cityGraph.markRoadOpen(src, dest);
                }
                break;
            }
                
            case 5: {
                int count;
                std::cout << "Enter number of test incidents to generate: ";
                std::cin >> count;
                incidents.generateTestIncidents(count, cityGraph);
                break;
            }
                
            case 6:
                cityGraph.saveToFile("map_saved.txt");
                rm.saveToFile("ambulances_saved.txt");
                incidents.saveToFile("incidents_saved.txt");
                std::cout << "All configurations saved!" << std::endl;
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
                std::cout << "Returning to main menu..." << std::endl;
                break;
                
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
        
    } while (choice != 10);
}

void dispatcherMenu(Graph &cityGraph, ResourceManager &rm, IncidentQueue &incidents) {
    int choice;
    
    do {
        std::cout << "\nDISPATCHER MENU" << std::endl;
        std::cout << "1. Report New Incident" << std::endl;
        std::cout << "2. Find Nearest Ambulance" << std::endl;
        std::cout << "3. Dispatch Ambulance to Incident" << std::endl;
        std::cout << "4. Mark Assignment Complete" << std::endl;
        std::cout << "5. Check Road Conditions" << std::endl;
        std::cout << "6. View System Status" << std::endl;
        std::cout << "7. Save Current State" << std::endl;
        std::cout << "8. Back to Main Menu" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                int loc;
                std::string pri, desc;
                std::cout << "Enter location (node): ";
                std::cin >> loc;
                std::cout << "Enter priority (HIGH/MEDIUM/LOW): ";
                std::cin >> pri;
                std::cout << "Enter description: ";
                std::cin.ignore();
                std::getline(std::cin, desc);
                incidents.addIncident(loc, pri, desc);
                break;
            }
                
            case 2: {
                int location;
                std::cout << "Enter incident location (node): ";
                std::cin >> location;
                Ambulance* nearest = rm.findNearestAmbulance(location, cityGraph);
                if (nearest) {
                    std::cout << "Nearest available ambulance: ";
                    nearest->display();
                    int dist = cityGraph.dijkstra(nearest->getLocation(), location);
                    std::cout << "Estimated travel time: " << dist << " units" << std::endl;
                } else {
                    std::cout << "No available ambulances!" << std::endl;
                }
                break;
            }
                
            case 3: {
                int ambId, incId, loc;
                std::cout << "Enter ambulance ID: ";
                std::cin >> ambId;
                std::cout << "Enter incident ID: ";
                std::cin >> incId;
                std::cout << "Enter incident location (node): ";
                std::cin >> loc;
                
                if (rm.dispatchAmbulance(ambId, incId, loc)) {
                    std::cout << "Dispatch confirmed!" << std::endl;
                }
                break;
            }
                
            case 4: {
                int ambId;
                std::cout << "Enter ambulance ID to mark complete: ";
                std::cin >> ambId;
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
                std::cout << "Active incidents: " << incidents.getActiveCount() << std::endl;
                break;
                
            case 7:
                incidents.saveToFile("incidents_current.txt");
                std::cout << "Current incidents saved to file." << std::endl;
                break;
                
            case 8:
                std::cout << "Returning to main menu..." << std::endl;
                break;
                
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
        
    } while (choice != 8);
}

void interactiveMenu() {
    Graph cityGraph;
    ResourceManager rm;
    IncidentQueue incidents;
    
    int roleChoice;
    
    do {
        std::cout << "\nEMERGENCY ROUTING SYSTEM" << std::endl;
        std::cout << "Select Role:" << std::endl;
        std::cout << "1. Dispatcher (Handle emergencies)" << std::endl;
        std::cout << "2. Administrator (System setup)" << std::endl;
        std::cout << "3. View Only (System status)" << std::endl;
        std::cout << "0. Exit System" << std::endl;
        std::cout << "Choice: ";
        std::cin >> roleChoice;
        
        switch(roleChoice) {
            case 1: {
                std::cout << "\nDISPATCHER MODE" << std::endl;
                std::cout << "Loading default configurations..." << std::endl;
                cityGraph.loadFromFile("map_small.txt");
                rm.loadFromFile("ambulances.txt");
                incidents.loadFromFile("incidents.txt", cityGraph);
                dispatcherMenu(cityGraph, rm, incidents);
                break;
            }
                
            case 2: {
                std::cout << "\nADMINISTRATOR MODE" << std::endl;
                std::cout << "Loading default configurations..." << std::endl;
                cityGraph.loadFromFile("map_small.txt");
                rm.loadFromFile("ambulances.txt");
                adminMenu(rm, cityGraph, incidents);
                break;
            }
                
            case 3: {
                std::cout << "\nSYSTEM STATUS" << std::endl;
                cityGraph.loadFromFile("map_small.txt");
                rm.loadFromFile("ambulances.txt");
                incidents.loadFromFile("incidents.txt", cityGraph);
                
                cityGraph.display();
                rm.displayAll();
                incidents.displayAll();
                std::cout << "Active incidents: " << incidents.getActiveCount() << std::endl;
                
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
                
            case 0:
                std::cout << "Exiting system..." << std::endl;
                break;
                
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
        
    } while (roleChoice != 0);
}

int main() {
    std::srand(std::time(0));
    
    
    std::cout << "  EMERGENCY ROUTING & RESOURCE SYSTEM   " << std::endl;
    std::cout << "        DSA Project - Week 12 Demo      " << std::endl;
    
    
    int choice;
    std::cout << "\nChoose mode:" << std::endl;
    std::cout << "1. Role-Based Interactive Menu" << std::endl;
    std::cout << "2. Automated Demo (Presentation)" << std::endl;
    std::cout << "Choice: ";
    std::cin >> choice;
    
    if (choice == 1) {
        interactiveMenu();
    } else {
        runDemo();
    }
    
    return 0;
}