//
// Created by szymo on 28/11/2024.
//

#include <random>
#include "IndividualSex.h"
#include <random>
#include <iostream>
#include <unordered_set>
#include <vector>


std::string IndividualSex::toString() {
    std::string result = "";
    result.append("fitness: " + std::to_string(fitness) + ": ");
    for(auto & city: route) {
        result.append(std::to_string(city+1) + " ");
    }
    std::cout<<result;
    return result.append("\n");
}
bool IndividualSex::isValidIndividualSex(int numCities) const {
    if (route.size() != numCities - 1) return false;
    std::unordered_set<int> citySet(route.begin(), route.end());
    bool isvalid = citySet.size() == route.size() && *std::min_element(route.begin(), route.end()) >= 1
                   && *std::max_element(route.begin(), route.end()) <= numCities - 1;
    return isvalid;
}
void IndividualSex::generateRandomRoute(const Problem &problem) {
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

std::vector<int> &IndividualSex::getRoute() {
    return route;
}
void IndividualSex::setGender(Gender g) {
    gender = g;
}

Gender IndividualSex::getGender() const {
    return gender;
}