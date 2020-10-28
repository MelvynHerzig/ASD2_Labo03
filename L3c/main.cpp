/*
 * File:   main.cpp
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 16:24
 * Updated on 16. aout 2019 by Antoine Rochat
 */

#include <iostream>
#include "TrainNetwork.h"
#include "TrainGraphWrapper.h"

#include "MinimumSpanningTree.h"
#include "ShortestPath.h"

using namespace std;

// Calcule et affiche le plus court chemin de la ville depart a la ville arrivee
// en passant par le reseau ferroviaire tn. Le critere a optimiser est la distance.
void PlusCourtChemin(const string& depart, const string& arrivee, TrainNetwork& tn) {
/****
*
*  A IMPLEMENTER
*
****/
}

// Calcule et affiche le plus court chemin de la ville depart a la ville arrivee
// en passant par le reseau ferroviaire tn ayant une ville en travaux et donc
// inaccessible. Vous pouvez mettre un cout infini aux arcs ayant comme depart ou
// comme arrivee cette ville en travaux. Le critere a optimiser est la distance.
void PlusCourtCheminAvecTravaux(const string& depart, const string& arrivee, const string& gareEnTravaux, TrainNetwork& tn) {
/****
*
*  A IMPLEMENTER
*
****/
}

// Calcule et affiche le plus rapide chemin de la ville depart a la ville arrivee via la ville "via"
// en passant par le reseau ferroviaire tn. Le critere a optimiser est le temps de parcours
void PlusRapideChemin(const string& depart, const string& arrivee, const string& via, TrainNetwork& tn) {
/****
*
*  A IMPLEMENTER
*
****/
}


// Calcule et affiche le plus reseau a renover couvrant toutes les villes le moins cher.
// Le prix pour renover 1 km de chemin de fer est de :
// - 15 CHF par km pour les lignes ayant 4 voies
// - 10 CHF par km pour les lignes ayant 3 voies
// - 6 CHF par km pour les lignes ayant 2 voies
// - 3 CHF par km pour les lignes ayant 1 voie
void ReseauLeMoinsCher(TrainNetwork& tn) {
/****
*
*  A IMPLEMENTER
*
****/
}

//ARGS reseau.txt
int main(int argc, const char* argv[]) {

    if (argc != 2) {
        cerr << "Le programme attend un seul fichier en argument" << endl;
        return EXIT_FAILURE;
    }

    TrainNetwork tn(argv[1]);

    cout << "1. Chemin le plus court entre Geneve et Coire" << endl;

    PlusCourtChemin("Geneve", "Coire", tn);

    cout << "2. Chemin le plus court entre Geneve et Coire, avec la gare de Sion en travaux" << endl;

    PlusCourtCheminAvecTravaux("Geneve", "Coire", "Sion", tn);

    cout << "3. Chemin le plus rapide entre Geneve et Coire en passant par Brigue" << endl;

    PlusRapideChemin("Geneve", "Coire", "Brigue", tn);

    cout << "4. Chemin le plus rapide entre Lausanne et Zurich en passant par Bale" << endl;

    PlusRapideChemin("Lausanne", "Zurich", "Bale", tn);

    cout << "5. Quelles lignes doivent etre renovees ? Quel sera le cout de la renovation de ces lignes ?" << endl;

    ReseauLeMoinsCher(tn);

    return EXIT_SUCCESS;
}
