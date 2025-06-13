//
// Created by szymo on 26/11/2024.
//

#include "EvolutionaryAlgorithmRoulette.h"
#include "Operators.h"
#include <algorithm>
#include <chrono>
#include <numeric>
#include <iostream>

EvolutionaryAlgorithmRoulette::EvolutionaryAlgorithmRoulette(const Config &cfg, const Problem &prob,
                                                             const std::string &logFilePath,
                                                             bool accumulateScores, bool logOperatorUsage,
                                                             bool useAverageImprovement, int resetInterval)
        : config(cfg), problem(prob), rng(std::random_device{}()), logger("../list5/a" + logFilePath),
          accumulateScores(accumulateScores), logOperatorUsage(logOperatorUsage),
          useAverageImprovement(useAverageImprovement), resetInterval(resetInterval),
          operatorLogger("../list5/op" + logFilePath), generationsSinceReset(0) {

    crossoverOperators = {"OX", "PMX", "PositionBased"};
    initialCrossProb = 1.0 / crossoverOperators.size();
    for (const auto &op: crossoverOperators) {
        crossoverOperatorProbabilities[op] = initialCrossProb;
        crossoverOperatorScores[op] = 0.0;
        crossoverOperatorUsage[op] = 0;
        crossoverOperatorPerformance[op] = 0.0;
    }

    mutationOperators = {"swap", "inverse", "scramble", "insertion"};
    initialMutProb = 1.0 / mutationOperators.size();
    for (const auto &op: mutationOperators) {
        mutationOperatorProbabilities[op] = initialMutProb;
        mutationOperatorScores[op] = 0.0;
        mutationOperatorUsage[op] = 0;
        mutationOperatorPerformance[op] = 0.0;
    }

    if (logOperatorUsage) {
        operatorLogger.log("generation,");
        for (const auto &op: crossoverOperators) {
            operatorLogger.log("cross_" + op + ",");
        }
        for (const auto &op: mutationOperators) {
            operatorLogger.log("mut_" + op + ",");
        }
        operatorLogger.log("\n");
    }
}

