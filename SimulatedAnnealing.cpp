//
// Created by szymo on 01/11/2024.
//
#include <chrono>
#include "SAConfig.h"
#include "SimulatedAnnealing.h"
#include "Operators.h"

SimulatedAnnealing::SimulatedAnnealing(const SAConfig& cfg, const Problem& prob, const std::string& logFilePath)
        : config(cfg), problem(prob), rng(cfg.seed), logger(logFilePath) {}


Individual SimulatedAnnealing::run() {

    rng = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> dist(0, 1);

    logger.log("generation,best,current\n");
    if (config.initializationMethod == "random") {
        bestCandidate.generateRandomRoute(problem);
    } else {
        bestCandidate.setRoute(problem.getBestGreedy());
    }

    bestCandidate.setFitness(problem.eval(bestCandidate.getRoute()));
    bestSolution = bestCandidate;
    currentTemperature = config.getInitialTemperature();

    for (int i = 1; i <= config.getIterations(); ++i) {
        std::vector<Individual> neighbors = generateNeighbors(bestCandidate);


        for (auto &neighbor: neighbors) {
            if (neighbor.getFitness() <= bestCandidate.getFitness()) {
                bestCandidate = neighbor;
            } else {
                double probability = acceptanceProbability(bestCandidate.getFitness(), neighbor.getFitness(), currentTemperature);
                if (probability > dist(rng)) {
                    bestCandidate = neighbor;
                }
            }
        }
        if (bestCandidate.getFitness() < bestSolution.getFitness()) {
            bestSolution = bestCandidate;
        }


        logger.logSA(i, bestSolution.getFitness(), bestCandidate.getFitness());
        updateTemperature();
    }
//    std::cout << std::endl << bestSolution.getFitness() << std::endl;
    return bestSolution;
}

void SimulatedAnnealing::evaluatePopulation(std::vector<Individual>& neighbors) {
    for (auto& ind : neighbors) {
        ind.setFitness(problem.eval(ind.getRoute()));
    }
}

std::vector<double> SimulatedAnnealing::getFitnesses(std::vector<Individual>& neighbors) const {
    std::vector<double> fitnesses;
    for (const auto& ind : neighbors) {
        fitnesses.push_back(ind.fitness);
    }
    return fitnesses;
}

std::vector<Individual> SimulatedAnnealing::generateNeighbors(Individual& currentInd) {
    std::vector<Individual> neighbors;
    Individual neighbor;
    neighbor = currentInd;

    for (int i = 0; i < config.getnSize(); ++i) {
        neighbor = currentInd;

        if (config.neighborOperator == "swap") {
            neighbor = swapOneOperator(neighbor, rng);
        } else {
            neighbor = inverseOperator(neighbor, rng);
        }

        neighbors.push_back(std::move(neighbor));
    }
    evaluatePopulation(neighbors);
    return neighbors;
}

Individual SimulatedAnnealing::swapOneOperator(const Individual& current, std::mt19937& rng) {
    Individual neighbor = current;
    int size = neighbor.getRoute().size();
    std::uniform_int_distribution<> dist(0, size - 1);
    int i= dist(rng);
    int j = dist(rng);
    std::swap(neighbor.getRoute()[i], neighbor.getRoute()[j]);
    return neighbor;
}

Individual SimulatedAnnealing::swapOperator(const Individual& current, std::mt19937& rng) {
    Individual neighbor = current;
    int size = neighbor.getRoute().size();
    std::uniform_int_distribution<> dist(0, size - 1);
    std::uniform_real_distribution<> distProb(0.0, 1.0);
    for(int i=0; i<size; i++) {
        if(distProb(rng) < config.getMutProb()) {
            int j = dist(rng);
            std::swap(neighbor.getRoute()[i], neighbor.getRoute()[j]);
        }
    }
    return neighbor;
}

Individual SimulatedAnnealing::inverseOperator(const Individual& current, std::mt19937& rng) {
    Individual neighbor = current;
    int size = neighbor.getRoute().size();
    std::uniform_int_distribution<> dist(0, size - 1);
    int i = dist(rng);
    int j = dist(rng);
    if (i > j) std::swap(i, j);
    std::reverse(neighbor.getRoute().begin() + i, neighbor.getRoute().begin() + j + 1);
    return neighbor;
}

void SimulatedAnnealing::updateTemperature() {
    if(currentTemperature > config.getFinalTemperature()) {
        currentTemperature *= config.getCoolingSchedule();
    }

}

double SimulatedAnnealing::acceptanceProbability(double currentFitness, double newFitness, double temperature) {
    if (newFitness < currentFitness) {
        return 1.0;
    }
    return std::exp( (currentFitness - newFitness) / temperature);
}
