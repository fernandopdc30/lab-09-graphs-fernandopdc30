#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>

class UnionFind {
private:
    std::map<std::string, std::string> parent;
    std::map<std::string, int> rank;

public:
    void makeSet(const std::string& s) {
        parent[s] = s;
        rank[s] = 0;
    }

    std::string find(const std::string& s) {
        return (parent[s] == s) ? s : (parent[s] = find(parent[s]));
    }

    bool unionSets(const std::string& a, const std::string& b) {
        std::string rootA = find(a);
        std::string rootB = find(b);

        if (rootA == rootB)
            return false;

        if (rank[rootA] < rank[rootB])
            parent[rootA] = rootB;
        else if (rank[rootA] > rank[rootB])
            parent[rootB] = rootA;
        else {
            parent[rootB] = rootA;
            rank[rootA]++;
        }

        return true;
    }
};

class Road {
public:
    std::string id;
    std::string city1;
    std::string city2;
    int cost;

    Road(const std::string& i, const std::string& c1, const std::string& c2, int co = 0) : id(i), city1(c1), city2(c2), cost(co) {}
};

std::string reconstruct(std::vector<std::string> roads) {
    std::vector<Road> damagedRoads;
    UnionFind uf;
    std::set<std::string> cities;
    std::set<std::string> selectedRoads;

    for (const auto& roadStr : roads) {
        std::stringstream ss(roadStr);
        std::string id, city1, city2;
        ss >> id >> city1 >> city2;
        uf.makeSet(city1);
        uf.makeSet(city2);
        cities.insert(city1);
        cities.insert(city2);
    }

    for (const auto& roadStr : roads) {
        std::stringstream ss(roadStr);
        std::string id, city1, city2;
        int cost = 0;
        ss >> id >> city1 >> city2;
        if (!(ss >> cost)) {
            uf.unionSets(city1, city2);
        } else {
            damagedRoads.emplace_back(id, city1, city2, cost);
        }
    }

    std::sort(damagedRoads.begin(), damagedRoads.end(), [](const Road& a, const Road& b) {
        return (a.cost != b.cost) ? (a.cost < b.cost) : (a.id < b.id);
    });

    for (const auto& road : damagedRoads) {
        if (uf.find(road.city1) != uf.find(road.city2)) {
            uf.unionSets(road.city1, road.city2);
            selectedRoads.insert(road.id);
        }
    }

    std::string root = uf.find(*cities.begin());
    for (const auto& city : cities) {
        if (uf.find(city) != root) {
            return "IMPOSSIBLE";
        }
    }

    std::string result;
    for (const auto& id : selectedRoads) {
        result += id + " ";
    }

    return result.empty() ? "" : result.substr(0, result.length() - 1);
}

int main() {
    // Puedes agregar código para probar la función reconstruct aquí
    return 0;
}
