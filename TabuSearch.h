//
// Created by szymo on 20/10/2024.
//

#ifndef METAHEURYSTYKI_CVRP_TABUSEARCH_H
#define METAHEURYSTYKI_CVRP_TABUSEARCH_H

#include "Logger.h"
#include "Problem.h"
#include "Individual.h"
#include "TabuConfig.h"
#include <vector>
#include <random>
#include <unordered_set>
#include <deque>
#include <list>

class TabuSearch {
public:

    TabuSearch(const TabuConfig &cfg, const Problem &prob, const std::string &logFilePath);

    Individual run();
    std::vector<Individual> generateNeighbors(const Individual& solution);
    Individual inverseOperator(const Individual &current, std::mt19937 &rng);
    Individual swapOneOperator(const Individual& current, std::mt19937& rng);

private:
    TabuConfig config;
    Problem problem;
    Individual bestCandidate;
    Individual bestSolution;
    std::mt19937 rng;
    Logger logger;


    std::list<Individual> tabu;


    void evaluatePopulation(std::vector<Individual> &neighbors);


    std::vector<double> getFitnesses(std::vector<Individual> &neighbors) const;

    std::vector<Individual> generateNeighbors(Individual &currentInd);

    Individual swapOperator(const Individual &current, std::mt19937 &rng);

};



#endif //METAHEURYSTYKI_CVRP_TABUSEARCH_H
