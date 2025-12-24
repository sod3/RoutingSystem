# What I Worked On: Ambulance.cpp and ResourceManagement.cpp

# 1. Ambulance.cpp - Making the Ambulance Class
I created the file that defines what an ambulance is in our system. Think of it like making a blueprint for each ambulance:

# What it stores:
Ambulance ID number (like a license plate)
Current location on the map
Status (Available or Busy)
Which incident it's assigned to (if any)

# What it can do:
Display its information on screen
Change status from Available to Busy when dispatched
Change status back to Available when done
Update its location after moving

# 2. ResourceManager.cpp - Managing All Ambulances
This is like the control center for all ambulances in the city:
# Adding/Removing Ambulances:
Can add new ambulances to the system
Can remove ambulances (but not if they're busy)
# Finding the Nearest Ambulance:
When there's an emergency, I find which ambulance is closest
Uses the Graph class (made by Anas) to calculate distances
# Dispatching Ambulances:
Assigns an ambulance to an emergency
Tracks which ambulance goes to which incident
# Reassigning Ambulances:
If new emergencies happen, can reassign ambulances
Keeps a log of all reassignments for tracking

# Challenges I Solved:
# Problem: How to find the nearest ambulance quickly?
Solution: I check all available ambulances one by one and use the Graph to calculate distances. I pick the one with smallest distance.

# Problem: What if we need to remove an ambulance but it's busy?
Solution: I check the status first. If busy, show an error message.

# Problem: How to track which ambulance went where?
Solution: I keep a "reassignment log" - a list that remembers all ambulance movements.

# Simple Example of What My Code Does:
Emergency at location 10
Ambulance A at location 5 (distance 15 minutes)
Ambulance B at location 8 (distance 8 minutes)

# My code:
Checks ambulance A: distance = 15
Checks ambulance B: distance = 8
Chooses ambulance B (because 8 < 15)
Dispatches ambulance B to the emergency