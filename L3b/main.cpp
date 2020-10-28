/*
 * File:   main.cpp
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 16:24
 * Updated on 16. aout 2019 by Antoine Rochat
 */

#include <iostream>
#include <cmath>

#include "ShortestPath.h"
#include "GraphWeightedDirected.h"

using namespace std;

// compare les algorithmes Dijkstra et BellmanFord pour calculer les plus courts chemins au
// sommet 0 dans le graphe defini par filename.
// retourne true si les algorithmes produisent un resultat similaire, faux sinon
bool testMST(const string& filename) {
    cout << "Test du fichier " << filename;

    typedef GraphWeightedDirected<double> Graph;
    Graph ewd(filename);
    cout << " (" << ewd.V() << " sommets)" << endl;

    //on execute les 2 algorithmes ShortestPath
    BellmanFordSP<Graph> referenceSP(ewd, 0);
    DijkstraSP<Graph> testSP(ewd, 0);

/****
*
*  A IMPLEMENTER
*  la solution d'un algorithme ShortestPath n'est pas forcement unique
*  on va verifier que:
*    1. pour chaque sommet du graphe, la distance du chemin le plus court depuis le sommet 0 est la même pour les deux algorithmes
*    (attention aux comparaisons de double en C++)
*
****/
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

