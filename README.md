# Traveling Salesman Problem (TSP) Solver

This project implements the **Traveling Salesman Problem (TSP)** using **Simulated Annealing (SA)** and fetches real-world distance data from **OpenStreetMap (OSM) via OSRM API**.

---

## **Features**
- Uses **OpenStreetMap (Nominatim API)** to fetch latitude & longitude for cities.
- Computes a **distance matrix** using **OSRM (Open Source Routing Machine)**.
- Applies **Simulated Annealing (SA)** to approximate the shortest route.
- Saves the distance matrix in `distances.txt`.

---

## **Dependencies**

Ensure you have the following installed:

### **Linux/MacOS**
```sh
sudo apt install libcurl4-openssl-dev  # For Ubuntu/Debian
brew install curl  # For macOS
```

### **C++ Libraries**
- `cURL` (for HTTP requests)
- `nlohmann/json.hpp` (for parsing JSON)

Install `nlohmann/json.hpp`:
```sh
sudo apt install nlohmann-json3-dev  # Ubuntu/Debian
brew install nlohmann-json  # macOS
```

---

## **How to Run**

### **Step 1: Compile the Code**
```sh
g++ distancematrix.cpp -o distance_matrix -lcurl
g++ tsp_sa.cpp -o tsp_solver
```

### **Step 2: Generate Distance Matrix**
```sh
./distance_matrix
```
- Enter the number of cities.
- Enter city names (e.g., `New York`, `Chicago`).
- The program fetches distances and saves them to `distances.txt`.

### **Step 3: Solve TSP Using Simulated Annealing**
```sh
./tsp_solver
```
- Enter the **same** number of cities in the **same order**.
- The program finds the **optimal route** using Simulated Annealing.

---

## **Example Run**
### **Generating Distance Matrix**
```sh
Enter the number of cities: 3
City 1: New York
City 2: Chicago
City 3: Los Angeles
Distance matrix saved to distances.txt
```

### **Solving TSP**
```sh
Enter the number of cities: 3
Enter the city names in the same order:
City 1: New York
City 2: Chicago
City 3: Los Angeles

Best route: New York -> Chicago -> Los Angeles -> New York
Best distance: 4200.5 km
```

---

## **Troubleshooting**

### **1. Distance values are too large (e.g., 10,000+ km)?**
✅ **Fix:** Check `distances.txt`. If the values are huge, update the conversion in `getDistanceMatrix()`.
```cpp
const double AVERAGE_SPEED_KMH = 90.0;  // Adjust this value if needed
value = (value / 3600.0) * AVERAGE_SPEED_KMH;
```

### **2. Route order seems incorrect?**
✅ **Fix:** Print the best route for debugging.
```cpp
cout << "Route Debug: ";
for (int city : bestRoute) cout << cities[city] << " -> ";
cout << cities[bestRoute[0]] << endl;
```

### **3. `distances.txt` is missing?**
✅ **Fix:** Check if `distance_matrix` ran successfully and the file is saved in the correct directory.

---

## **Future Enhancements**
- Support **more vehicle profiles** (e.g., walking, cycling, public transport).
- Allow **waypoint reordering** for flexible routes.
- Implement **parallel computing** for faster processing.

---

## **Contributors**
- **Paawan Kashyap** (Project Owner)


