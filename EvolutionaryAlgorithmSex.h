//
// Created by szymo on 28/11/2024.
//

#ifndef METAHEURYSTYKI_CVRP_EVOLUTIONARYALGORITHMSEX_H
#define METAHEURYSTYKI_CVRP_EVOLUTIONARYALGORITHMSEX_H

#include <vector>
#include <random>
#include <string>
#include "Config.h"
#include "Problem.h"
#include "Individual.h"
#include "Logger.h"

class EvolutionaryAlgorithmSex {
public:
    EvolutionaryAlgorithmSex(const Config& cfg, const Problem& prob, const std::string& logFilePath);

    Individual run();

private:
    Config config;
    Problem problem;
    std::vector<Individual> malePopulation;
    std::vector<Individual> femalePopulation;
    std::mt19937 rng;
    Logger logger;

    void initializePopulation();
    void evaluatePopulation();
    Individual selectParent(const std::vector<Individual>& population);
    std::pair<Individual, Individual> applyCrossover(const Individual& maleParent, const Individual& femaleParent);
    void applyMutation(Individual& ind);
    std::vector<double> getFitnesses() const;
};




#endif //METAHEURYSTYKI_CVRP_EVOLUTIONARYALGORITHMSEX_H
