/* 
 * File:   ShortestPath.h
 * Author: Olivier Cuisenaire
 *
 * Created on 5. novembre 2014, 10:16
 */

#ifndef ASD2_ShortestPath_h
#define ASD2_ShortestPath_h

#include <algorithm>
#include <queue>
#include <vector>
#include <set>
#include <functional>
#include "EdgeWeightedDirected.h"

// Classe parente de toutes les classes de plus court chemin.
// Defini les membres edgeTo et distanceTo commun à toutes ces
// classes, ainsi que les methodes DistanceTo, EdgeTo et PathTo
// qui permettent de les interroger.
//
// Le calcul des plus courts chemins est fait dans les constructeurs
// des classes derivees.

template<typename GraphType>   // Type du graphe pondere oriente a traiter
// GraphType doit se comporter comme un
// GraphWeightedDirected et definir le
// type GraphType::Edge
class ShortestPath {
public:

    typedef double Weight;
    typedef EdgeWeightedDirected<Weight> Edge;

    // Listes d'arcs et de poids
    typedef std::vector<Edge> Edges;
    typedef std::vector<Weight> Weights;

    // Renvoie la distance du chemin le plus court du sommet source a v
    double distanceToVertex(int v) {
        return distanceTo.at(v);
    }

    // Renvoie le dernier arc u->v du chemin le plus court du sommet source a v
    Edge edgeToVertex(int v) {
        return edgeTo.at(v);
    }

protected:
    Edges edgeTo;
    Weights distanceTo;
};

// Algorithme de BellmanFord.

template<typename GraphType> // Type du graphe pondere oriente a traiter
// GraphType doit se comporter comme un
// GraphWeightedDirected et definir forEachEdge(Func),
// ainsi que le type GraphType::Edge. Ce dernier doit
// se comporter comme EdgeWeightedDirected, c-a-dire definir From(),
// To() et Weight()

class BellmanFordSP : public ShortestPath<GraphType> {

    typedef ShortestPath<GraphType> BASE;
protected:

    typedef typename BASE::Edge Edge;
    typedef typename BASE::Weight Weight;

    // Relachement de l'arc e
    void relax(const Edge& e) {
        int v = e.From(), w = e.To();
        Weight distThruE = this->distanceTo[v] + e.Weight();

        if (this->distanceTo[w] > distThruE) {
            this->distanceTo[w] = distThruE;
            this->edgeTo[w] = e;
        }
    }

public:

    // Constructeur a partir du graphe g et du sommet v a la source
    // des plus courts chemins
    BellmanFordSP(const GraphType& g, int v) {

        this->edgeTo.resize(g.V());
        this->distanceTo.assign(g.V(), std::numeric_limits<Weight>::max());

        this->edgeTo[v] = Edge(v, v, 0);
        this->distanceTo[v] = 0;

        for (int i = 0; i < g.V() - 1; ++i)
            g.forEachEdge([this](const Edge& e) {
                this->relax(e);
            });
    }
};

template<typename GraphType> // Type du graphe pondere oriente a traiter
// GraphType doit se comporter comme un
// GraphWeightedDirected et definir forEachEdge(Func),
// ainsi que le type GraphType::Edge. Ce dernier doit
// se comporter comme EdgeWeightedDirected, c-a-dire definir From(),
// To() et Weight()
class BellmanFordCycleDetection : public BellmanFordSP<GraphType> {

private:
    typedef BellmanFordSP<GraphType> BASE;
    typedef typename BASE::Edge Edge;
    typedef typename BASE::Weight Weight;

public:

    // Constructeur a partir du graphe g et du sommet v a la source
    // des plus courts chemins
    BellmanFordCycleDetection(const GraphType& g, int v) : BASE(g, v) {
/****
*
*  A IMPLEMENTER
*  Doit lancer une GraphNegativeDirectedCycleException si un circuit absorbant est detecté
*
****/
    }

    //exception si le graphe contient un circuit absorbant
    class GraphNegativeDirectedCycleException : public std::exception {

    private:
        //liste des arcs du circuit absorbant
        const std::list<Edge> directedCycle;

    public:
        GraphNegativeDirectedCycleException(const std::list<Edge> directedCycle) noexcept: exception(), directedCycle(directedCycle) {}

        virtual const char* what() const noexcept {
            return "Shortest Path impossible : the graph contains a negative directed cycle";
        }

        const std::list<Edge>& Cycle() const noexcept {
            return directedCycle;
        }
    };
};

#endif