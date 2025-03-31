#include <bits/stdc++.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cmath>
using namespace std;
using json = nlohmann::json;
size_t writeback(void *content, size_t size, size_t memb, string *output) {
    size_t total = size * memb;
    output->append((char*)content, total);
    return total;
}

// Function to get latitude & longitude for a city using OpenStreetMap (Nominatim API)
auto getCoordinates(const string& city) -> pair<double, double> {
    string encodedCity;
    for (char c : city) {
        if (c == ' ') 
            encodedCity += "%20";  
        else 
            encodedCity += c;
    }
    string url = "https://nominatim.openstreetmap.org/search?format=json&q=" + encodedCity;
    
    CURL *curl = curl_easy_init();
    string response;
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        if (res != CURLE_OK) {
            cerr << "Error fetching coordinates for " << city << endl;
            return {0.0, 0.0};
        }
    }

    try {
        json result = json::parse(response);
        if (!result.empty()) {
            double lat = stod(result[0]["lat"].get<string>());
            double lon = stod(result[0]["lon"].get<string>());
            return make_pair(lat, lon);
        }
    } catch (...) {
        cerr << "Error parsing JSON for " << city << endl;
    }

    return {0.0, 0.0}; 
}

// Haversine function to calculate Great Circle Distance (in km)
double haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371.0;  // Earth radius in kilometers
    double lat1_rad = lat1 * M_PI / 180.0;
    double lon1_rad = lon1 * M_PI / 180.0;
    double lat2_rad = lat2 * M_PI / 180.0;
    double lon2_rad = lon2 * M_PI / 180.0;
    
    double dlat = lat2_rad - lat1_rad;
    double dlon = lon2_rad - lon1_rad;
    
    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(lat1_rad) * cos(lat2_rad) *
               sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    
    return R * c;  // Distance in kilometers
}

int main() {
    int n;
    cout << "Enter the number of cities: ";
    cin >> n;
    cin.ignore();

    vector<string> cities(n);
    vector<pair<double, double>> coordinates;

    cout << "Enter the city names:\n";
    for (int i = 0; i < n; i++) {
        cout << "City " << i + 1 << ": ";
        getline(cin, cities[i]);
        coordinates.push_back(getCoordinates(cities[i]));
    }

    // Calculate the Great Circle Distance Matrix
    vector<vector<double>> distanceMatrix(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                double lat1 = coordinates[i].first;
                double lon1 = coordinates[i].second;
                double lat2 = coordinates[j].first;
                double lon2 = coordinates[j].second;
                distanceMatrix[i][j] = haversine(lat1, lon1, lat2, lon2);
            }
        }
    }

    // Print the distance matrix
    cout << "Distance Matrix (Great Circle Distance in km):\n";
    for (const auto& row : distanceMatrix) {
        for (double d : row) {
            cout << d << " ";
        }
        cout << endl;
    }

    // Save the distance matrix to a file
    ofstream file("/Users/paawankashyap/Desktop/TSP/distances.txt");
    for (const auto& row : distanceMatrix) {
        for (double d : row) file << d << " ";
        file << "\n";
    }
    file.close();

    cout << "Distance matrix saved to distances.txt\n";
    return 0;
}
