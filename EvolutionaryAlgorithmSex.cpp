//
// Created by szymo on 28/11/2024.
//
#include "EvolutionaryAlgorithmSex.h"
#include "Operators.h"
#include <algorithm>
#include <chrono>

EvolutionaryAlgorithmSex::EvolutionaryAlgorithmSex(const Config& cfg, const Problem& prob, const std::string& logFilePath)
        : config(cfg), problem(prob), rng(std::random_device{}()), logger(logFilePath) {}

Individual EvolutionaryAlgorithmSex::run() {
    initializePopulation();
    evaluatePopulation();
//    logger.log("generation,best_fitness\n");
    logger.log("generation,best,avg,worst\n");

    logger.logGeneration(0, getFitnesses());
    Individual bestSolution;
    if (!malePopulation.empty()) {
        bestSolution = *std::min_element(malePopulation.begin(), malePopulation.end());
    }
    if (!femalePopulation.empty()) {
        Individual bestFemale = *std::min_element(femalePopulation.begin(), femalePopulation.end());
        if (bestFemale.getFitness() < bestSolution.getFitness()) {
            bestSolution = bestFemale;
        }
    }

    for (int i = 1; i <= config.getGenerations(); ++i) {
        std::vector<Individual> newMalePopulation;
        std::vector<Individual> newFemalePopulation;

        while (newMalePopulation.size() + newFemalePopulation.size() < malePopulation.size() + femalePopulation.size()) {
            Individual maleParent = selectParent(malePopulation);
            Individual femaleParent = selectParent(femalePopulation);

            std::pair<Individual, Individual> offspring = applyCrossover(maleParent, femaleParent);

            std::uniform_real_distribution<> distProb(0.0, 1.0);
            if (distProb(rng) < config.getMutProb()) {
                applyMutation(offspring.first);
            }
            if (distProb(rng) < config.getMutProb()) {
                applyMutation(offspring.second);
            }

            offspring.first.setFitness(problem.eval(offspring.first.getRoute()));
            offspring.second.setFitness(problem.eval(offspring.second.getRoute()));

            newMalePopulation.push_back(offspring.first);
            newFemalePopulation.push_back(offspring.second);

            if (offspring.first.getFitness() < bestSolution.getFitness()) {
                bestSolution = offspring.first;
            }
            if (offspring.second.getFitness() < bestSolution.getFitness()) {
                bestSolution = offspring.second;
            }
        }

        malePopulation = std::move(newMalePopulation);
        femalePopulation = std::move(newFemalePopulation);
        logger.logGeneration(i, getFitnesses());

    }

    return bestSolution;
}

void EvolutionaryAlgorithmSex::initializePopulation() {
    malePopulation.clear();
    femalePopulation.clear();
    int totalPopulationSize = config.getPopulationSize();
    int maleSize = totalPopulationSize / 2;
    int femaleSize = totalPopulationSize - maleSize;

    int size = problem.getDimension() - 1;
    std::vector<int> route(size);

    int idx = 0;
    for (int i = 0; i < size + 1; i++) {
        if (i != problem.getDepot()) {
            route[idx] = i;
            idx++;
        }
    }

    for (int i = 0; i < maleSize; ++i) {
        std::shuffle(route.begin(), route.end(), rng);
        malePopulation.emplace_back(route, 0);
    }

    for (int i = 0; i < femaleSize; ++i) {
        std::shuffle(route.begin(), route.end(), rng);
        femalePopulation.emplace_back(route, 0);
    }
}

void EvolutionaryAlgorithmSex::evaluatePopulation() {
    for (auto& ind : malePopulation) {
        ind.fitness = problem.eval(ind.route);
    }
    for (auto& ind : femalePopulation) {
        ind.fitness = problem.eval(ind.route);
    }
}

Individual EvolutionaryAlgorithmSex::selectParent(const std::vector<Individual>& population) {
    if (config.getTournamentSize() > 0) {
        return Operators::tourSelection(population, config.getTournamentSize(), rng);
    } else {
        return Operators::rouletteScaledSelection(population, rng);
    }
}

std::pair<Individual, Individual> EvolutionaryAlgorithmSex::applyCrossover(const Individual& maleParent, const Individual& femaleParent) {
    if (config.getCrossoverOperator() == "OX") {
        return std::pair<Individual, Individual>(
                Operators::crossOX(maleParent, femaleParent, rng),
                Operators::crossOX(femaleParent, maleParent, rng)
        );
    } else if (config.getCrossoverOperator() == "CX") {
        return std::pair<Individual, Individual>(
                Operators::crossCX(maleParent, femaleParent),
                Operators::crossCX(femaleParent, maleParent)
        );
    } else if (config.getCrossoverOperator() == "PMX") {
        return std::pair<Individual, Individual>(
                Operators::crossPMX(maleParent, femaleParent, rng),
                Operators::crossPMX(femaleParent, maleParent, rng)
        );
    } else if (config.getCrossoverOperator() == "PositionBased") {
        return crossPositionBased(maleParent, femaleParent, rng);
    } else {
        return std::pair<Individual, Individual>(
                Operators::crossOX(maleParent, femaleParent, rng),
                Operators::crossOX(femaleParent, maleParent, rng)
        );
    }}

void EvolutionaryAlgorithmSex::applyMutation(Individual& ind) {
    if (config.getMutationOperator() == "swap") {
        Operators::mutationSwapOneRandom(ind, rng);
    } else if (config.getMutationOperator() == "inverse") {
        Operators::mutationInverse(ind, rng);
    } else if (config.getMutationOperator() == "scramble") {
        mutationScramble(ind, rng);
    } else if (config.getMutationOperator() == "insertion") {
        mutationInsertion(ind, rng);
    } else {
        Operators::mutationSwapOneRandom(ind, rng);
    }}

std::vector<double> EvolutionaryAlgorithmSex::getFitnesses() const {
    std::vector<double> fitnesses;
    for (const auto& ind : malePopulation) {
        fitnesses.push_back(ind.fitness);
    }
    for (const auto& ind : femalePopulation) {
        fitnesses.push_back(ind.fitness);
    }
    return fitnesses;
}


