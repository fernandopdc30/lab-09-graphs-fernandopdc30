#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>

class UnionFind {
private:
    std::unordered_map<std::string, std::string> padre;
    std::unordered_map<std::string, int> rango;

public:
    void hacerConjunto(const std::string& elemento) {
        padre[elemento] = elemento;
        rango[elemento] = 0;
    }

    std::string encontrar(const std::string& elemento) {
        if (padre[elemento] != elemento)
            padre[elemento] = encontrar(padre[elemento]);
        return padre[elemento];
    }

    bool unirConjuntos(const std::string& a, const std::string& b) {
        std::string raizA = encontrar(a);
        std::string raizB = encontrar(b);

        if (raizA == raizB) {
            return false;
        }

        if (rango[raizA] < rango[raizB]) {
            padre[raizA] = raizB;
        } else if (rango[raizA] > rango[raizB]) {
            padre[raizB] = raizA;
        } else {
            padre[raizB] = raizA;
            rango[raizA]++;
        }

        return true;
    }
};

struct Carretera {
    std::string id;
    std::string ciudad1;
    std::string ciudad2;
    int costo;

    Carretera(const std::string& i, const std::string& c1, const std::string& c2, int co = 0)
        : id(i), ciudad1(c1), ciudad2(c2), costo(co) {}
};

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);
    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    result.push_back(str.substr(start));
    return result;
}

std::string reconstruye(std::vector<std::string> listaCarreteras) {
    std::vector<Carretera> carreterasDeterioradas;
    UnionFind uf;
    std::set<std::string> raices; // Conjunto de raíces de conjuntos

    for (const auto& carreteraStr : listaCarreteras) {
        auto elementos = split(carreteraStr, ' ');
        std::string id = elementos[0];
        std::string ciudad1 = elementos[1];
        std::string ciudad2 = elementos[2];

        uf.hacerConjunto(ciudad1);
        uf.hacerConjunto(ciudad2);
        raices.insert(uf.encontrar(ciudad1));
        raices.insert(uf.encontrar(ciudad2));
    }

    for (const auto& carreteraStr : listaCarreteras) {
        auto elementos = split(carreteraStr, ' ');
        std::string id = elementos[0];
        std::string ciudad1 = elementos[1];
        std::string ciudad2 = elementos[2];
        int costo = (elementos.size() > 3) ? std::stoi(elementos[3]) : 0;

        if (costo == 0) {
            uf.unirConjuntos(ciudad1, ciudad2);
        } else {
            carreterasDeterioradas.emplace_back(id, ciudad1, ciudad2, costo);
        }
    }

    std::sort(carreterasDeterioradas.begin(), carreterasDeterioradas.end(),
              [](const Carretera& a, const Carretera& b) {
                  if (a.costo != b.costo) {
                      return a.costo < b.costo;
                  }
                  return a.id < b.id;
              });

    std::set<std::string> carreterasSeleccionadas;

    for (const auto& carretera : carreterasDeterioradas) {
        if (uf.encontrar(carretera.ciudad1) != uf.encontrar(carretera.ciudad2)) {
            uf.unirConjuntos(carretera.ciudad1, carretera.ciudad2);
            carreterasSeleccionadas.insert(carretera.id);
        }
    }

    if (raices.size() == 1) {
        std::string resultado;
        for (const auto& id : carreterasSeleccionadas) {
            resultado += id + " ";
        }
        return resultado.empty() ? "" : resultado.substr(0, resultado.length() - 1);
    } else {
        return "IMPOSIBLE";
    }
}

int main() {
    // Tu código principal aquí
    return 0;
}
