/* 
 * File:   GraphWeightedCommon.h
 * Author: Olivier Cuisenaire
 *
 * Created on 11. novembre 2014, 16:17
 */

#ifndef ASD2_GraphWeightedCommon_h
#define ASD2_GraphWeightedCommon_h

#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <limits>
#include <fstream>

//  Classe regroupant toutes les parties communes de
//  GraphWeighted et GraphWeightedDirected

template<typename T>  // type des edges
class GraphWeightedCommon {
public:
    // Type des arcs/arêtes.
    typedef T Edge;

    // Type de donnée pour les poids
    typedef typename Edge::WeightType WeightType;

protected:
    // Type pour une liste d'arcs/arêtes
    typedef std::list<Edge> EdgeList;

    // Structure de donnée pour les listes d'adjacences. Une EdgeList par sommet.
    std::vector<EdgeList> edgeAdjacencyLists;

public:

    // Constructeur par defaut.
    GraphWeightedCommon() {}

    // Constructeur specifiant le nombre de sommets V
    GraphWeightedCommon(int N) {
        edgeAdjacencyLists.resize(N);
    }

    // Renvoie le nombre de sommets V
    int V() const {
        return int(edgeAdjacencyLists.size());
    }

    // fonction a definir par les classes filles. Rend cette class abstraite
    virtual void addEdge(int v, int w, WeightType weight) = 0;

    // Parcours des arcs/arêtes adjacentes au sommet v.
    // la fonction f doit prendre un seul argument de type
    // EdgeWeightedGraphCommon::Edge
    template<typename Func>
    void forEachAdjacentEdge(int v, Func f) const {
        for (const Edge& e : adjacentEdges(v))
            f(e);
    }

    // Parcours de tous les sommets du graphe.
    // la fonction f doit prendre un seul argument de type int
    template<typename Func>
    void forEachVertex(Func f) const {
        for (int v = 0; v < V(); ++v)
            f(v);
    }

protected:
    // Renvoie une const reference a la list d'arcs/aretes
    // adjacent-es au sommet v
    const EdgeList& adjacentEdges(int v) const {
        return edgeAdjacencyLists.at(v);
    }

    // lecture depuis un fichier.
    void ReadFromFile(const std::string& filename) {
        std::ifstream s(filename);
        this->ReadFromStream(s);
        s.close();
    }

    // lecture depuis un stream. Utilise addEdge(...)
    // a definir par les classe fille
    void ReadFromStream(std::istream& s) {
        int V, E;

        s >> V >> E;

        edgeAdjacencyLists.resize(V);

        for (int i = 0; i < E; i++) {
            int v, w;
            WeightType weight;

            s >> v >> w >> weight;
            addEdge(v, w, weight);
        }
    }
};

#endif
