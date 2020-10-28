/* 
 * File:   EdgeWeightedCommon.h
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 16:24
 * Updated on 8. sept 2020 by Antoine Rochat
 */

#ifndef ASD2_EdgeWeightedCommon_h
#define ASD2_EdgeWeightedCommon_h

#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <limits>
#include <fstream>

//  Classe regroupant toutes les parties communes de EdgeWeighted et EdgeWeightedDirected.


template< typename T> // Type du poids, par exemple int ou double
// T doit etre comparable, et être un parametre
// valide pour std::numeric_limits
struct EdgeWeightedCommon {
public:
    // Type de donnée pour les poids.
    typedef T WeightType;

protected:
    // v1 et v2 sont les deux sommets de l'arc/arête
    int v1,v2;

    // weight en est le poids
    WeightType weight;

public:
    // Constructeur par défaut. Le poids est infini
    EdgeWeightedCommon() : v1(-1), v2(-1), weight(std::numeric_limits<T>::max()) { }

    // Constructeur spécifiant les deux sommets et le poids
    EdgeWeightedCommon(int v, int w, WeightType _weight) : v1(v), v2(w), weight(_weight) { }

    // renvoie la valeur du poids
    WeightType Weight() const { return weight; }

    // opérations de comparaison entre edges en fonction de leur poids.
    inline bool operator< (const EdgeWeightedCommon<T>& rhs) const { return weight < rhs.Weight(); }
    inline bool operator> (const EdgeWeightedCommon<T>& rhs) const {return rhs < *this;}
    inline bool operator<=(const EdgeWeightedCommon<T>& rhs) const {return !(*this > rhs);}
    inline bool operator>=(const EdgeWeightedCommon<T>& rhs) const {return !(*this < rhs);}
};

#endif
