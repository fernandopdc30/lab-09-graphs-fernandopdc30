#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

class Union {
private:
    map<string, string> padre;
    map<string, int> rango;

public:
    void hacerConjunto(const string& elemento) {
        padre[elemento] = elemento;
        rango[elemento] = 0;
    }

    string encontrar(const string& elemento) {
        if (padre[elemento] != elemento)
            padre[elemento] = encontrar(padre[elemento]);
        return padre[elemento];
    }

    bool unirConjuntos(const string& a, const string& b) {
        string raizA = encontrar(a);
        string raizB = encontrar(b);

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

class Carretera {
public:
    string id;
    string ciudad1;
    string ciudad2;
    int costo;

    Carretera(string i, string c1, string c2, int co = 0) : id(i), ciudad1(c1), ciudad2(c2), costo(co) {}
};

void inicializarConjuntos(Union& unionCiudad, set<string>& ciudades, const vector<string>& listaCarreteras) {
    for (const auto& carreteraStr : listaCarreteras) {
        stringstream ss(carreteraStr);
        string id, ciudad1, ciudad2;
        ss >> id >> ciudad1 >> ciudad2;
        unionCiudad.hacerConjunto(ciudad1);
        unionCiudad.hacerConjunto(ciudad2);
        ciudades.insert(ciudad1);
        ciudades.insert(ciudad2);
    }
}

bool todasCiudadesConectadas(Union& unionCiudad, const set<string>& ciudades) {
    string raiz = unionCiudad.encontrar(*ciudades.begin());
    for (const auto& ciudad : ciudades) {
        if (unionCiudad.encontrar(ciudad) != raiz) {
            return false;
        }
    }
    return true;
}

string reconstruye(vector<string> listaCarreteras) {
    vector<Carretera> carreterasDeterioradas;
    Union unionCiudad;
    set<string> ciudades;
    set<string> carreterasSeleccionadas;

    inicializarConjuntos(unionCiudad, ciudades, listaCarreteras);

    for (const auto& carreteraStr : listaCarreteras) {
        stringstream ss(carreteraStr);
        string id, ciudad1, ciudad2;
        int costo = 0;
        ss >> id >> ciudad1 >> ciudad2;
        if (!(ss >> costo)) {
            unionCiudad.unirConjuntos(ciudad1, ciudad2);
        } else {
            carreterasDeterioradas.emplace_back(id, ciudad1, ciudad2, costo);
        }
    }

    sort(carreterasDeterioradas.begin(), carreterasDeterioradas.end(), [](const Carretera& a, const Carretera& b) {
        return (a.costo != b.costo) ? a.costo < b.costo : a.id < b.id;
    });

    for (const auto& carretera : carreterasDeterioradas) {
        if (unionCiudad.encontrar(carretera.ciudad1) != unionCiudad.encontrar(carretera.ciudad2)) {
            unionCiudad.unirConjuntos(carretera.ciudad1, carretera.ciudad2);
            carreterasSeleccionadas.insert(carretera.id);
        }
    }

    if (todasCiudadesConectadas(unionCiudad, ciudades)) {
        string resultado;
        for (const auto& id : carreterasSeleccionadas) {
            resultado += id + " ";
        }
        return resultado.empty() ? "" : resultado.substr(0, resultado.length() - 1);
    } else {
        return "IMPOSIBLE";
    }
}
