/*
 * File:   TrainNetwork.cpp
 * Author: Olivier Cuisenaire
 * Modified: by Antoine Rochat on 16.08.19.
 *
 * Created on 18. novembre 2014, 10:48
 */

#include "TrainNetwork.h"

TrainNetwork::TrainNetwork(const std::string& filename) {
    std::ifstream s(filename);

    int N;
    s >> N;

    cities.resize(N);
    for (size_t i = 0; i < N; ++i) {
        std::string name;
        std::getline(s, name);

        if (!name.empty() && name[name.size() - 1] == '\r')
            name.erase(name.size() - 1);
        if (!name.empty()) {
            cities[i].name = name;
            cityIdx[name] = i;
        } else --i;
    }

    while (s.good()) {
        std::string line;
        std::getline(s, line);

        auto elements = split(line, ';');

        size_t s1 = cityIdx[elements[0]];
        size_t s2 = cityIdx[elements[1]];

        size_t rIdx = lines.size();
        cities[s1].lines.push_back(rIdx);
        cities[s2].lines.push_back(rIdx);
        lines.emplace_back(s1, s2,
                           std::stod(elements[2]),
                           std::stod(elements[3]),
                           std::stoi(elements[4])
        );
    }
    s.close();
}
