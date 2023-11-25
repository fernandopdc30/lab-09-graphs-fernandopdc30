#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

struct Road {
    string id;
    int city1, city2, cost;

    bool operator<(const Road& other) const {
        if (cost != other.cost) return cost < other.cost;
        return id < other.id;
    }
};

vector<int> parent;

int findSet(int v) {
    if (v == parent[v])
        return v;
    return parent[v] = findSet(parent[v]);
}

void makeSet(int v) {
    parent[v] = v;
}

void unionSets(int a, int b) {
    a = findSet(a);
    b = findSet(b);
    if (a != b)
        parent[b] = a;
}

vector<string> reconstructRoads(vector<string> roads) {
    map<string, int> cityToInt;
    vector<Road> roadList;
    int cityCounter = 0;

    for (string road : roads) {
        stringstream ss(road);
        string id, cityA, cityB;
        int cost = -1;
        ss >> id >> cityA >> cityB;

        if (ss >> cost) {
            if (cityToInt.find(cityA) == cityToInt.end()) cityToInt[cityA] = cityCounter++;
            if (cityToInt.find(cityB) == cityToInt.end()) cityToInt[cityB] = cityCounter++;
            roadList.push_back({id, cityToInt[cityA], cityToInt[cityB], cost});
        }
    }

    sort(roadList.begin(), roadList.end());
    parent.resize(cityCounter);

    for (int i = 0; i < cityCounter; i++) makeSet(i);

    vector<string> result;

    for (const auto& road : roadList) {
        if (findSet(road.city1) != findSet(road.city2)) {
            result.push_back(road.id);
            unionSets(road.city1, road.city2);
        }
    }

    for (int i = 1; i < cityCounter; i++) {
        if (findSet(i) != findSet(0)) {
            result.clear();
            result.push_back("IMPOSSIBLE");
            break;
        }
    }

    return result;
}

int main() {
    vector<string> roads = {"R1 Lima Trujillo 1", "R2 Tacna Trujillo", "R3 Tacna Arequipa"};
    vector<string> result = reconstructRoads(roads);

    for (const string& id : result) {
        cout << id << " ";
    }

    cout << endl;

    return 0;
}