Individual EvolutionaryAlgorithmRoulette::run() {
    initializePopulation();
    evaluatePopulation();
    logger.log("generation,best,avg,worst\n");

    logger.logGeneration(0, getFitnesses());
    Individual bestSolution = *std::min_element(population.begin(), population.end());

    for (int t = 1; t <= config.getGenerations(); ++t) {
        std::vector<Individual> newPopulation;

        for (auto &[op, count]: crossoverOperatorUsage) {
            count = 0;
        }
        for (auto &[op, count]: mutationOperatorUsage) {
            count = 0;
        }

        for (auto &[op, perf]: crossoverOperatorPerformance) {
            perf = 0.0;
        }
        for (auto &[op, perf]: mutationOperatorPerformance) {
            perf = 0.0;
        }

        while (newPopulation.size() < population.size()) {
            Individual parent1 = selectParent();
            Individual parent2 = selectParent();

            std::uniform_real_distribution<> distProb(0.0, 1.0);
            std::pair<Individual, Individual> offspring = {parent1, parent2};

            if (distProb(rng) < config.getCrossProb()) {
                std::string crossoverOp = selectCrossoverOperator();
                offspring = applyCrossover(crossoverOp, parent1, parent2);

                offspring.first.setFitness(problem.eval(offspring.first.getRoute()));
                offspring.second.setFitness(problem.eval(offspring.second.getRoute()));

                double improvement1 = parent1.getFitness() - offspring.first.getFitness();
                double improvement2 = parent2.getFitness() - offspring.second.getFitness();
                if (useAverageImprovement) {
                    crossoverOperatorPerformance[crossoverOp] += (
                            (std::max(0.0, improvement1) + std::max(0.0, improvement2)) / 2.0);
                } else {
                    crossoverOperatorPerformance[crossoverOp] +=
                            (improvement1 > 0 ? 1.0 : 0) + (improvement2 > 0 ? 1.0 : 0);
                }

                crossoverOperatorUsage[crossoverOp]++;
            }
            // mutacja offspring1
            if (distProb(rng) < config.getMutProb()) {
                std::string mutationOp = selectMutationOperator();
                double oldFitness = offspring.first.getFitness();
                applyMutation(mutationOp, offspring.first);
                double newFitness = problem.eval(offspring.first.getRoute());
                offspring.first.setFitness(newFitness);
                double improvement = oldFitness - newFitness;

                if (useAverageImprovement) {
                    mutationOperatorPerformance[mutationOp] += std::max(0.0, improvement);
                } else {
                    mutationOperatorPerformance[mutationOp] += (improvement > 0 ? 1.0 : 0);
                }

                mutationOperatorUsage[mutationOp]++;
            }

            // Mutacja dla offspring2
            if (distProb(rng) < config.getMutProb()) {
                std::string mutationOp = selectMutationOperator();
                double oldFitness = offspring.second.getFitness();
                applyMutation(mutationOp, offspring.second);
                double newFitness = problem.eval(offspring.second.getRoute());
                offspring.second.setFitness(newFitness);
                double improvement = oldFitness - newFitness;

                if (useAverageImprovement) {
                    mutationOperatorPerformance[mutationOp] += std::max(0.0, improvement);
                } else {
                    mutationOperatorPerformance[mutationOp] += (improvement > 0 ? 1.0 : 0);
                }

                mutationOperatorUsage[mutationOp]++;
            }

            newPopulation.push_back(offspring.first);
            if (newPopulation.size() < population.size()) {
                newPopulation.push_back(offspring.second);
            }
        }

        population = std::move(newPopulation);

        generationsSinceReset++;


        // przyznawanie punktów i ranking
        if (useAverageImprovement) {
            auto updateScores = [&](std::unordered_map<std::string, double> &operatorPerformance,
                                    std::unordered_map<std::string, double> &operatorScores,
                                    const std::vector<std::string> &operators,
                                    std::unordered_map<std::string, int> &operatorUsage) {
                for (const auto &op: operators) {
                    double sumPerformance = operatorPerformance[op];
                    operatorScores[op] += sumPerformance;
                }
            };

            updateScores(crossoverOperatorPerformance, crossoverOperatorScores, crossoverOperators,
                         crossoverOperatorUsage);
            updateScores(mutationOperatorPerformance, mutationOperatorScores, mutationOperators, mutationOperatorUsage);
        } else {
            auto updateScores = [&](std::unordered_map<std::string, double> &operatorPerformance,
                                    std::unordered_map<std::string, double> &operatorScores,
                                    const std::vector<std::string> &operators) {

                std::vector<std::pair<std::string, double>> performanceVec(operatorPerformance.begin(),
                                                                           operatorPerformance.end());

                // sortowanie operatorow
                std::sort(performanceVec.begin(), performanceVec.end(),
                          [&](const std::pair<std::string, double> &a, const std::pair<std::string, double> &b) {
                              return a.second > b.second;
                          });

                int n = performanceVec.size();
                for (int i = 0; i < n; ++i) {
                    const std::string &op = performanceVec[i].first;
                    if (n >= 4) {
                        if (i == 0) {
                            operatorScores[op] += 2.0;
                        } else if (i == 1) {
                            operatorScores[op] += 1.0;
                        } else if (i == n - 2) {
                            operatorScores[op] += 0.5;
                        } else if (i == n - 1) {
                            operatorScores[op] += 0.0;
                        }
                        else {
                            operatorScores[op] += 1.0;
                        }
                    } else {
                        if (i == 0) {
                            operatorScores[op] += 2.0;
                        } else if (i == 1) {
                            operatorScores[op] += 1.0;
                        } else {
                            operatorScores[op] += 0.0;
                        }
                    }
                }
            };
            updateScores(crossoverOperatorPerformance, crossoverOperatorScores, crossoverOperators);
            updateScores(mutationOperatorPerformance, mutationOperatorScores, mutationOperators);
        }



        updateOperatorProbabilities();

        if (!accumulateScores && resetInterval > 0) {
            if (generationsSinceReset >= resetInterval) {

                for (auto &[op, score]: crossoverOperatorScores) {
                    score = 0.0;
                }
                for (auto &[op, score]: mutationOperatorScores) {
                    score = 0.0;
                }

                for (auto &[op, prob]: crossoverOperatorProbabilities) {
                    prob = initialCrossProb;
                }
                for (auto &[op, prob]: mutationOperatorProbabilities) {
                    prob = initialMutProb;
                }

                generationsSinceReset = 0;
            }
        }
        //najlepsze update
        Individual currentBest = *std::min_element(population.begin(), population.end());
        if (currentBest.getFitness() < bestSolution.getFitness()) {
            bestSolution = currentBest;
        }
        logger.logGeneration(t, getFitnesses());

        if (logOperatorUsage) {
            logOperatorUsageData(t);
        }
    }

    return bestSolution;
}

void EvolutionaryAlgorithmRoulette::logOperatorUsageData(int generation) {
    operatorLogger.log(std::to_string(generation) + ",");
    for (const auto &op: crossoverOperators) {
        operatorLogger.log(std::to_string(crossoverOperatorUsage[op]) + ",");
    }
    for (const auto &op: mutationOperators) {
        operatorLogger.log(std::to_string(mutationOperatorUsage[op]) + ",");
    }
    operatorLogger.log("\n");
}

