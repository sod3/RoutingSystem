# Final Project Report (Emergency Routing System)

# Executive Summary
Successfully implemented an Emergency Routing System that:
- Models city as weighted graph
- Computes optimal routes using Dijkstra
- Manages incidents via priority queue
- Dynamically allocates resources
- Adapts to road closures

# Technical Achievements
1. Core Algorithms Implemented:
- Dijkstra with priority queue
- Nearest neighbor search (Find the closest thing to me)
- Priority-based scheduling (Handle the most important things first")

2. Data Structure Mastery:
- Custom Graph class
- Incident priority queue
- Resource manager with reassignment log

3. File I/O System:
- Persistent data storage
- Backup/restore functionality
- Test data generation

# Challenges & Solutions
1. Challenge: Dynamic reassignment with changing conditions
   Solution: Implemented reallocation algorithm that considers both distance and priority

2. Challenge: Blocked road handling in pathfinding
   Solution: Modified Dijkstra to skip blocked edges

3. Challenge: Memory management with pointers
   Solution: Used RAII principles and proper cleanup

# Learning Outcomes
- Advanced graph algorithms
- Priority queue applications
- System design patterns
- File-based persistence
- Console UI design