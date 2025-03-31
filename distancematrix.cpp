#include <bits/stdc++.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// Function to handle API response
size_t writeback(void *content, size_t size, size_t memb, string *output) {
    size_t total = size * memb;
    output->append((char*)content, total);
    return total;
}

// Function to get latitude & longitude for a city using OpenStreetMap (Nominatim API)
auto getCoordinates(const string& city) -> pair<double, double> 
    {
        string encodedCity;
    for (char c : city) {
        if (c == ' ') 
            encodedCity += "%20";  // Encode spaces
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
            return std::make_pair(lat, lon);
        }
    } catch (...) {
        cerr << "Error parsing JSON for " << city << endl;
    }

    return {0.0, 0.0};  // Return invalid coordinates if not found
}

// Function to get distance matrix using OSRM
vector<vector<double>> getDistanceMatrix(const vector<pair<double, double>>& coordinates) {
    string baseUrl = "http://router.project-osrm.org/table/v1/driving/";
    
    // Append lat/lon to URL
    for (const auto& coord : coordinates) {
        baseUrl += to_string(coord.second) + "," + to_string(coord.first) + ";";
    }
    baseUrl.pop_back();  // Remove trailing ";"

    CURL *curl = curl_easy_init();
    string response;
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, baseUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        if (res != CURLE_OK) {
            cerr << "Error fetching distance matrix" << endl;
            return {};
        }
    }
    const double AVERAGE_SPEED_KMH = 70.0;  // Average speed in km/h

    try {
        json result = json::parse(response);
        if (result.contains("durations")) {
            vector<vector<double>> distanceMatrix = result["durations"];
    

            for (auto& row : distanceMatrix) {
                for (auto& value : row) {
                    value = (value / 3600.0) * AVERAGE_SPEED_KMH;  
                }
            }
    
            return distanceMatrix;
        }    
    } catch (...) {
        cerr << "Error parsing distance matrix JSON" << endl;
    }

    return {};
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

    vector<vector<double>> distanceMatrix = getDistanceMatrix(coordinates);

    if (distanceMatrix.empty()) {
        cerr << "Failed to retrieve distance matrix. Exiting.\n";
        return 1;
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
