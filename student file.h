#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_set>

using namespace std;

struct Road {
    string id;
    string city1;
    string city2;
    int cost;
    bool requiresRepair;

    Road(string i, string c1, string c2, int co) : id(i), city1(c1), city2(c2), cost(co), requiresRepair(co > 0) {}
};

bool compareRoads(const Road& a, const Road& b) {
    if (a.cost != b.cost) return a.cost < b.cost;
    return a.id < b.id;
}

class CityNetwork {
private:
    vector<int> parent;
    vector<string> cities;

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

public:
    CityNetwork(const unordered_set<string>& allCities) {
        for (const string& city : allCities) {
            cities.push_back(city);
            parent.push_back(parent.size());
        }
    }

    void connect(string city1, string city2) {
        int i = findCity(city1);
        int j = findCity(city2);
        parent[find(i)] = find(j);
    }

    bool isConnected(string city1, string city2) {
        return find(findCity(city1)) == find(findCity(city2));
    }

    int findCity(const string& city) {
        for (size_t i = 0; i < cities.size(); ++i) {
            if (cities[i] == city) return i;
        }
        return -1;
    }
};

string rebuildNetwork(vector<string> roads) {
    vector<Road> network;
    unordered_set<string> allCitiesSet;

    for (const string& road : roads) {
        stringstream ss(road);
        string id, city1, city2;
        int cost = 0;
        ss >> id >> city1 >> city2;
        if (ss >> cost) {
            network.emplace_back(id, city1, city2, cost);
        } else {
            network.emplace_back(id, city1, city2, 0);
        }
        allCitiesSet.insert(city1);
        allCitiesSet.insert(city2);
    }

    sort(network.begin(), network.end(), compareRoads);

    CityNetwork cityNetwork(allCitiesSet);

    vector<string> toRepair;
    for (const Road& road : network) {
        if (!cityNetwork.isConnected(road.city1, road.city2)) {
            cityNetwork.connect(road.city1, road.city2);
            if (road.requiresRepair) {
                toRepair.push_back(road.id);
            }
        }
    }

    for (const string& city1 : allCitiesSet) {
        for (const string& city2 : allCitiesSet) {
            if (!cityNetwork.isConnected(city1, city2)) {
                return "IMPOSSIBLE";
            }
        }
    }

    sort(toRepair.begin(), toRepair.end());
    string result;
    for (const string& id : toRepair) {
        if (!result.empty()) result += " ";
        result += id;
    }
    return result;
}
