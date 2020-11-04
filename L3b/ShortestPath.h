/* 
 * File:   ShortestPath.h
 * Author: Olivier Cuisenaire
 *
 * Created on 5. novembre 2014, 10:16
 * Modified on 4. novembre 2020 by Berney Alec, Forestier Quentin, Herzig Melvyn
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
// classes, ainsi que les methodes DistanceTo et EdgeTo qui permettent
// de les interroger.
//
// Le calcul des plus courts chemins est fait dans les constructeurs
// des classes derivees.

template<typename GraphType>
// Type du graphe pondere oriente a traiter
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

private:
    typedef ShortestPath<GraphType> BASE;
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

/**
 * @brief Implémentation de l'algorithme de Dijkstra.
 * @tparam GraphType Doit se comporter comme un GraphWeightedDirected et définir V(),
 *         forEachVertex(Func) et forEachAdjacentEdge(int, Func), ainsi que le type
 *         GraphType::Edge. Ce dernier doit se comporter comme EdgeWeightedDirected,
 *         c-a-dire definir From(), To et Weight.
 */
template<typename GraphType>
class DijkstraSP : public ShortestPath<GraphType> {
public:

    typedef ShortestPath<GraphType> BASE;
    typedef typename BASE::Edge Edge;
    typedef typename BASE::Weight Weight;
    typedef std::pair<Weight, int> WeightVertex;

private:

    /**
     * @brief Priority Queue des paires <Poids, sommet> à traîter.
     */
    std::set<WeightVertex> PQ;

    /**
     * @brief Retire la paire <poids, sommet> ou poids est le plus petit de PQ.
     * @return WeightVertex contenant le plus petit poids de PQ.
     */
    WeightVertex extract_min()
    {
       WeightVertex e = *PQ.begin();
       PQ.erase(PQ.begin());
       return e;
    }

    /**
     * @brief Retire la paire <oldWeight, v> et la remplace par <newWeight, v> dans PQ.
     * @param oldWeight Ancien poids de v.
     * @param newWeight Nouveau poids de v.
     * @param v Sommet v.
     */
    void decrease_priority(Weight oldWeight, Weight newWeight, int v)
    {
       PQ.erase(std::make_pair(oldWeight, v));
       PQ.insert(std::make_pair(newWeight, v));
    };

    /**
     * @brief Ajoute la paire WeightVertex (w, i) à la pq.
     * @param i Numéro du sommet.
     * @param w Poids jusqu'au sommet.
     */
    void add_with_priority(int i, Weight w)
    {
       PQ.insert(std::make_pair(w, i));
    }

    /**
     * @brief Relâche l'arc e.
     * @param e Arc à relâcher.
     */
    void relax(const Edge& e) {
       int v = e.From(), w = e.To();
       Weight distThruE = this->distanceTo[v] + e.Weight();

       if (this->distanceTo[w] > distThruE) {
          decrease_priority(this->distanceTo[w], distThruE, w);
          this->distanceTo[w] = distThruE;
          this->edgeTo[w] = e;
       }
    }

public:

    /**
     * @brief Applique l'algorithme de Dijkstra au graphe g depuis v.
     * @param g Graphe à traîter.
     * @param v Sommet de départ.
     */
    DijkstraSP(const GraphType& g, int v) {

       //Initialisation
       this->edgeTo.resize(g.V());
       this->distanceTo.assign(g.V(), std::numeric_limits<Weight>::max());

       this->edgeTo[v] = Edge(v, v, 0);
       this->distanceTo[v] = 0;

       g.forEachVertex([&](int i)
       {
         add_with_priority(i, this->distanceTo[i]);
       });

       //Traitement de chaque plus petite paire WeightVertex restante dans PQ.
       while(!PQ.empty())
       {
          WeightVertex ew = extract_min();

          g.forEachAdjacentEdge(ew.second, [&](const Edge& e){
             relax(e);
          });
       }
    }
};


#endif
