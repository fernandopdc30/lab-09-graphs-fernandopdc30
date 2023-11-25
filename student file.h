#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <unordered_set>

using namespace std;

class CustomUnionFind {
private:
    map<string, string> parent;

public:
    string find(const string& s) {
        if (parent.find(s) == parent.end())
            parent[s] = s;
        else if (parent[s] != s)
            parent[s] = find(parent[s]);
        return parent[s];
    }

    void merge(const string& a, const string& b) {
        string rootA = find(a);
        string rootB = find(b);
        if (rootA != rootB)
            parent[rootA] = rootB;
    }

    bool areConnected(const string& a, const string& b) {
        return find(a) == find(b);
    }

    bool allConnected(const vector<string>& cities) {
        if (cities.empty()) return true;
        string root = find(cities.front());
        for (const auto& city : cities) {
            if (find(city) != root) return false;
        }
        return true;
    }
};

struct Road {
    string id;
    string city1;
    string city2;
    int cost;

    Road(string i, string c1, string c2, int co = -1) : id(i), city1(c1), city2(c2), cost(co) {}
};

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    while (end != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

string reconstruct(vector<string> roads) {
    vector<Road> allRoads;
    vector<string> allCities;
    CustomUnionFind uf;
    unordered_set<string> roadsToRebuild;

    for (const auto& roadStr : roads) {
        auto tokens = split(roadStr, ' ');
        allCities.push_back(tokens[1]);
        allCities.push_back(tokens[2]);
        if (tokens.size() == 4) {
            allRoads.emplace_back(tokens[0], tokens[1], tokens[2], stoi(tokens[3]));
        } else {
            uf.merge(tokens[1], tokens[2]);
        }
    }

    for (const auto& road : allRoads) {
        if (!uf.areConnected(road.city1, road.city2)) {
            uf.merge(road.city1, road.city2);
            roadsToRebuild.insert(road.id);
        }
    }

    if (!uf.allConnected(allCities)) {
        return "IMPOSSIBLE";
    }

    string result;
    for (const auto& id : roadsToRebuild) {
        result += id + " ";
    }

    return result.empty() ? "" : result.substr(0, result.length() - 1);
}

int main() {
    // Ejemplo de uso
    vector<string> inputRoads = {"A X Y", "B Y Z 5", "C X Z 10", "D P Q 8"};
    string output = reconstruct(inputRoads);
    cout << "Resultado: " << output << endl;

    return 0;
}
