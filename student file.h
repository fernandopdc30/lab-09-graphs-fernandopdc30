#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Subset {
    int parent, rank;
};

class Road {
public:
    string roadId;
    int city1, city2, cost;

    Road(string id, int c1, int c2, int cost) : roadId(id), city1(c1), city2(c2), cost(cost) {}

    bool operator<(const Road& compareRoad) const {
        int compareCost = this->cost - compareRoad.cost;
        if (compareCost != 0) return compareCost < 0;
        return this->roadId < compareRoad.roadId;
    }
};

class TransportationNetwork {
public:
    vector<Road> roads;
    unordered_map<string, int> cityMap;

    TransportationNetwork() {}

    int find(Subset subsets[], int i) {
        if (subsets[i].parent != i) {
            subsets[i].parent = find(subsets, subsets[i].parent);
        }
        return subsets[i].parent;
    }

    void uniteSets(Subset subsets[], int x, int y) {
        int xRoot = find(subsets, x);
        int yRoot = find(subsets, y);

        if (subsets[xRoot].rank < subsets[yRoot].rank) {
            subsets[xRoot].parent = yRoot;
        } else if (subsets[xRoot].rank > subsets[yRoot].rank) {
            subsets[yRoot].parent = xRoot;
        } else {
            subsets[xRoot].parent = yRoot;
            subsets[xRoot].rank++;
        }
    }

    void addConnection(string id, string city1, string city2, int cost) {
        if (cityMap.find(city1) == cityMap.end()) {
            cityMap[city1] = cityMap.size(); 
        }
        if (cityMap.find(city2) == cityMap.end()) {
            cityMap[city2] = cityMap.size(); 
        }
        roads.emplace_back(id, cityMap[city1], cityMap[city2], cost);
    }

    string findOptimalRepair() {
        if (roads.empty()) {
            return "NO SOLUTION";
        }

        sort(roads.begin(), roads.end());

        Subset subsets[cityMap.size()];
        for (int i = 0; i < cityMap.size(); ++i) {
            subsets[i].parent = i;
            subsets[i].rank = 0;
        }

        string roadToRepair = "";
        int minRepairCost = INT_MAX;

        int connectedCities = 0;

        for (const Road& road : roads) {
            int x = find(subsets, road.city1);
            int y = find(subsets, road.city2);

            if (x != y) {
                if (road.cost != 0 && road.cost < minRepairCost) {
                    roadToRepair += road.roadId;
                    minRepairCost = road.cost;
                }
                uniteSets(subsets, x, y);
                connectedCities++;
            }
        }

        if (connectedCities == 0 && cityMap.size() == 1) {
            return "";
        } else if (connectedCities < cityMap.size() - 1) {
            return "NO SOLUTION";
        }

        return roadToRepair.empty() ? "" : roadToRepair;
    }
};

string reconstructRoads(vector<string>& roadList) {
    if (roadList.size() == 1) {
        return "";
    }

    TransportationNetwork network;

    for (const string& road : roadList) {
        vector<string> parts;

        size_t start = 0, end = 0;
        while ((end = road.find(' ', start)) != string::npos) {
            parts.push_back(road.substr(start, end - start));
            start = end + 1;
        }
        parts.push_back(road.substr(start));
        try {
            int cost = stoi(parts[3]);
            network.addConnection(parts[0], parts[1], parts[2], cost);
        } catch (invalid_argument& e) {
            return "NO SOLUTION";
        }
    }

    return network.findOptimalRepair();
}

