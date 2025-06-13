//
// Created by szymo on 01/11/2024.
//

#ifndef METAHEURYSTYKI_CVRP_SIMULATEDANNEALING_H
#define METAHEURYSTYKI_CVRP_SIMULATEDANNEALING_H

#include <random>
#include <unordered_map>
#include <iostream>
#include "Individual.h"
#include "Problem.h"
#include "Logger.h"
#include "SAconfig.h"


class SimulatedAnnealing {
public:
    SimulatedAnnealing(const SAConfig& cfg, const Problem& prob, const std::string& logFilePath);

    Individual run();

private:
    void evaluatePopulation(std::vector<Individual>& neighbors);
    std::vector<double> getFitnesses(std::vector<Individual>& neighbors) const;
    std::vector<Individual> generateNeighbors(Individual& currentInd);
    Individual swapOneOperator(const Individual& current, std::mt19937& rng);
    Individual swapOperator(const Individual& current, std::mt19937& rng);
    Individual inverseOperator(const Individual& current, std::mt19937& rng);
    void updateTemperature();
    double acceptanceProbability(double currentFitness, double newFitness, double temperature);

    SAConfig config;
    Problem problem;
    std::mt19937 rng;
    Logger logger;

    Individual bestCandidate;
    Individual bestSolution;
    double currentTemperature;
};

#endif //METAHEURYSTYKI_CVRP_SIMULATEDANNEALING_H