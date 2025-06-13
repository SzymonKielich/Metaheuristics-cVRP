//
// Created by szymo on 06/10/2024.
//

#include "Logger.h"

void Logger::logIteration(int iteration, double best, double current, double average, double worst) {
    file << iteration << "," << best << "," << current << "," << average << "," << worst << "\n";

}

void Logger::logGeneration(int generation, const std::vector<double> &fitnesses) {
    double best = *std::min_element(fitnesses.begin(), fitnesses.end());
    double worst = *std::max_element(fitnesses.begin(), fitnesses.end());
    double avg = std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0) / fitnesses.size();
    double std_dev = std::sqrt(std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0,
                                               [avg](double sum, double val) { return sum + (val - avg) * (val - avg); }) / fitnesses.size());

//    file << generation << "," << best << "," << avg << "," << worst << "," << std_dev << "\n";
    file << generation << "," << best << "," << avg << "," << worst <<"\n";

}

void Logger::logTabu(int generation, int bestFitness, int currentFitness, const std::vector<double> &fitnesses) {

    double worst = *std::max_element(fitnesses.begin(), fitnesses.end());
    double avg = std::accumulate(fitnesses.begin(), fitnesses.end(), 0.0) / fitnesses.size();

    file << generation << "," << bestFitness << "," << currentFitness << "," << avg << "," << worst << "\n";
}
