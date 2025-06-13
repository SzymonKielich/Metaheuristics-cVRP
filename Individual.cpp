//
// Created by szymo on 06/10/2024.
//

#include <random>
#include <iostream>
#include "Individual.h"
#include <unordered_set>
#include <algorithm>
#include <vector>


std::string Individual::toString() {
    std::string result = "";
    result.append("fitness: " + std::to_string(fitness) + ": ");
    for(auto & city: route) {
        result.append(std::to_string(city+1) + " ");
    }
    std::cout<<result;
    return result.append("\n");
}
bool Individual::isValidIndividual(int numCities) const {
    if (route.size() != numCities - 1) return false;
    std::unordered_set<int> citySet(route.begin(), route.end());
    bool isvalid = citySet.size() == route.size() && *std::min_element(route.begin(), route.end()) >= 1
                   && *std::max_element(route.begin(), route.end()) <= numCities - 1;
    return isvalid;
}
void Individual::generateRandomRoute(const Problem &problem) {
    int size = problem.getDimension() - 1; //bez depot
    route.resize(size);
    int idx = 0;
    for(int i=0; i<size+1; i++) {
        if(i != problem.getDepot()) {
            route[idx] = i;
            idx++;
        }
    }
    std::random_device rd;
    std::shuffle(route.begin(), route.end(), rd);
    fitness = problem.eval(route);
}

std::vector<int> &Individual::getRoute() {
    return route;
}
//size_t Individual::getHashCode() const {
//    std::hash<int> hasher;
//    size_t seed = route.size();
//    for (int i : route) {
//        seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
//    }
//    return seed;
//}


