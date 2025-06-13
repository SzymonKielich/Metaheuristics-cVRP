//
// Created by szymo on 06/10/2024.
//

#ifndef METAHEURYSTYKI_CVRP_INDIVIDUAL_H
#define METAHEURYSTYKI_CVRP_INDIVIDUAL_H


#include <memory>
#include "Problem.h"
#include "Config.h"

class Individual {
public:
    Individual() {
        fitness = -1;
    }

    Individual(const std::vector<int>& rt, int fit): route(rt), fitness(fit) {}

    Individual(std::vector<int>&& rt, int fit): route(std::move(rt)), fitness(fit) {}

    void generateRandomRoute(const Problem& problem);
    std::vector<int> getRouteCopy() {return route;}

    bool operator<(const Individual& other) const {
        return fitness < other.fitness;
    }
    bool operator==(const Individual& other) const {
        if(fitness != other.fitness) return false;
        return route == other.route;
    }

    std::string toString();

    // ---------- GETTERS & SETTERS ----------------
    void setFitness(int value) {
        fitness = value;
    }
    int getFitness() {return fitness;}
    void setRoute(const std::vector<int>& route) {
        this->route = route;
    }
    std::vector<int> &getRoute();
    void setRoute(std::vector<int> &&route) {
        this->route = std::move(route);
    }

    int fitness;
    bool isValidIndividual(int numCities) const;
    std::vector<int> route;
//    size_t getHashCode() const;

private:


};


#endif //METAHEURYSTYKI_CVRP_INDIVIDUAL_H
