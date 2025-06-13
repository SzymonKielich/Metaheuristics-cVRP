//
// Created by szymo on 06/10/2024.
//

#ifndef METAHEURYSTYKI_CVRP_CONFIG_H
#define METAHEURYSTYKI_CVRP_CONFIG_H


#include <random>

class Config {
public:
    Config(int popSize, int gens, double cProb, double mProb, int tSize,
           const std::string& crossOp, const std::string& mutOp)
            : populationSize(popSize), generations(gens), crossProb(cProb),
              mutProb(mProb), tournamentSize(tSize),
              crossoverOperator(crossOp), mutationOperator(mutOp) {
        seed = std::random_device{}();
    }
    int getPopulationSize() const;

    int getGenerations() const;

    double getCrossProb() const;

    double getMutProb() const;

    int getTournamentSize() const;

    const std::string &getCrossoverOperator() const;

    const std::string &getMutationOperator() const;

    void printConfig();


    unsigned int seed;
private:
    int generations;
    double crossProb;
    int populationSize;
    std::string crossoverOperator;
    std::string mutationOperator;
    double mutProb;
    int tournamentSize;


};


#endif //METAHEURYSTYKI_CVRP_CONFIG_H
