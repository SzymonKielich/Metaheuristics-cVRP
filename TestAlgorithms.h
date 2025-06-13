//
// Created by szymo on 02/12/2024.
//

#ifndef METAHEURYSTYKI_CVRP_TESTALGORITHMS_H
#define METAHEURYSTYKI_CVRP_TESTALGORITHMS_H
#include <iostream>
#include <chrono>
#include "Problem.h"
#include "EvolutionaryAlgorithmRoulette.h"
#include "EvolutionaryAlgorithmSex.h"

static void testAlgorithms() {
    Problem problem("../problems/A-n60-k9.vrp");

//    Config rouletteConfig(80, 1250, 0.1, 0.9, 20, "", "");
    Config rouletteConfig(300, 334, 0.45, 0.9, 20, "", "");

    EvolutionaryAlgorithmRoulette rouletteAlg(rouletteConfig, problem, "roulette_logft.csv", false, true, true, 5);

    auto start = std::chrono::high_resolution_clock::now();
    Individual bestRoulette = rouletteAlg.run();
    auto end = std::chrono::high_resolution_clock::now();
    double durationRoulette = std::chrono::duration<double>(end - start).count();
    std::cout<< "Roulette Algorithm Best Fitness: " << bestRoulette.getFitness() << "\n";
    std::cout << "Time: " << durationRoulette << " seconds\n";


        Config rouletteConfig2(300, 334, 0.8, 0.8,14, "", "");
//    Config rouletteConfig2(120, 834, 0.65, 0.9,20, "", "");
//    Config rouletteConfig2(80, 1250, 0.85, 0.9,20, "", "");

//    Config rouletteConfig2(1250, 80, 0.45, 0.9,20, "", "");

    // bool accumulateScores = false, bool logOperatorUsage = false, bool useAverageImprovement = false)
    EvolutionaryAlgorithmRoulette rouletteAlg2(rouletteConfig2, problem, "roulette_log2tf.csv", true, true, false, 0);

    auto start2 = std::chrono::high_resolution_clock::now();
    Individual bestRoulette2 = rouletteAlg2.run();
    auto end2 = std::chrono::high_resolution_clock::now();
    double durationRoulette2 = std::chrono::duration<double>(end2 - start2).count();
    std::cout<< "Roulette Algorithm Best Fitness: " << bestRoulette2.getFitness() << "\n";
    std::cout << "Time: " << durationRoulette2 << " seconds\n";

    Config rouletteConfig3(300, 334, 0.1, 0.9, 14, "", "");
//    Config rouletteConfig3(300, 334, 0.8, 0.8, 18, "", "");
//    Config rouletteConfig3(1250, 80, 0.85, 0.9, 14, "", "");
    EvolutionaryAlgorithmRoulette rouletteAlg3(rouletteConfig3, problem, "roulette_log3ff.csv", false, true, false, 180);
    auto start3 = std::chrono::high_resolution_clock::now();
    Individual bestRoulette3 = rouletteAlg3.run();
    auto end3 = std::chrono::high_resolution_clock::now();
    double durationRoulette3 = std::chrono::duration<double>(end3 - start3).count();
    std::cout<< "Roulette Algorithm Best Fitness: " << bestRoulette3.getFitness() << "\n";
    std::cout << "Time: " << durationRoulette3 << " seconds\n";

//    Config rouletteConfig4(120, 834, 0.45, 0.9, 20, "", "");
//    Config rouletteConfig4(120, 834, 0.65, 0.9, 14, "", "");
//    Config rouletteConfig4(120, 834, 0.45, 0.9, 20, "", "");
    Config rouletteConfig4(80, 1250, 0.3, 0.8, 18, "", "");

    EvolutionaryAlgorithmRoulette rouletteAlg4(rouletteConfig4, problem, "roulette_log4tt.csv", true, true, true);

    auto start4 = std::chrono::high_resolution_clock::now();
    Individual bestRoulette4 = rouletteAlg4.run();
    auto end4 = std::chrono::high_resolution_clock::now();
    double durationRoulette4 = std::chrono::duration<double>(end4 - start4).count();
    std::cout<< "Roulette Algorithm Best Fitness: " << bestRoulette4.getFitness() << "\n";
    std::cout << "Time: " << durationRoulette4 << " seconds\n";

    Config rouletteConfig5(200, 500, 0.5, 0.1, 5, "", "");
    EvolutionaryAlgorithmRoulette rouletteAlg5(rouletteConfig5, problem, "roulette_log5ff.csv", false, true, false, 50);

    auto start5 = std::chrono::high_resolution_clock::now();
    Individual bestRoulette5 = rouletteAlg5.run();
    auto end5 = std::chrono::high_resolution_clock::now();
    double durationRoulette5 = std::chrono::duration<double>(end5 - start5).count();
    std::cout<< "Roulette Algorithm Best Fitness: " << bestRoulette5.getFitness() << "\n";
    std::cout << "Time: " << durationRoulette5 << " seconds\n";
//
    Config sexConfig(100, 100, 0.9, 0.1, 5, "", "");
    EvolutionaryAlgorithmSex sexAlg(sexConfig, problem, "../list5/sex_log.csv");

    start = std::chrono::high_resolution_clock::now();
    Individual bestSex = sexAlg.run();
    end = std::chrono::high_resolution_clock::now();
    double durationSex = std::chrono::duration<double>(end - start).count();

    std::cout << "Sex-based Algorithm Best Fitness: " << bestSex.getFitness() << "\n";
    std::cout << "Time: " << durationSex << " seconds\n";
}

#endif //METAHEURYSTYKI_CVRP_TESTALGORITHMS_H