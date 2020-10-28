/*
 * File:   main.cpp
 * Author: Antoine Rochat
 *
 * Created on 14. octobre 2020
 */

#include <iostream>
#include <cmath>

#include "SymbolGraphWeightedDirected.h"
#include "ShortestPath.h"
#include "GraphWeightedDirected.h"

using namespace std;

//ARGS reseau.txt
//ARGS reseau2.txt
int main(int argc, const char* argv[]) {

    if (argc < 2) {
        cerr << "Aucun fichier fourni en argument" << endl;
        return EXIT_FAILURE;
    }

    typedef GraphWeightedDirected<double> Graph;

    for (int i = 1; i < argc; ++i) {
        cout << "Fichier " << argv[i] << " :" << endl;
/****
*
*  A IMPLEMENTER
*
****/
    }

    return EXIT_SUCCESS;
}

