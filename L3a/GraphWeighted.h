/* 
 * File:   GraphWeighted.h
 * Author: Olivier Cuisenaire
 *
 * Created on 05. novembre 2014, 13:26
 */

#ifndef ASD2_GraphWeighted_h
#define ASD2_GraphWeighted_h

#include "GraphWeightedCommon.h"
#include "EdgeWeighted.h"

// Classe definissant un graphe pondere non-oriente.
// Elle herite de GraphWeightedCommon en
// specifiant des aretes de type EdgeWeighted<T>

template<typename T> // Type du poids, par exemple int ou double
// T doit etre comparable, et être un parametre
// valide pour std::numeric_limits
class GraphWeighted : public GraphWeightedCommon<EdgeWeighted<T>> {
// defini la class mere comme BASE.
    typedef GraphWeightedCommon<EdgeWeighted<T>> BASE;

public:
// Type des arêtes.
    typedef typename BASE::Edge Edge;

    // Type de donnée pour les poids
    typedef typename BASE::WeightType WeightType;

    // Constructeur a partir d'un nom de fichier
    GraphWeighted(const std::string& filename) {
        this->ReadFromFile(filename);
    }

    // Constructeur a partie d'un stream
    GraphWeighted(std::istream
                  & s) {
        this->ReadFromStream(s);
    }

    // Constructeur specifiant le nombre de sommet
    // Il faudra appeler addEdge pour ajouter les
    // aretes
    GraphWeighted(int N) : BASE(N) {
    }

    // Ajoute une arete de poids weight entre v et w
    // On ajoute cette arete aux listes d'adjacence des
    // deux sommets
    virtual void addEdge(int v, int w, WeightType weight) {
        this->edgeAdjacencyLists.at(v).push_back(Edge(v, w, weight));
        if (v != w) this->edgeAdjacencyLists.at(w).push_back(Edge(v, w, weight));
    }

    // Parcours de tous les sommets adjacents au sommet v
    // la fonction f doit prendre un seul argument de type int
    template<typename Func>
    void forEachAdjacentVertex(int v, Func f) const {
        for (const Edge& e : this->adjacentEdges(v))
            f(e.Other(v));
    }

    // Parcours de toutes les arêtes du graphe.
    // la fonction f doit prendre un seul argument de type
    // EdgeWeightedGraph::Edge
    template<typename Func>
    void forEachEdge(Func f) const {
        for (int v = 0; v < this->V(); ++v)
            for (const Edge& e : this->adjacentEdges(v))
                if (e.Other(v) >= v)
                    f(e);
    }

};

#endif
