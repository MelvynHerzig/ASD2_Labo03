/*
 * File:   main.cpp
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 16:24
 * Updated on 16. aout 2019 by Antoine Rochat
 * Modified on 04. novembre 2020 by Berney Alec, Forestier Quentin, Herzig Melvyn
 */

#include <iostream>
#include <cmath>

#include "ShortestPath.h"
#include "GraphWeightedDirected.h"

using namespace std;

/**
 * @brief Vérifie la validité de l'algorithme de Dijkstra par comparaison avec BellmanFord.
 * @param filename Nom du ficher à partir duquel générer les graphes.
 * @return True si les chemin jusqu'a tout les sommets sont de même poids.
 */
bool testMST(const string& filename) {
   cout << "Test du fichier " << filename;

   typedef GraphWeightedDirected<double> Graph;
   Graph ewd(filename);
   cout << " (" << ewd.V() << " sommets)" << endl;

   //on execute les 2 algorithmes ShortestPath
   BellmanFordSP<Graph> referenceSP(ewd, 0);
   DijkstraSP<Graph> testSP(ewd, 0);

   for(int i = 1; i < ewd.V(); ++i)
   {
      if(fabs(referenceSP.distanceToVertex(i) - testSP.distanceToVertex(i)) > 0.000001)
      {
         return false;
      }
   }

   return true;
}

//ARGS tinyEWD.txt
//ARGS mediumEWD.txt
//ARGS 1000EWD.txt
int main(int argc, const char* argv[]) {

   if (argc < 2) {
      cerr << "Aucun fichier fourni en argument" << endl;
      return EXIT_FAILURE;
   }

   for (int i = 1; i < argc; ++i) {
      if (testMST(argv[i])) {
         cout << "Le test a reussi" << endl << endl;
      } else {
         cout << "La verification a echouee" << endl << endl;
      }
   }

   return EXIT_SUCCESS;
}
