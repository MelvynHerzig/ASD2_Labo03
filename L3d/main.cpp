/*
 * File:   main.cpp
 * Author: Antoine Rochat
 *
 * Created on 14. octobre 2020
 * Modified on 18. novembre 2020 by Berney Alec, Forestier Quentin, Herzig Melvyn
 */

#include <iostream>
#include <cmath>

#include "SymbolGraphWeightedDirected.h"
#include "ShortestPath.h"
#include "GraphWeightedDirected.h"

using namespace std;
using costFunction = const std::function<double (double)>;

/*
 * @brief Affiche la transformation de 1 par chaque poids des arcs de loop issu d'un
 *        graphe à symboles
 * @details Edge doit implémenter int From() const, int To() const, double Weight() const
 *          SymbolGraphe doit implémenter std::string symbol(int idx) const.
 */
template<typename Edge, typename SymbolGraphe>
void afficherCircuit (const std::list<Edge> &loop, const SymbolGraphe &sg)
{
   double result = 1;
   double convertedWeight;

   for (const Edge &e : loop)
   {
      convertedWeight = exp(-e.Weight());
      cout << result << " " << sg.symbol(e.From()) << " (x" << convertedWeight << ")" << " -> ";
      result *= convertedWeight;
   }

   cout << result << " " << sg.symbol(loop.back().To()) << endl;
}

//ARGS reseau.txt
//ARGS reseau2.txt
int main (int argc, const char *argv[])
{

   if (argc < 2)
   {
      cerr << "Aucun fichier fourni en argument" << endl;
      return EXIT_FAILURE;
   }

   typedef GraphWeightedDirected<double> Graph;

   costFunction getTaux = [&] (double d)
   {
      // Avec cette transformation, on ramnène les multiplication du problème
      // d'arbitrage, à des addition.
      // On inverse les nombre ainsi obtenus, pour que les conversion qui permettent
      // des gains ( > 1) soient négatives et ainsi détectables par BellmanFord.
      return -log(d);
   };

   for (int i = 1; i < argc; ++i)
   {
      cout << "Fichier " << argv[i] << " :" << endl;
      SymbolGraphWeightedDirected<Graph> symbolGraph(argv[i], getTaux, ';');

      try
      {
         BellmanFordCycleDetection<Graph> bfcd(symbolGraph.G(), 0);
      }
      catch (const BellmanFordCycleDetection<Graph>::GraphNegativeDirectedCycleException& e)
      {

         cout << "Circuit absorbant détecté :" << endl;

         afficherCircuit(e.Cycle(), symbolGraph);

         return EXIT_FAILURE;
      }

      cout << "Aucun circuit absorbant détecté" << endl;
   }

   return EXIT_SUCCESS;
}