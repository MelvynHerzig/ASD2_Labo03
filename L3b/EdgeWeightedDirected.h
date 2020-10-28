/* 
 * File:   EdgeWeightedDirected.h
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 16:24
 * Updated on 8. sept 2020 by Antoine Rochat
 */

#ifndef ASD2_EdgeWeightedDirected_h
#define ASD2_EdgeWeightedDirected_h

#include "EdgeWeightedCommon.h"

/**
 * Classe definissant les arcs ponderes (orientes) derive de la class EdgeWeightedCommon
 */
template< typename T> // Type du poids, par exemple int ou double
// T doit etre comparable, et être un parametre
// valide pour std::numeric_limits
struct EdgeWeightedDirected  : public EdgeWeightedCommon<T> {
    // defini la class mere comme BASE.
    typedef EdgeWeightedCommon<T> BASE;

public:
    // Constructeur par defaut. Appelle BASE() par defaut
    EdgeWeightedDirected() { }

    // Constructeur spécifiant les deux sommets et le poids
    EdgeWeightedDirected(int v, int w, T _weight) : BASE(v,w,_weight) { }

    // Sommet de départ de l'arc
    int From() const { return this->v1; }

    // Sommet d'arrivée de l'arc
    int To() const { return this->v2; }

    // Affiche "v1->v2 (weight)"
    friend std::ostream& operator << (std::ostream& s, const EdgeWeightedDirected<T>& e) {
        return s << e.From() << "->" << e.To() << " (" << e.Weight() << ")";
    }
};

#endif
