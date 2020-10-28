/* 
 * File:   GraphWeightedDirected.h
 * Author: Olivier Cuisenaire
 *
 * Created on 05. novembre 2014, 13:26
 */

#ifndef WeightedDirectedGraph
#define WeightedDirectedGraph

#include "GraphWeightedCommon.h"
#include "EdgeWeightedDirected.h"

// Classe definissant un graphe pondere oriente.
// Elle herite de GraphWeightedCommon en
// specifiant des aretes de type EdgeWeightedDirected<T>

template<typename T> // Type du poids, par exemple int ou double
// T doit etre comparable, et être un parametre
// valide pour std::numeric_limits
class GraphWeightedDirected : public GraphWeightedCommon<EdgeWeightedDirected<T>

> {
// defini la class mere comme BASE.
    typedef GraphWeightedCommon<EdgeWeightedDirected<T>> BASE;

public:
// Type des arcs
    typedef typename BASE::Edge Edge;

// Type de donnée pour les poids
    typedef typename BASE::WeightType WeightType;

// Constructeur a partir d'un nom de fichier
    GraphWeightedDirected(const std::string& filename) {
        this->ReadFromFile(filename);
    }

// Constructeur a partie d'un stream
    GraphWeightedDirected(std::istream& s) {
        this->ReadFromStream(s);
    }

// Constructeur specifiant le nombre de sommet
// Il faudra appeler addEdge pour ajouter les arcs
    GraphWeightedDirected(int N) : BASE(N) {}

// Ajoute un arcs de poids weight de v q w
// On ajoute cette arete à la liste d'adjacence de v
    virtual void addEdge(int v, int w, WeightType weight) {
        this->edgeAdjacencyLists.at(v).push_back(Edge(v, w, weight));
    }

// Parcours de tous les sommets adjacents au sommet v
// la fonction f doit prendre un seul argument de type int
    template<typename Func>
    void forEachAdjacentVertex(int v, Func f) const {
        for (const Edge& e : this->adjacentEdges(v))
            f(e.To());
    }

// Parcours de tous les arcs du graphe.
// la fonction f doit prendre un seul argument de type
// GraphWeightedDirected::Edge
    template<typename Func>
    void forEachEdge(Func f) const {
        for (int v = 0; v < BASE::V(); ++v)
            for (const Edge& e : this->adjacentEdges(v))
                f(e);
    }

};

#endif
