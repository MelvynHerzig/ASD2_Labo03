/*
 * File:   TrainGraphWrapper.h
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 16:24
 * Updated on 16. aout 2019 by Antoine Rochat
 * Modified on 13. novembre 2020 by Berney Alec, Forestier Quentin, Herzig Melvyn
 */

#ifndef ASD2_TrainGraphWrapper_h
#define ASD2_TrainGraphWrapper_h

#include <functional>
#include "TrainNetwork.h"
#include "EdgeWeighted.h"
#include "EdgeWeightedDirected.h"


class TrainGraphWrapperCommon
{
protected:
   const TrainNetwork &tn;

   // Fonction de cout
   // Correspond typiquement à une expression lambda qui prend une ligne du
   // TrainNetwork en paramètre et retourne un double qui correspond au cout de
   // cette ligne.
   const std::function<double (const TrainNetwork::Line &)> &costFunction;

   TrainGraphWrapperCommon (const TrainNetwork &tn,
                            const std::function<double (const TrainNetwork::Line &)> &costFunction)
           : tn(tn), costFunction(costFunction)
   {}

public:

   /**
    * @brief Retourne le nombre de sommets dans le graphe, c-à-d le nb de villes du réseau ferroviaire.
    * @return Retourne le nombre de sommets dans le graphe simulé du réseau ferroviaire.
    */
   int V () const
   {
      return tn.cities.size();
   };

   /**
    * @ Applique la fonction f à chaque sommet du graphe.
    * @param f Fonction prenant un int en paramètre.
    */
   template<typename Func>
   void forEachVertex (Func f) const
   {
      for (const std::pair<std::string, int> &couple : tn.cityIdx)
      {
         f(couple.second);
      }
   }
};


class TrainGraphWrapper : public TrainGraphWrapperCommon
{
   typedef EdgeWeighted<double> Edge;


public:

   TrainGraphWrapper (const TrainNetwork &tn, const std::function<double (const TrainNetwork::Line &)> &costFunction)
           : TrainGraphWrapperCommon(tn, costFunction)
   {}

   /**
    * @brief Applique la fonction f à chaque EdgeWeighted<double> Edge dérivé du réseau ferroviaire.
    * @param f Fonction à appliquer.
    */
   template<typename Func>
   void forEachEdge (Func f) const
   {
      for(const TrainNetwork::Line& line : tn.lines)
      {
         f(Edge(line.cities.first, line.cities.second, costFunction(line)));
      }
   };

   /**
    * @brief Applique la fonction f à tous les EdgeWeighted<double> adjacent de v.
    * @param v Sommet à traîter les arrêtes adjacentes.
    * @param f Fonction à appliquer.
    */
   template<typename Func>
   void forEachAdjacentEdge (int v, Func f) const
   {
      for(size_t i : tn.cities[v].lines)
      {
         f(Edge(tn.lines[i].cities.first, tn.lines[i].cities.second, costFunction(tn.lines[i])));
      }
   }
};

class TrainGraphWrapperDirected : public TrainGraphWrapperCommon
{

   typedef EdgeWeightedDirected<double> Edge;

public:

   TrainGraphWrapperDirected (const TrainNetwork &tn,
                              const std::function<double (const TrainNetwork::Line &)> &costFunction)
           : TrainGraphWrapperCommon(tn, costFunction)
   {}

   /**
    * @brief Applique la fonction f à chaque EdgeWeightedDirected<double> Edge
    *        dérivé du réseau ferroviaire.
    * @param f Fonction à appliquer.
    */
   template<typename Func>
   void forEachEdge (Func f) const
   {
      for(const TrainNetwork::Line& line : tn.lines)
      {
         f(Edge(line.cities.first , line.cities.second, costFunction(line)));
         f(Edge(line.cities.second, line.cities.first, costFunction(line)));
      }
   };

   /**
    * @brief Applique la fonction f à tous les EdgeWeightedDirected<double> connecté à v.
    * @param v Sommet à traîter les arrêtes connecté (entrantes et sortantes).
    * @param f Fonction à appliquer.
    */
   template<typename Func>
   void forEachAdjacentEdge (int v, Func f) const
   {
      for(int i : tn.cities[v].lines)
      {
         f(Edge(tn.lines[i].cities.first, tn.lines[i].cities.second, costFunction(tn.lines[i])));
         f(Edge(tn.lines[i].cities.second, tn.lines[i].cities.first, costFunction(tn.lines[i])));
      }
   }
};

#endif
