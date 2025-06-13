//
// Created by szymo on 20/10/2024.
//

#ifndef METAHEURYSTYKI_CVRP_TABUCONFIG_H
#define METAHEURYSTYKI_CVRP_TABUCONFIG_H


#include <string>
#include <random>

class TabuConfig {


public:
    TabuConfig() {
        seed = std::random_device{}();
    }
    TabuConfig(int iterations, int tabuListSize, int neighborCount, const std::string &initializationMethod,
               const std::string &neighborhoodOperator, double mutProb): seed(seed), iterations(iterations),tabuSize(tabuListSize),nSize(neighborCount),
               initializationMethod(initializationMethod), neighborOperator(neighborhoodOperator), mutProb(mutProb) {seed = std::random_device{}();}

    unsigned int seed;

    std::string initializationMethod;
    int getIterations() {
        return iterations;
    }

    int getTabuSize() {
        return tabuSize;
    }
    int getnSize() {
        return nSize;
    }
    std::string neighborOperator;
    double getMutProb() {
        return mutProb;
    }
private:
    int iterations;
    int tabuSize;
    int nSize;
    double mutProb;
};


#endif //METAHEURYSTYKI_CVRP_TABUCONFIG_H
