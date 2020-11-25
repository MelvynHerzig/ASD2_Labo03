/*
 * File:   main.cpp
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 16:24
 * Updated on 16. aout 2019 by Antoine Rochat
 * Modified on 13. novembre 2020 by Berney Alec, Forestier Quentin, Herzig Melvyn
 */

#include <iostream>
#include "TrainNetwork.h"
#include "TrainGraphWrapper.h"

#include "MinimumSpanningTree.h"
#include "ShortestPath.h"

using namespace std;

using costFunction = function<double (const TrainNetwork::Line &)>;

/**
 * @brief Renvoie l'itineraire entre la ville de départ et d'arrivée en respectant fCost.
 * @param vDepart No de sommet de la ville de départ.
 * @param vArriveeNo de sommet de la ville d'arrivée.
 * @param fCost Fonction de coût à appliquer sur les lignes de trainNetwork
 * @param tn Trainnetwork Contenant les sommet et les arrêtes du graphe à traiter.
 * @param outDist Variable où stocker le coût total entre départ et arrivée.
 * @param outItineraire Variable où stocker le string qui décrit l'itinéraire.
 */
void chemin(int vDepart, int vArrivee, costFunction fCost,const TrainNetwork &tn, double* outDist, string* outItineraire)
{
   // Création de l'arborescence BelmanFord sur la trainNetwork
   TrainGraphWrapperDirected tdgw(tn, fCost);
   BellmanFordSP<TrainGraphWrapperDirected> sp(tdgw, vDepart);

   // Retour de la distance depuis la ville de départ jusqu'a l'arrivée.
   *outDist = sp.distanceToVertex(vArrivee);

   // Construction de l'itinéraire.
   string itineraire;

   itineraire += tn.cities[sp.PathTo(vArrivee)[0].From()].name;
   for(const auto& e : sp.PathTo(vArrivee))
   {
      itineraire += " -> " + tn.cities[e.To()].name;
   }

   // Retour de l'itinéraire.
   *outItineraire = itineraire;
}

/**
 * @brief Calcule et affiche le plus court chemin de la ville départ a la ville arrivée
 *        en passant par le réseau ferroviaire tn.
 * @param depart Nom de la ville de départ.
 * @param arrivee Nom de la ville d'arrivée
 * @param tn Réseau ferroviaire.
 */
void PlusCourtChemin (const string &depart, const string &arrivee, TrainNetwork &tn)
{
   int noDepart  = tn.cityIdx.find(depart)->second;
   int noArrivee = tn.cityIdx.find(arrivee)->second;

   costFunction getLength = [&] (const TrainNetwork::Line& line)
   {
      return line.length;
   };

   double distance;
   string itineraire;

   chemin(noDepart, noArrivee, getLength, tn, &distance, &itineraire);

   cout << "Longueur = " << distance << " km" << endl;
   cout << "Via " << itineraire << endl;

}


/**
 * @brief Calcule et affiche le plus court chemin de la ville départ a la ville arrivée
 *        en passant par le réseau ferroviaire tn ayant une ville en travaux  inaccessible.
 * @param depart Nom de la ville de départ.
 * @param arrivee Nom de la ville d'arrivée
 * @param gareEnTravaux Nom de la ville où la gare est en travaux.
 * @param tn Réseau ferroviaire.
 */
void PlusCourtCheminAvecTravaux (const string &depart, const string &arrivee, const string &gareEnTravaux, TrainNetwork &tn)
{
   int noDepart  = tn.cityIdx.find(depart)->second;
   int noArrivee = tn.cityIdx.find(arrivee)->second;
   int noTravaux = tn.cityIdx.find(gareEnTravaux)->second;

   costFunction getLength = [&] (const TrainNetwork::Line &line)
   {
      //Si la gare de départ est la gare en travaux
      if( noTravaux == line.cities.first) return numeric_limits<double>::max();
      return line.length;
   };
   
   double distance;
   string itineraire;

   chemin(noDepart, noArrivee, getLength, tn, &distance, &itineraire);

   cout << "Longueur = " << distance << " km" << endl;
   cout << "Via " << itineraire << endl;

}

