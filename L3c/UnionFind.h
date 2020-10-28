/*
 * File:   UnionFind.h
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 10:16
 */

#ifndef ASD2_UnionFind_h
#define ASD2_UnionFind_h

#include <vector>

//  Cette classe met en oeuvre de la structure Union-Find, aussi connue
//  sous le nom de disjoint sets. Utilisé par l'algorithme de
//  Kruskal.

class UnionFind {
private:
    // id[i] stocke l'id du parent de i dans l'arbre des classes d'équivalence
    std::vector<int> id;

    // sz[i] stocke la taille de l'arbre dont i est la racine.
    std::vector<int> sz;

public:

    // Constructeur: spécifie le nombre N d'éléments
    UnionFind(int N) {
        id.resize(N);
        for (int i = 0; i < N; ++i)
            id[i] = i;
        sz.assign(N, 1);
    }

    // Find renvoie l'id représentatif de la classe d'équivalence de p.
    int Find(int p) {
        if (id[p] != p)
            id[p] = Find(id[p]);      // compression de chemin par récursion
        return id[p];
    }

    // Connected indique que p et q appartiennent à la même classe d'équivalence
    bool Connected(int p, int q) {
        return Find(p) == Find(q);
    }

    // Union fusionne les classes d'équivalence de p et q
    void Union(int p, int q) {
        int i = Find(p);
        int j = Find(q);
        if (i == j) return;
        if (sz[i] < sz[j]) {     // on attache l'arbre le plus petit à la racine du plus grand
            id[i] = j;
            sz[j] += sz[i];
        } else {
            id[j] = i;
            sz[i] += sz[j];
        }
    }
};

#endif
