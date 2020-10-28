/*
 * File:   ShortestPath.h
 * Author: Fabien Dutoit
 *
 * Created on 21. octobre 2015, 15:50
 */

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "GraphWeighted.h"
#include "MinimumSpanningTree.h"

using namespace std;

// compare les algorithmes de Boruvska et Prim pour calculer le MST, les vecteurs de Edges resultants sont compares
// retourne true si les algorithmes produisent un resultat similaire, faux sinon
bool testMST(const string& filename) {
    cout << "Test du fichier " << filename;

    typedef GraphWeighted<double> Graph;
    Graph ewd(filename);
    cout << " (" << ewd.V() << " sommets)" << endl;

    //on execute les 2 algorithmes MST
    MinimumSpanningTree<Graph>::EdgeList toTest = MinimumSpanningTree<Graph>::BoruvkaUnionFind(ewd);
    MinimumSpanningTree<Graph>::EdgeList reference = MinimumSpanningTree<Graph>::EagerPrim(ewd);

/****
*
*  A IMPLEMENTER
*  Si les poids des aretes ne sont pas tous differents, la solution d'un algorithme MST n'est pas toujours unique.
*  Ici, on teste la correction du MST de Boruvka via trois proprietes :
*    1. Tous les sommets sont presents dans ses aretes.
*    2. Le MST a exactement g.V()-1 aretes.
*    3. Les sommes des poids des aretes des deux MST (Prim et Boruvka) sont identiques. (attention aux comparaisons de double en C++)
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
            cout << "Les trois tests ont reussi" << endl << endl;
        } else {
            cout << "L'un des tests a echoue" << endl << endl;
        }
    }

    return EXIT_SUCCESS;
}
