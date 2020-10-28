/* 
 * File:   EdgeWeighted.h
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 16:24
 * Updated on 8. sept 2020 by Antoine Rochat
 */

#ifndef ASD2_EdgeWeighted_h
#define ASD2_EdgeWeighted_h

#include "EdgeWeightedCommon.h"

/**
 * Classe definissant les aretes ponderes (non orientees) derive de la class EdgeWeightedCommon
 */

template< typename T> // Type du poids, par exemple int ou double
// T doit etre comparable, et être un parametre
// valide pour std::numeric_limits
struct EdgeWeighted : public EdgeWeightedCommon<T> {
    // defini la class mere comme BASE.
    typedef EdgeWeightedCommon<T> BASE;

public:
    // Constructeur par defaut. Appelle BASE() par defaut
    EdgeWeighted() { }

    // Constructeur spécifiant les deux sommets et le poids
    EdgeWeighted(int v, int w, T _weight) : BASE(v,w,_weight) { }

    // Renvoie un des deux sommets
    int Either() const { return this->v1; }

    // Renvoie l'autre sommet
    int Other(int v) const { return (v==this->v1) ? this->v2 : this->v1; }

    // Affiche "v1-v2 (weight)"
    friend std::ostream& operator << (std::ostream& s, const EdgeWeighted<T>& e) {
        return s << e.Either() << "-" << e.Other(e.Either()) << " (" << e.Weight() << ")";
    }
};

#endif
