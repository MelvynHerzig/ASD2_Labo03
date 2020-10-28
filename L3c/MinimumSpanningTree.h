/*
 * File:   MinimumSpanningTree.h
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 14:58
 */

#ifndef ASD2_MinimumSpanningTree_h
#define ASD2_MinimumSpanningTree_h

#include <queue>
#include <vector>
#include <set>
#include <functional>
#include "EdgeWeighted.h"

// Classe définissant les difféents algorithmes de calcul de l'arbre
// couvrant de poids minimum sous forme de methodes statiques.

template<typename GraphType> // Type du graphe pondere non oriente a traiter
// GraphType doit se comporter comme un
// EdgeWeighted et definir forEachEdge(Func),
// forEachAdjacentEdge(int,Func) ainsi que le
// type GraphType::Edge, qui doit se comporter
// comme EdgeWeighted, c-a-dire definir Either(),
// Other(int), et operator<
class MinimumSpanningTree {
public:
    typedef double Weight;
    typedef EdgeWeighted<Weight> Edge;

    // Type liste d'arêtes.
    typedef std::vector<Edge> EdgeList;

public:

    // Algorithme de Prim en version stricte. Utilise une queue de priorite
    // pour les sommets a traiter. Celle ci est mise en oeuvre avec std::set.

    static EdgeList EagerPrim(const GraphType& g) {

        EdgeList output;
        output.reserve(g.V() - 1);

        typedef std::pair<Edge, int> EdgeVertex;     // paire arc/sommet.
        // operator< s'applique sur le premier
        // element de la paire, puis le deuxieme
        // si egalite.

        std::set<EdgeVertex> pq;                    // queue de priorite
        std::vector<Edge> edge(g.V());              // arc le plus leger pour joindre chaque sommet
        // a l'arbre courrant.
        std::vector<bool> marked(g.V(), false);

        marked[0] = true;
        g.forEachAdjacentEdge(0, [&](const Edge& e) {
            int w = e.Other(0);
            edge[w] = e;
            pq.insert(std::make_pair(e, w)); // set::insert() correspond à priority_queue::push().
        });

        while (!pq.empty() && output.size() < g.V() - 1) {

            Edge e = pq.begin()->first;     // set::begin() correspond à priority_queue::top().
            output.push_back(e);

            int v = pq.begin()->second;
            marked[v] = true;

            pq.erase(pq.begin());            // correspond à priority_queue::pop().

            g.forEachAdjacentEdge(v, [&](const Edge& e) {
                int w = e.Other(v);
                if (!marked[w] && edge[w] > e) {
                    pq.erase(std::make_pair(edge[w], w));    // deux operations pour decrease_key
                    pq.insert(std::make_pair(e, w));
                    edge[w] = e;
                }
            });
        }
        return output;
    }
};

#endif
