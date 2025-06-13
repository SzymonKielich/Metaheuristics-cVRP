//
// Created by szymo on 06/10/2024.
//

#ifndef METAHEURYSTYKI_CVRP_LOGGER_H
#define METAHEURYSTYKI_CVRP_LOGGER_H


#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <tuple> // Include this header for std::get

class Logger {
public:
    Logger(const std::string& filename) : file(filename) {
    }
    Logger(const std::string& filename, std::string columns) : file(filename) {
        file << columns << "\n";
    }
    void log(int gen, int fitness) {
        file << gen << "," << fitness << "\n";
    }
    void log(std::string str) {
        file << str;
    }
    void logStats(std::tuple<int, int, double, double> stats) {
        file << std::get<0>(stats) << "," << std::get<1>(stats) << "," << std::get<2>(stats) << "," << std::get<3>(stats) << "\n";
    }

    void logGeneration(int generation, const std::vector<double>& fitnesses);

    void logTabu(int generation, int bestFitness, int currentFitness, const std::vector<double> &fitnesses);
    void logSA(int generation, int bestFitness, int currentFitness) {
        file << generation << "," << bestFitness << "," << currentFitness << "\n";
    }

private:
    std::ofstream file;

    void logIteration(int iteration, double best, double current, double average, double worst);
};


#endif //METAHEURYSTYKI_CVRP_LOGGER_H
