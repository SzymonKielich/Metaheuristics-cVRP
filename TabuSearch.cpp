//
// Created by szymo on 20/10/2024.
//
#include <chrono>
#include "TabuConfig.h"
#include "TabuSearch.h"
#include "Operators.h"

TabuSearch::TabuSearch(const TabuConfig &cfg, const Problem &prob, const std::string &logFilePath)
        : config(cfg), problem(prob), rng(cfg.seed), logger(logFilePath) {}

Individual TabuSearch::run() {
    rng = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());

    logger.log("generation,best,current,avg,worst\n");
    if (config.initializationMethod == "random") {
        bestCandidate.generateRandomRoute(problem);
    } else {
        bestCandidate.setRoute(problem.getBestGreedy());
    }

    bestCandidate.setFitness(problem.eval(bestCandidate.getRoute()));
    bestSolution = bestCandidate;
    tabu.push_back(bestCandidate);

    for (int i = 0; i <= config.getIterations(); i++) {
        std::vector<Individual> neighbors = generateNeighbors(bestCandidate);

        for (const auto& neighbor : neighbors) {
            if (std::find(tabu.begin(), tabu.end(), neighbor) == tabu.end()) {
                bestCandidate = neighbor;
                break;
            }
        }
//        bestCandidate.setFitness(INT_MAX);
        for (auto& neighbor : neighbors) {
            if (std::find(tabu.begin(), tabu.end(), neighbor) == tabu.end()) {
                if (neighbor.getFitness() < bestCandidate.getFitness()) {
                    bestCandidate = neighbor;
                }
            }
        }
//        if(bestCandidate.getFitness() == INT_MAX) {
//            logger.logTabu(i, bestSolution.getFitness(), problem.eval(bestCandidate.getRoute()), getFitnesses(neighbors));
//            continue;
//        }
        if (bestCandidate.getFitness() < bestSolution.getFitness()) {
            bestSolution = bestCandidate;
        }


        logger.logTabu(i, bestSolution.getFitness(), bestCandidate.getFitness(), getFitnesses(neighbors));
        if (std::find(tabu.begin(), tabu.end(), bestCandidate) == tabu.end()) {
            tabu.push_back(bestCandidate);
            if (tabu.size() > config.getTabuSize()) {
                tabu.pop_front();
            }
        }
    }
//    std::cout << bestSolution.getFitness() << std::endl;

    return bestSolution;
}

void TabuSearch::evaluatePopulation(std::vector<Individual> &neighbors) {
    for (auto &ind: neighbors) {
        ind.setFitness(problem.eval(ind.getRoute()));
    }
}

std::vector<double> TabuSearch::getFitnesses(std::vector<Individual> &neighbors) const {
    std::vector<double> fitnesses;
    for (const auto &ind: neighbors) {
        fitnesses.push_back(ind.fitness);
    }
    return fitnesses;
}

std::vector<Individual> TabuSearch::generateNeighbors(Individual &currentInd) {

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

Individual TabuSearch::swapOneOperator(const Individual &current, std::mt19937 &rng) {
    Individual neighbor = current;
    int size = neighbor.getRoute().size();
    std::uniform_int_distribution<> dist(0, size - 1);
    int i = dist(rng);
    int j = dist(rng);
    std::swap(neighbor.getRoute()[i], neighbor.getRoute()[j]);


    return neighbor;
}

Individual TabuSearch::swapOperator(const Individual &current, std::mt19937 &rng) {
    Individual neighbor = current;
    int size = neighbor.getRoute().size();
    std::uniform_int_distribution<> dist(0, size - 1);
    std::uniform_real_distribution<> distProb(0.0, 1.0);
    for (int i = 0; i < size; i++) {
        if (distProb(rng) < config.getMutProb()) {
            int j = dist(rng);
            std::swap(neighbor.getRoute()[i], neighbor.getRoute()[j]);
        }

    }
    return neighbor;
}

Individual TabuSearch::inverseOperator(const Individual &current, std::mt19937 &rng) {
    Individual neighbor = current;
    int size = neighbor.getRoute().size();
    std::uniform_int_distribution<> dist(0, size - 1);
    int i = dist(rng);
    int j = dist(rng);
    if (i > j) std::swap(i, j);
    std::reverse(neighbor.getRoute().begin() + i, neighbor.getRoute().begin() + j + 1);
    return neighbor;
}


