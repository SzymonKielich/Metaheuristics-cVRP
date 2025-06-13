//
// Created by szymo on 06/10/2024.
//
#include "Config.h"
#include "Logger.h"
#include "Problem.h"
#include "Individual.h"
#include "Operators.h"
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include "EvolutionaryAlgorithm.h"
//todo stary swap
//Individual EvolutionaryAlgorithm::run() {
//    rng = std::mt19937 (std::chrono::system_clock::now().time_since_epoch().count());
//    config.printConfig();
//    initializePopulation();
//    evaluatePopulation();
//    logger.logGeneration(0, getFitnesses());
//
//    Individual bestSolution = population[0];
//    //std::cout<<"1.";
//    //printPopulation();
//    for (int t = 1; t <= config.getGenerations(); ++t) {
//        std::vector<Individual> newPopulation;
//
//        while (newPopulation.size() < population.size()) {
//            Individual parent1 = selectParent();
//            Individual parent2 = selectParent();
//            //std::cout<<"parent1"<<std::endl;
//            //std::cout<<parent1.toString();
//            //std::cout<<"parent2"<<std::endl;
//            //std::cout<<parent2.toString();
//
//
//            Individual child = parent1;
//            std::uniform_real_distribution<> distProb(0.0, 1.0);
//            if (distProb(rng) < config.getCrossProb()) {
////                    child = crossOX(parent1, parent2, rng);
//                applyCrossover(parent1, parent2);
////                    child = crossCX(parent1, parent2);
//
//                //std::cout<<"crossed"<<std::endl;
//
//            }
//            //std::cout<<"offcross"<<child.toString()<<std::endl;
//
//            if (distProb(rng) < config.getMutProb()) {
////                    mutationInverse(child, rng);
//                applyMutation(child);
//
//                ////std::cout<<"mutated"<<std::endl;
//
//            }
//            ////std::cout<<"offmut"<<child.toString()<<std::endl;
//
//
//
//
//            child.fitness = problem.eval(child.route);
//            if(child.isValidIndividual(problem.getDimension())) {
//
//                newPopulation.push_back(std::move(child));
//
//            } else {
//                std::cout<<"wrong";
//            }
//
//            if (newPopulation.back().fitness < bestSolution.fitness) {
//                bestSolution = newPopulation.back();
//            }
//        }
//
//        population = std::move(newPopulation);
//        logger.logGeneration(t, getFitnesses());
//    }
//
//    return bestSolution;
//}
Individual EvolutionaryAlgorithm::run() {
    rng = std::mt19937 (std::chrono::system_clock::now().time_since_epoch().count());
//    config.printConfig();
    initializePopulation();
    evaluatePopulation();
    logger.log("generation,best,avg,worst,std\n");
    logger.logGeneration(0, getFitnesses());

    Individual bestSolution = population[0];
    //std::cout<<"1.";
    //printPopulation();
    for (int t = 1; t <= config.getGenerations(); ++t) {
        std::vector<Individual> newPopulation;

        while (newPopulation.size() < population.size()) {

            Individual parent1 = selectParent();
            Individual parent2 = selectParent();



            Individual child = parent1;
            std::uniform_real_distribution<> distProb(0.0, 1.0);
            if (distProb(rng) < config.getCrossProb()) {
                applyCrossover(parent1, parent2);


            }
            if (config.getMutationOperator() == "swap") {
                for(int i=0; i<child.route.size()-1;i++) {
                    if (distProb(rng) < config.getMutProb()) {
                        Operators::mutationSwapIdx(i, child, rng);
                    }
                }
            } else if (config.getMutationOperator()=="inverse" ||config.getMutationOperator()=="inv") {
                if (distProb(rng) < config.getCrossProb()) {
                    Operators::mutationInverse(child, rng);
                }
            }

            child.fitness = problem.eval(child.route);
//            if(child.isValidIndividual(problem.getDimension())) {

            newPopulation.push_back(std::move(child));
//
//            } else {
//                std::cout<<"wrong";
//            }

            if (newPopulation.back().fitness < bestSolution.fitness) {
                bestSolution = newPopulation.back();
            }
        }

        population = std::move(newPopulation);
        logger.logGeneration(t, getFitnesses());
    }

    return bestSolution;
}
void EvolutionaryAlgorithm::printPopulation() {
    for(auto & ind: population) {
        problem.printRoute(ind.route);
        //std::cout<<ind.fitness<<std::endl;
    }
}
Individual EvolutionaryAlgorithm::applyCrossover(const Individual& parent1, const Individual& parent2) {
    if (config.getCrossoverOperator() == "OX") {
        return Operators::crossOX(parent1, parent2, rng);
    } else if (config.getCrossoverOperator() == "PMX") {
        return Operators::crossPMX(parent1, parent2, rng);
    } else if (config.getCrossoverOperator() == "CX") {
        return Operators::crossCX(parent1, parent2);
    } else {
        return Operators::crossOX(parent1, parent2, rng);
    }
}
void EvolutionaryAlgorithm::applyMutation(Individual& ind) {

    if (config.getMutationOperator() == "swap") {
        Operators::mutationSwapOneRandom(ind, rng);
    } else if (config.getMutationOperator()=="inverse" ||config.getMutationOperator()=="inv") {
//        std::cout<<"inv"<<std::endl;
        Operators::mutationInverse(ind, rng);
    } else {
        Operators::mutationSwapOneRandom(ind, rng);
    }
}

void EvolutionaryAlgorithm::initializePopulation() {

    population.clear();
    int size = problem.getDimension() - 1; //bez depot
    std::vector<int> route(size);

    int idx = 0;
    for(int i=0; i<size+1; i++) {
        if(i != problem.getDepot()) {
            route[idx] = i;
            idx++;
        }
    }

    for (int i = 0; i < config.getPopulationSize(); ++i) {
        std::shuffle(route.begin(), route.end(), rng);
        population.emplace_back(route, 0);
    }
//    printPopulation();
}

void EvolutionaryAlgorithm::evaluatePopulation() {
    for (auto& ind : population) {
        ind.fitness = problem.eval(ind.route);
    }
}

Individual EvolutionaryAlgorithm::selectParent() {
    if (config.getTournamentSize() > 0) {
        return Operators::tourSelection(population, config.getTournamentSize(), rng);
    } else {
        return Operators::rouletteScaledSelection(population, rng);
    }
}

std::vector<double> EvolutionaryAlgorithm::getFitnesses() const {
    std::vector<double> fitnesses;
    for (const auto& ind : population) {
        fitnesses.push_back(ind.fitness);
    }
    return fitnesses;
}
