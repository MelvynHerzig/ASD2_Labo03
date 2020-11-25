/* 
 * File:   ShortestPath.h
 * Author: Olivier Cuisenaire
 *
 * Created on 5. novembre 2014, 10:16
 * Modified on 18. novembre 2020 by Berney Alec, Forestier Quentin, Herzig Melvyn
 */

#ifndef ASD2_ShortestPath_h
#define ASD2_ShortestPath_h

#include <algorithm>
#include <vector>

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
class ShortestPath
{
public:

   typedef double Weight;
   typedef EdgeWeightedDirected<Weight> Edge;

   // Listes d'arcs et de poids
   typedef std::vector<Edge> Edges;
   typedef std::vector<Weight> Weights;

   // Renvoie la distance du chemin le plus court du sommet source a v
   double distanceToVertex (int v)
   {
      return distanceTo.at(v);
   }

   // Renvoie le dernier arc u->v du chemin le plus court du sommet source a v
   Edge edgeToVertex (int v)
   {
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

class BellmanFordSP : public ShortestPath<GraphType>
{

   typedef ShortestPath<GraphType> BASE;
protected:

   typedef typename BASE::Edge Edge;
   typedef typename BASE::Weight Weight;

   // Relachement de l'arc e
   void relax (const Edge &e)
   {
      int v = e.From(), w = e.To();
      Weight distThruE = this->distanceTo[v] + e.Weight();

      if (this->distanceTo[w] > distThruE)
      {
         this->distanceTo[w] = distThruE;
         this->edgeTo[w] = e;
      }
   }

public:

   // Constructeur a partir du graphe g et du sommet v a la source
   // des plus courts chemins
   BellmanFordSP (const GraphType &g, int v)
   {

      this->edgeTo.resize(g.V());
      this->distanceTo.assign(g.V(), std::numeric_limits<Weight>::max());

      this->edgeTo[v] = Edge(v, v, 0);
      this->distanceTo[v] = 0;

      for (int i = 0; i < g.V() - 1; ++i)
      {
         g.forEachEdge([this] (const Edge &e)
                       {
                          this->relax(e);
                       });
      }
   }
};

/**
 * @brief Vérifie si le graphe contient un circuit à coût négatif.
 * @tparam GraphType Type du graphe pondere oriente à traiter.
 * @details GraphType doit se comporter comme un
 *          GraphWeightedDirected et definir forEachEdge(Func),
 *          ainsi que le type GraphType::Edge. Ce dernier doit
 *          se comporter comme EdgeWeightedDirected, c-a-d definir From(),
 *          To() et Weight().
 */
template<typename GraphType>
class BellmanFordCycleDetection : public BellmanFordSP<GraphType>
{

private:
   typedef BellmanFordSP<GraphType> BASE;
   typedef typename BASE::Edge Edge;
   typedef typename BASE::Weight Weight;

   std::vector<bool> isVerified;
   int source;

   /**
    * @brief vérifie si le sommet i peut remonter jusqu'a la source.
    * @param i No du sommet à tester.
    * @return Retour le numéro de la source sinon le numéro du premier sommet
    *         rencontré appartenant à la boucle.
    */
   int checkAccessToSource (int i)
   {
      std::vector<bool> marked;
      marked.resize(isVerified.size());
      std::fill_n(marked.begin(), marked.size(), false);

      Edge e = BASE::edgeTo[i];

      // Temps qu'on ne recontre pas un sommet déjà traité.
      while (!isVerified[e.From()])
      {
         isVerified[e.From()] = true;
         marked[e.From()] = true;
         e = BASE::edgeTo[e.From()];
      }

      // Si marked, une boucle est détectée, sinon on a atteint
      // une branche de l'arbre qui à accès à la source
      return marked[e.From()] ? e.From() : source;
   }

   /**
    * @brief Calcule les arcs formant une boucle. i est un sommet de la boucle.
    * @param i Sommet issu de la boucle.
    * @return Liste des arcs formant la boucle.
    * @details Si i n'appartient pas à une boucle, retourne une liste vide.
    */
   std::list<Edge> getLoop (int i)
   {
      std::list<Edge> loop;
      Edge e = BASE::edgeTo[i];

      while (e.From() != i)
      {
         loop.push_back(e);
         e = BASE::edgeTo[e.From()];
      }

      loop.push_back(e);

      std::reverse(loop.begin(), loop.end());

      return e.From() == source ? std::list<Edge>() : loop;
   }

public:

   /**
    * @brief Constructeur a partir du graphe g et du sommet v a la source
    *        des plus courts chemins
    * @param g Graphe où calculer les plus courts chemins.
    * @param v Numéro du sommet source.
    * @Throws Lance une GraphNegativeDirectedCycleException en cas de circuit à
    *         coût négatif.
    */
   BellmanFordCycleDetection (const GraphType &g, int v) : BASE(g, v), source(v)
   {
      isVerified.resize(g.V());
      std::fill_n(isVerified.begin(), isVerified.size(), false);
      isVerified[source] = true;

      // Vérifie si chaque sommet peut atteindre la source
      g.forEachVertex([&] (int s)
                      {
                         if (!isVerified[s])
                         {
                            int vReached = checkAccessToSource(s);
                            if (vReached != source)
                            {
                               throw GraphNegativeDirectedCycleException(getLoop(vReached));
                            }
                         }
                      });

   }

   /**
    * @brief Exception si le graphe contient un circuit absorbant
    */
   class GraphNegativeDirectedCycleException : public std::exception
   {

   private:
      //liste des arcs du circuit absorbant
      const std::list<Edge> directedCycle;

   public:
      GraphNegativeDirectedCycleException (const std::list<Edge> directedCycle) noexcept
              : exception(), directedCycle(directedCycle)
      {}

      virtual const char *what () const noexcept
      {
         return "Shortest Path impossible : the graph contains a negative directed cycle";
      }

      const std::list<Edge> &Cycle () const noexcept
      {
         return directedCycle;
      }
   };
};

#endif