/**
 * @brief Calcule et affiche le plus rapide chemin de la ville départ a la ville arrivée via la ville "via"
 *        en passant par le réseau ferroviaire tn.
 * @param depart Nom de la ville de départ.
 * @param arrivee Nom de la ville d'arrivée.
 * @param via Nom de la ville intermédiaire.
 * @param tn Réseau ferroviaire.
 */
void PlusRapideChemin (const string &depart, const string &arrivee, const string &via, TrainNetwork &tn)
{
   costFunction getTime = [] (const TrainNetwork::Line &line)
   {
      return line.duration;
   };

   int noDepart  = tn.cityIdx.find(depart)->second;
   int noArrivee = tn.cityIdx.find(arrivee)->second;
   int noVia     = tn.cityIdx.find(via)->second;

   double distance1, distance2;
   string itineraire1, itineraire2;

   chemin(noDepart, noVia, getTime, tn, &distance1, &itineraire1);
   chemin(noVia, noArrivee, getTime, tn, &distance2, &itineraire2);

   cout << "Temps = " << distance1 + distance2 << " minutes" << endl;
   cout << "Via " << itineraire1 << itineraire2.substr(via.size()) << endl;
}

/**
 * @brief Calcule et affiche le réseau à rénover le moins chère possible, couvrant toutes les villes.
 * @param tn Réseau ferroviaire.
 * @details Le prix pour rénover 1 km de chemin de fer est de :
 *              - 15 CHF par km pour les lignes ayant 4 voies
 *              - 10 CHF par km pour les lignes ayant 3 voies
 *              - 6 CHF par km pour les lignes ayant 2 voies
 *              - 3 CHF par km pour les lignes ayant 1 voie
 */
void ReseauLeMoinsCher (TrainNetwork &tn)
{
   costFunction getPrice = [] (const TrainNetwork::Line &line)
   {
      switch(line.nbTracks)
      {
         case 1: return 3  * line.length;
         case 2: return 6  * line.length;
         case 3: return 10 * line.length;
         case 4: return 15 * line.length;
         default: return numeric_limits<double>::max();
      }
   };

   TrainGraphWrapper tgw(tn, getPrice);
   auto mst = MinimumSpanningTree<TrainGraphWrapper>::EagerPrim(tgw);

   double coutTotal;

   for(const auto& edge : mst)
   {
      coutTotal += edge.Weight();
      cout << tn.cities[edge.Either()].name             << " - "
           << tn.cities[edge.Other(edge.Either())].name << " : "
           << edge.Weight() << " MF" << endl;
   }

   cout << "Coût Total: " << coutTotal << " MF" << endl;
}

//ARGS reseau.txt
int main (int argc, const char *argv[])
{

   if (argc != 2)
   {
      cerr << "Le programme attend un seul fichier en argument" << endl;
      return EXIT_FAILURE;
   }

   TrainNetwork tn(argv[1]);

   cout << "1. Chemin le plus court entre Geneve et Coire" << endl;

   PlusCourtChemin("Geneve", "Coire", tn);

   cout << "\n2. Chemin le plus court entre Geneve et Coire, avec la gare de Sion en travaux" << endl;

   PlusCourtCheminAvecTravaux("Geneve", "Coire", "Sion", tn);

   cout << "\n3. Chemin le plus rapide entre Geneve et Coire en passant par Brigue" << endl;

   PlusRapideChemin("Geneve", "Coire", "Brigue", tn);

   cout << "\n4. Chemin le plus rapide entre Lausanne et Zurich en passant par Bale" << endl;

   PlusRapideChemin("Lausanne", "Zurich", "Bale", tn);

   cout << "\n5. Quelles lignes doivent etre renovees ? Quel sera le cout de la renovation de ces lignes ?" << endl;

   ReseauLeMoinsCher(tn);

   return EXIT_SUCCESS;
}
