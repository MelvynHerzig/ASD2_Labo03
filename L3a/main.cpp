/*
 * File:   ShortestPath.h
 * Author: Fabien Dutoit
 *
 * Created on 21. octobre 2015, 15:50
 * Modified on 28. octobre 20201 by Berney Alec, Forestier Quentin, Herzig Melvyn.
 */

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "GraphWeighted.h"
#include "MinimumSpanningTree.h"

using namespace std;

/**
 * @brief Test si les MST issu du graphe de filename est correct en comparant deux algorithmes.
 * @param filename Nom du fichier contenant le graphe.
 * @return Retourne vrai si les algorithmes produisent un résultat similaire sinon faux.
 */
bool testMST(const string& filename) {
    cout << "Test du fichier " << filename;

    typedef GraphWeighted<double> Graph;

    Graph ewd(filename);
    cout << " (" << ewd.V() << " sommets)" << endl;

    //on execute les 2 algorithmes MST
    MinimumSpanningTree<Graph>::EdgeList toTest = MinimumSpanningTree<Graph>::BoruvkaUnionFind(ewd);
    MinimumSpanningTree<Graph>::EdgeList reference = MinimumSpanningTree<Graph>::EagerPrim(ewd);

    vector<bool> marked;
    marked.resize(ewd.V());

    double totalWeightPrim    = 0;
    double totalWeightBoruvka = 0;

    // On marque les sommets pour voir si ils sont tous relié
    for(const Graph::Edge& edge : toTest)
    {
       int v = edge.Either();
      marked[v] = true;
      marked[edge.Other(v)] = true;
      totalWeightBoruvka += edge.Weight();
   }

    // Première condition, tous les sommets sont visité une fois.
    if (!all_of(marked.begin(), marked.end(), [&](int i){ return marked[i];}))
    {
       return false;
    }

    cout << "1. Le MST de Boruvka couvre tous les sommets" << endl;


    // Deuxième condition, il y a autant d'arrêtes que de sommets - 1.
    if(toTest.size() != ewd.V() - 1)
    {
       return false;
    }
    cout << "2. Nombre d'aretes du MST de Boruvka : " << toTest.size() << endl;


    // Troisième condition, le poids des mst est égal (à un lambda près)
    for(const Graph::Edge& edge : reference)
    {
       totalWeightPrim += edge.Weight();
    }

    if(fabs(totalWeightBoruvka - totalWeightPrim) > 0.000000001)
    {
       return false;
    }

    cout << "3. Poids total du MST de Prim : " << totalWeightPrim << endl;
    cout << "   Poids total du MST de Boruvka : " << totalWeightBoruvka << endl;

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
            cout << "Les trois tests ont reussi" << endl << endl;
        } else {
            cout << "L'un des tests a echoue" << endl << endl;
        }
    }

    return EXIT_SUCCESS;
}
