//
// Created by szymo on 06/10/2024.
//

#ifndef METAHEURYSTYKI_CVRP_EVOLUTIONARYALGORITHM_H
#define METAHEURYSTYKI_CVRP_EVOLUTIONARYALGORITHM_H


#include <vector>
#include <sstream>
#include "Individual.h"
#include "Logger.h"

class EvolutionaryAlgorithm {
public:
    EvolutionaryAlgorithm(const Config& cfg, const Problem& prob): config(cfg), problem(prob), rng(cfg.seed) {}
    Individual run();
    void applyMutation(Individual &ind);

    Individual applyCrossover(const Individual &parent1, const Individual &parent2);
    void printPopulation();
private:
    Config config;
    Problem problem;
    std::vector<Individual> population;
    std::mt19937 rng;
    std::string getFilePath() {
        std::ostringstream filePath;
        filePath << "C:\\Users\\szymo\\CLionProjects\\Metaheurystyki_cVRP\\summaries\\ga_progress.csv";
        return filePath.str();
    }
    Logger logger{getFilePath()};

//    void evolve();
    void evaluatePopulation();

    void initializePopulation();

    Individual selectParent();

    std::vector<double> getFitnesses() const;


};


#endif //METAHEURYSTYKI_CVRP_EVOLUTIONARYALGORITHM_H