std::string EvolutionaryAlgorithmRoulette::selectCrossoverOperator() {
    std::uniform_real_distribution<> dist(0.0, 1.0);
    double randValue = dist(rng);
    double cumulativeProb = 0.0;
    for (const auto &op: crossoverOperators) {
        double prob = crossoverOperatorProbabilities[op];
        cumulativeProb += prob;
        if (randValue <= cumulativeProb) {
            return op;
        }
    }
    return crossoverOperators.back();
}

std::string EvolutionaryAlgorithmRoulette::selectMutationOperator() {
    std::uniform_real_distribution<> dist(0.0, 1.0);
    double randValue = dist(rng);
    double cumulativeProb = 0.0;
    for (const auto &op: mutationOperators) {
        double prob = mutationOperatorProbabilities[op];
        cumulativeProb += prob;
        if (randValue <= cumulativeProb) {
            return op;
        }
    }
    return mutationOperators.back();
}

void EvolutionaryAlgorithmRoulette::updateOperatorProbabilities() {
    double totalCrossoverScore = std::accumulate(crossoverOperatorScores.begin(), crossoverOperatorScores.end(), 0.0,
                                                 [](double sum, const auto &pair) { return sum + pair.second; });

    if (totalCrossoverScore > 0.0) {
        for (auto &[op, prob]: crossoverOperatorProbabilities) {
            prob = crossoverOperatorScores[op] / totalCrossoverScore;
        }
    } else {
        for (auto &[op, prob]: crossoverOperatorProbabilities) {
            prob = initialCrossProb;
        }
    }

    double totalMutationScore = std::accumulate(mutationOperatorScores.begin(), mutationOperatorScores.end(), 0.0,
                                                [](double sum, const auto &pair) { return sum + pair.second; });

    if (totalMutationScore > 0.0) {
        for (auto &[op, prob]: mutationOperatorProbabilities) {
            prob = mutationOperatorScores[op] / totalMutationScore;
        }
    } else {
        for (auto &[op, prob]: mutationOperatorProbabilities) {
            prob = initialMutProb;
        }
    }

}

std::pair<Individual, Individual>
EvolutionaryAlgorithmRoulette::applyCrossover(const std::string &crossoverOp, const Individual &parent1,
                                              const Individual &parent2) {
    if (crossoverOp == "OX") {
        return std::pair<Individual, Individual>(Operators::crossOX(parent1, parent2, rng),
                                                 Operators::crossOX(parent2, parent1, rng));
    } else if (crossoverOp == "PMX") {
        return std::pair<Individual, Individual>(Operators::crossPMX(parent1, parent2, rng),
                                                 Operators::crossPMX(parent2, parent1, rng));
    } else if (crossoverOp == "PositionBased") {
        return crossPositionBased(parent1, parent2, rng);
    } else {
        return std::pair<Individual, Individual>(Operators::crossOX(parent1, parent2, rng),
                                                 Operators::crossOX(parent2, parent1, rng));
    }
}

void EvolutionaryAlgorithmRoulette::applyMutation(const std::string &mutationOp, Individual &ind) {
    if (mutationOp == "swap") {
        Operators::mutationSwapOneRandom(ind, rng);
    } else if (mutationOp == "inverse") {
        Operators::mutationInverse(ind, rng);
    } else if (mutationOp == "scramble") {
        mutationScramble(ind, rng);
    } else if (mutationOp == "insertion") {
        mutationInsertion(ind, rng);
    } else {
        Operators::mutationSwapOneRandom(ind, rng);
    }
}

void EvolutionaryAlgorithmRoulette::initializePopulation() {
    population.clear();
    int size = problem.getDimension() - 1;
    std::vector<int> route(size);

    int idx = 0;
    for (int i = 0; i < size + 1; i++) {
        if (i != problem.getDepot()) {
            route[idx] = i;
            idx++;
        }
    }

    for (int i = 0; i < config.getPopulationSize(); ++i) {
        std::shuffle(route.begin(), route.end(), rng);
        population.emplace_back(route, problem.eval(route));
    }
}

void EvolutionaryAlgorithmRoulette::evaluatePopulation() {
    for (auto &ind: population) {
        ind.fitness = problem.eval(ind.route);
    }
}

Individual EvolutionaryAlgorithmRoulette::selectParent() {
    if (config.getTournamentSize() > 0) {
        return Operators::tourSelection(population, config.getTournamentSize(), rng);
    } else {
        return Operators::rouletteScaledSelection(population, rng);
    }
}

std::vector<double> EvolutionaryAlgorithmRoulette::getFitnesses() const {
    std::vector<double> fitnesses;
    for (const auto &ind: population) {
        fitnesses.push_back(ind.fitness);
    }
    return fitnesses;
}
