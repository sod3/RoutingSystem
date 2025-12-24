# Team Members
Mudassir Muneer
Anas Siddiqui

# Emergency Routing & Resource Allocation System

DSA Final Project Week 13 Submission

# Overview
A system for routing ambulances through city graphs using Dijkstra's algorithm with dynamic resource allocation.

# Features
- Graph-based city map modeling
- Dijkstra's shortest path algorithm
- Priority-based incident queue
- Nearest ambulance allocation
- Dynamic reassignment
- Road blockage simulation
- File-based persistence

# Data Structures Used
- Graph (Adjacency List): City road network
- Priority Queue: Dijkstra's algorithm & incident prioritization
- Vector: Ambulance and incident storage
- Map: Distance tracking and blocked roads
- Pair: Edge representation

# Key Design Decisions

1. Adjacency List over Matrix: Better for sparse city graphs
2. Priority Queue for Incidents: Ensures HIGH priority emergencies are handled first
3. File-based Persistence: Easy testing and data management
4. Role-based Access: Separates dispatcher and admin concerns
