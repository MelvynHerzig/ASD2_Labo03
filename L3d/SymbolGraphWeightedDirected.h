/*
 * File:   SymbolGraph.h
 * Author: Olivier Cuisenaire
 * Modified: Raphaël Racine (2018), Antoine Rochat (2020)
 *
 * Created on 26. septembre 2014, 15:08
 */
#ifndef ASD2_SymbolGraphWeightedDirected_h
#define ASD2_SymbolGraphWeightedDirected_h

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <functional>

#include "Util.h"

template<typename GraphType>
class SymbolGraphWeightedDirected {
    typedef GraphType Graph;
private:
    Graph* g;

    std::map<std::string, int> symbolTable;
    std::vector<std::string> keys;

public:

    ~SymbolGraphWeightedDirected() {
        delete g;
    }

    //creation du SymbolGraph a partir du fichier movies.txt
    SymbolGraphWeightedDirected(const std::string& filename, const std::function<double(double)>& costFunction, char delim = '/') {
        std::string line;

        std::ifstream s(filename);
        int cnt = 0;

        while (std::getline(s, line)) {
            auto names = split(line, delim);
            if (!contains(names[0])) {
                symbolTable[names[0]] = cnt++;
            }
            if (!contains(names[1])) {
                symbolTable[names[1]] = cnt++;
            }
        }
        s.close();

        keys.resize(cnt);
        for (const auto& pair : symbolTable)
            keys[pair.second] = pair.first;

        g = new Graph(cnt);

        s.open(filename);
        while (std::getline(s, line)) {
            auto names = split(line, delim);
            g->addEdge(index(names[0]), index(names[1]), costFunction(std::stod(names[2])));
        }
        s.close();
    }

    //verifie la presence d'un symbole
    bool contains(const std::string& name) const {
        return symbolTable.find(name) != symbolTable.end();
    }

    //index du sommet correspondant au symbole
    int index(const std::string& name) const {
        return symbolTable.at(name);
    }

    //symbole correspondant au sommet
    std::string symbol(int idx) const {
        return keys[idx];
    }

    //symboles adjacents a un symbole
    std::vector<std::string> adjacent(const std::string& name) const {
        if (!contains(name)) {
            return std::vector<std::string>(0);
        }

        auto& adjIdx = g->adjacent(index(name));
        int N = adjIdx.size();
        std::vector<std::string> adjStr(N);

        std::transform(adjIdx.cbegin(), adjIdx.cend(), adjStr.begin(), [&](int i) { return this->name(i); });

        return adjStr;
    }

    const Graph& G() const {
        return *g;
    }
};

#endif
