//
// Created by szymo on 06/10/2024.
//

#include <iostream>
#include "Config.h"

void Config::printConfig() {
    std::cout<<"\nPopsize: "<<populationSize<<", generations: "<<generations<<", crossover"<<crossoverOperator<<" with prob: "<<crossProb<<", mutation "<<mutationOperator<<" with prob: "<<mutProb<<", tournament: "<<tournamentSize<<std::endl;

}
int Config::getPopulationSize() const {
    return populationSize;
}

int Config::getGenerations() const {
    return generations;
}

double Config::getCrossProb() const {
    return crossProb;
}

double Config::getMutProb() const {
    return mutProb;
}

int Config::getTournamentSize() const {
    return tournamentSize;
}

const std::string &Config::getCrossoverOperator() const {
    return crossoverOperator;
}

const std::string &Config::getMutationOperator() const {
    return mutationOperator;
}
