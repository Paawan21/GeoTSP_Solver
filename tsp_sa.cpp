#include <bits/stdc++.h>
using namespace std;

const double START_TEMP = 1000.0;
const double COOLING_RATE = 0.995; // Slightly slower cooling for better exploration
const int MAX_ITERATIONS = 10000;

// Function to calculate total distance of a given route
double tourDistance(const vector<int> &route, const vector<vector<double>> &distanceMatrix) {
    double totalDist = 0.0;
    for (size_t i = 0; i < route.size() - 1; i++) {
        totalDist += distanceMatrix[route[i]][route[i + 1]];
    }
    totalDist += distanceMatrix[route.back()][route[0]];  // Returning to the start city
    return totalDist;
}

// Simulated Annealing algorithm for TSP
vector<int> simulatedAnnealing(vector<int> route, const vector<vector<double>> &distanceMatrix) {
    double temp = START_TEMP;
    vector<int> bestRoute = route;
    double bestDistance = tourDistance(route, distanceMatrix);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> probDist(0.0, 1.0);
    uniform_int_distribution<int> cityDist(0, route.size() - 1);

    for (int i = 0; i < MAX_ITERATIONS; i++) {
        int city1 = cityDist(gen);
        int city2 = cityDist(gen);
        while (city1 == city2) city2 = cityDist(gen);  // Ensure different cities are swapped

        swap(route[city1], route[city2]);
        double newDist = tourDistance(route, distanceMatrix);
        double delta = newDist - tourDistance(bestRoute, distanceMatrix);  // Compare with current route distance

        if (delta < 0 || exp(-delta / temp) > probDist(gen)) {
            bestRoute = route;
            bestDistance = newDist;
        } else {
            swap(route[city1], route[city2]);  // Revert if worse
        }

        temp *= COOLING_RATE;
        if (temp < 1e-6) break;  // Stop if temperature is too low
    }

    return bestRoute;
}

// Function to read distance matrix from file
vector<vector<double>> readDistanceMatrix(const string& filename, int n) {
    vector<vector<double>> distanceMatrix(n, vector<double>(n));
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open distance matrix file!\n";
        exit(1);
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!(file >> distanceMatrix[i][j])) {
                cerr << "Error: Invalid data in distance matrix file!\n";
                exit(1);
            }
        }
    }
    file.close();
    return distanceMatrix;
}

int main() {
    int n;
    cout << "Enter the number of cities: ";
    cin >> n;
    cin.ignore();

    vector<string> cities(n);
    cout << "Enter the names of the cities in the same order:\n";
    for (int i = 0; i < n; i++) {
        cout << "City " << i + 1 << ": ";
        getline(cin, cities[i]);
    }

    vector<vector<double>> distanceMatrix = readDistanceMatrix("distances.txt", n);

    vector<int> route(n);
    iota(route.begin(), route.end(), 0);  // Fill with 0,1,2,...

    vector<int> bestRoute = simulatedAnnealing(route, distanceMatrix);

    cout << "\nBest route: ";
    for (int city : bestRoute) {
        cout << cities[city] << " -> ";
    }
    cout << cities[bestRoute[0]] << endl;  

    cout << "\nBest distance: " << tourDistance(bestRoute, distanceMatrix) << " km\n";

    return 0;
}
