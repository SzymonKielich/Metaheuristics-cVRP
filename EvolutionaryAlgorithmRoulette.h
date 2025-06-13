#ifndef EVOLUTIONARYALGORITHMROULETTE_H
#define EVOLUTIONARYALGORITHMROULETTE_H

#include <vector>
#include <random>
#include <string>
#include <unordered_map>
#include "Config.h"
#include "Problem.h"
#include "Individual.h"
#include "Logger.h"

class EvolutionaryAlgorithmRoulette {
public:
    EvolutionaryAlgorithmRoulette(const Config& cfg, const Problem& prob, const std::string& logFilePath,
                                  bool accumulateScores = false, bool logOperatorUsage = false,
                                  bool useAverageImprovement = false, int resetInterval = 0);

    Individual run();

private:
    Config config;
    Problem problem;
    std::vector<Individual> population;
    std::mt19937 rng;
    Logger logger;

    std::vector<std::string> crossoverOperators;
    std::vector<std::string> mutationOperators;
    std::unordered_map<std::string, double> crossoverOperatorProbabilities;
    std::unordered_map<std::string, double> mutationOperatorProbabilities;
    std::unordered_map<std::string, double> crossoverOperatorScores;
    std::unordered_map<std::string, double> mutationOperatorScores;
    std::unordered_map<std::string, int> crossoverOperatorUsage;
    std::unordered_map<std::string, int> mutationOperatorUsage;

    std::unordered_map<std::string, double> crossoverOperatorPerformance;
    std::unordered_map<std::string, double> mutationOperatorPerformance;


    int resetInterval;
    int generationsSinceReset;

    bool accumulateScores; // czy sumowac
    bool logOperatorUsage; // czy logowac ile operatorow w generacji
    bool useAverageImprovement; //

    Logger operatorLogger;

    double initialCrossProb;
    double initialMutProb;

    void initializePopulation();
    void evaluatePopulation();
    Individual selectParent();
    std::pair<Individual, Individual> applyCrossover(const std::string& crossoverOp, const Individual& parent1, const Individual& parent2);
    void applyMutation(const std::string& mutationOp, Individual& ind);
    std::string selectCrossoverOperator();
    std::string selectMutationOperator();
    void updateOperatorProbabilities();
    void logOperatorUsageData(int generation);
    std::vector<double> getFitnesses() const;
};

#endif // EVOLUTIONARYALGORITHMROULETTE_H
