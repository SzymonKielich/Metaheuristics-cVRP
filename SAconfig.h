//
// Created by szymo on 01/11/2024.
//

#ifndef METAHEURYSTYKI_CVRP_SACONFIG_H
#define METAHEURYSTYKI_CVRP_SACONFIG_H

#include <string>
#include <random>

class SAConfig {
public:
    SAConfig() {
        seed = std::random_device{}();
    }
    SAConfig(int iterations, double initialTemperature, double finalTemperature, double coolingSchedule, int neighborCount, const std::string& initializationMethod,
             const std::string& neighborhoodOperator, double mutProb): seed(seed), iterations(iterations), initialTemperature(initialTemperature), finalTemperature(finalTemperature), coolingSchedule(coolingSchedule), nSize(neighborCount),
                                                                       initializationMethod(initializationMethod), neighborOperator(neighborhoodOperator), mutProb(mutProb) {seed = std::random_device{}();}

    unsigned int seed;

    std::string initializationMethod;
    int getIterations() {
        return iterations;
    }

    double getInitialTemperature() {
        return initialTemperature;
    }
    double getCoolingSchedule() {
        return coolingSchedule;
    }
    int getnSize() {
        return nSize;
    }
    std::string neighborOperator;
    double coolingSchedule;

    double getMutProb() {
        return mutProb;
    }

    double getFinalTemperature() const {
        return finalTemperature;
    }

    void setFinalTemperature(double finalTemperature) {
        finalTemperature = finalTemperature;
    }


private:
    int iterations;
    double initialTemperature;
    double finalTemperature;
    int nSize;
    double mutProb;

};

#endif //METAHEURYSTYKI_CVRP_SACONFIG_H