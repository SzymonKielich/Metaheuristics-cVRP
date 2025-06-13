//
// Created by szymo on 28/11/2024.
//

#ifndef METAHEURYSTYKI_CVRP_INDIVIDUALSEX_H
#define METAHEURYSTYKI_CVRP_INDIVIDUALSEX_H


#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include <iostream>
#include "Problem.h"

enum class Gender { MALE, FEMALE };

class IndividualSex {
public:
    IndividualSex() {
        fitness = -1;
    }

    IndividualSex(const std::vector<int>& rt, int fit): route(rt), fitness(fit) {}

    IndividualSex(std::vector<int>&& rt, int fit): route(std::move(rt)), fitness(fit) {}

    void generateRandomRoute(const Problem& problem);
    std::vector<int> getRouteCopy() {return route;}

    bool operator<(const IndividualSex& other) const {
        return fitness < other.fitness;
    }
    bool operator==(const IndividualSex& other) const {
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
    Gender gender;
    int fitness;
    bool isValidIndividualSex(int numCities) const;
    std::vector<int> route;
//    size_t getHashCode() const;

    void setGender(Gender g);
    Gender getGender() const;


    struct HashFunction {
        size_t operator()(const IndividualSex& ind) const;
    };


};


#endif //METAHEURYSTYKI_CVRP_INDIVIDUALSEX_H
