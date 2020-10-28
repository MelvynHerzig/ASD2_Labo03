/*
 * File:   TrainGraphWrapper.h
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 16:24
 * Updated on 16. aout 2019 by Antoine Rochat
 */

#ifndef ASD2_TrainGraphWrapper_h
#define ASD2_TrainGraphWrapper_h

#include <functional>
#include "TrainNetwork.h"
#include "EdgeWeighted.h"
#include "EdgeWeightedDirected.h"

class TrainGraphWrapperCommon {
protected:
    const TrainNetwork& tn;

    // Fonction de cout
    // Correspond typiquement à une expression lambda qui prend une ligne du TrainNetwork en paramètre et retourne un double qui
    // correspond au cout de cette ligne
    const std::function<double(const TrainNetwork::Line&)>& costFunction;

    TrainGraphWrapperCommon(const TrainNetwork& tn, const std::function<double(const TrainNetwork::Line&)>& costFunction)
            : tn(tn), costFunction(costFunction) {}

};


class TrainGraphWrapper : public TrainGraphWrapperCommon {
    typedef EdgeWeighted<double> Edge;


public:

    TrainGraphWrapper(const TrainNetwork& tn, const std::function<double(const TrainNetwork::Line&)>& costFunction)
            : TrainGraphWrapperCommon(tn, costFunction) {}

/****
*
*  A IMPLEMENTER
*
****/
};

class TrainGraphWrapperDirected : public TrainGraphWrapperCommon {

    typedef EdgeWeightedDirected<double> Edge;

public:

    TrainGraphWrapperDirected(const TrainNetwork& tn, const std::function<double(const TrainNetwork::Line&)>& costFunction)
            : TrainGraphWrapperCommon(tn, costFunction) {}

/****
*
*  A IMPLEMENTER
*
****/
};

#endif
