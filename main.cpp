#include <iostream>
#include "Problem.h"
#include "Config.h"
#include <chrono>
#include <filesystem>
#include "EvolutionaryAlgorithm.h"
#include "TabuSearch.h"
#include "TabuConfig.h"
#include "SAconfig.h"
#include "SimulatedAnnealing.h"
#include "List4.h"
#include "TestAlgorithms.h"
//#include "SA.h"
//#include "List4.h"

std::string basePath;
void setBasePath() {
#ifdef _WIN32
    basePath = "C:/Users/szymo/CLionProjects/Metaheurystyki_cVRP";
#else
    basePath = "/mnt/c/Users/szymo/CLionProjects/Metaheurystyki_cVRP";
#endif
}
void testOnAllFiles() {
    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };

//    int popSize = 100;
//    int generations = 1000;
//    std::string crossoverOperator = "OX";
//    double crossProb = 0.8;
//    std::string mutationOperator = "inverse";
//    double mutProb = 0.1;
//    int tournamentSize = 15;
    std::ostringstream filePath;
    filePath << "../summaries/gaa.csv";
//    filePath << "../summaries/ga_progress.csv";

    Logger logger(filePath.str(), "best,worst,avg,std");

    Config config(80, 1250, 0.93, 0.8, 15, "OX", "inverse");
    for (auto &file: problemFiles) {
        std::vector<double> bestValues;
        double bestOverall = std::numeric_limits<double>::max();
        double worstOverall = std::numeric_limits<double>::lowest();
        double totalFitness = 0.0;
        Problem problem("../problems/" + file);

        for (int run = 0; run < 10; ++run) {
            EvolutionaryAlgorithm ea(config, problem);
            Individual bestSolution = ea.run();

            if (bestSolution.fitness < bestOverall) {
                bestOverall = bestSolution.fitness;
            }
            if (bestSolution.fitness > worstOverall) {
                worstOverall = bestSolution.fitness;
            }
            totalFitness += bestSolution.fitness;
            bestValues.push_back(bestSolution.fitness);
        }

        //    auto endTime = std::chrono::high_resolution_clock::now();
        //    std::chrono::duration<double> elapsed = endTime - startTime;

        double avgFitness = totalFitness / bestValues.size();
        double stdDev = std::sqrt(std::accumulate(bestValues.begin(), bestValues.end(), 0.0,
                                                  [avgFitness](double sum, double val) {
                                                      return sum +
                                                             (val - avgFitness) *
                                                             (val - avgFitness);
                                                  }) / bestValues.size());

        logger.logStats(std::make_tuple(bestOverall, worstOverall, avgFitness, stdDev));

    }
}

void tabuTest() {
    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };
    int fileNumber = 6;
    int iterations = 1500;
    int tabuListSize = 10;
    int neighborCount = 67;
    std::string initializationMethod = "random";
    std::string neighborhoodOperator = "inverse";
//    Problem problem("../problems/"+problemFiles[fileNumber]);
    Problem problem("../problems/" + problemFiles[fileNumber]);

    std::cout << "../problems/" + problemFiles[fileNumber] << std::endl;
    TabuConfig config(iterations, tabuListSize, neighborCount, initializationMethod, neighborhoodOperator, 0.001);

    std::vector<double> bestValues;
    double bestOverall = std::numeric_limits<double>::max();
    double worstOverall = std::numeric_limits<double>::lowest();
    double totalFitness = 0.0;

//    auto startTime = std::chrono::high_resolution_clock::now();

    TabuSearch ts(config, problem, "../summaries/tabu_progress.csv");
    ts.run();
}

void saTest() {
    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };
    int fileNumber = 7;
    int iterations = 1000;
    double initialTemperature = 1000.0;
    double finalTemperature = 0.5;
    double coolingRate = 0.96;
    int neighborCount = 100;

    std::string initializationMethod = "random";
    std::string neighborhoodOperator = "inverse";
    Problem problem("../problems/" + problemFiles[fileNumber]);

    std::cout << "../problems/" + problemFiles[fileNumber] << std::endl;
    SAConfig config(iterations, initialTemperature, finalTemperature, coolingRate, neighborCount, initializationMethod,
                    neighborhoodOperator, 0.001);

    std::vector<double> bestValues;
    double bestOverall = std::numeric_limits<double>::max();
    double worstOverall = std::numeric_limits<double>::lowest();
    double totalFitness = 0.0;

    SimulatedAnnealing sa(config, problem, "../summaries/sa_progress.csv");
    sa.run();
}
//void saThreadsTest() {
//    std::vector<std::string> problemFiles = {
//            "toy.vrp",
//            "A-n32-k5.vrp",
//            "A-n37-k6.vrp",
//            "A-n39-k5.vrp",
//            "A-n45-k6.vrp",
//            "A-n48-k7.vrp",
//            "A-n54-k7.vrp",
//            "A-n60-k9.vrp"
//    };
//    int fileNumber = 6;
//    int iterations = 1000;
//    double initialTemperature = 500.0;
//    double finalTemperature = 0.0;
//    double coolingRate = 0.992;
//    int neighborCount = 40;
//
//    std::string initializationMethod = "greedy";
//    std::string neighborhoodOperator = "inverse";
//    Problem problem("../problems/" + problemFiles[fileNumber]);
//
//    std::cout << "../problems/" + problemFiles[fileNumber] << std::endl;
//    SAConfig config(iterations, initialTemperature, finalTemperature, coolingRate, neighborCount, initializationMethod,
//                    neighborhoodOperator, 0.001);
//
//    std::vector<double> bestValues;
//    double bestOverall = std::numeric_limits<double>::max();
//    double worstOverall = std::numeric_limits<double>::lowest();
//    double totalFitness = 0.0;
//
//    SA sa(config, problem, "../summaries/sa" + std::to_string(fileNumber) + ".csv");
//    sa.run();
//}
void testOnAllFilesRoulette() {
    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };

//    Config config(80, 1250, 0.3, 0.8, 18, "", "");
//truetrue
//    Config rouletteConfig(80, 1250, 0.1, 0.9, 20, "", "");
//    Config rouletteConfig2(300, 334, 0.8, 0.8,14, "", "");
//    Config rouletteConfig2(120, 834, 0.65, 0.9,20, "", "");
//    Config rouletteConfig2(80, 1250, 0.85, 0.9,20, "", "");

//    Config rouletteConfig2(1250, 80, 0.45, 0.9,20, "", "");
//    Config rouletteConfig3(300, 334, 0.1, 0.9, 14, "", "");
//    Config rouletteConfig3(300, 400, 0.8, 0.8, 18, "", "");
//    Config rouletteConfig3(1250, 80, 0.85, 0.9, 14, "", "");
    Config rouletteConfig2(80, 1250, 0.85, 0.9,20, "", "");
//    Config rouletteConfig4(120, 834, 0.65, 0.9, 14, "", "");
//    Config rouletteConfig4(120, 834, 0.45, 0.9, 20, "", "");
//    Config rouletteConfig4(80, 1250, 0.3, 0.8, 18, "", "");
    double totalBest = 0.0;
    double totalWorst = 0.0;
    double totalAvg = 0.0;

    for (auto &file: problemFiles) {
        std::vector<double> bestValues;
        double bestOverall = std::numeric_limits<double>::max();
        double worstOverall = std::numeric_limits<double>::lowest();
        double totalFitness = 0.0;
        Problem problem("../problems/" + file);

        for (int run = 0; run < 10; ++run) {
            EvolutionaryAlgorithmRoulette ea(rouletteConfig2, problem, "roulette_log2tf.csv", true, true, false, 0);
            Individual bestSolution = ea.run();

            if (bestSolution.fitness < bestOverall) {
                bestOverall = bestSolution.fitness;
            }
            if (bestSolution.fitness > worstOverall) {
                worstOverall = bestSolution.fitness;
            }
            totalFitness += bestSolution.fitness;
            bestValues.push_back(bestSolution.fitness);
        }

        double avgFitness = totalFitness / bestValues.size();
        double stdDev = std::sqrt(std::accumulate(bestValues.begin(), bestValues.end(), 0.0,
                                                  [avgFitness](double sum, double val) {
                                                      return sum + (val - avgFitness) * (val - avgFitness);
                                                  }) / bestValues.size());

        std::ofstream summaryFile("../summaries/roulette_summary.csv", std::ios::app);
        int roundedAvg = static_cast<int>(std::ceil(avgFitness));
        int roundedStdDev = static_cast<int>(std::ceil(stdDev));
        summaryFile << file << "," << bestOverall << "," << worstOverall << "," << roundedAvg << "," << roundedStdDev << "\n";
        summaryFile.close();

        totalBest += bestOverall;
        totalWorst += worstOverall;
        totalAvg += avgFitness;
    }

    std::cout << "Total:"<<totalBest+totalAvg+totalWorst<<std::endl;

}
void testOnAllFilesSex() {
    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };

//    Config config(660, 151, 0.2, 0.87, 16, "OX", "inverse"); //24937.2
//    Config config(800, 125, 0.5, 0.87, 16, "OX", "inverse"); //24852.8
//    Config config(500, 200, 0.1, 0.82, 16, "OX", "inverse"); //24908.7
    Config config(300, 333, 0.05, 0.95, 16, "OX", "inverse"); //24775.1
    double totalBest = 0.0;
    double totalWorst = 0.0;
    double totalAvg = 0.0;

    for (auto &file: problemFiles) {
        std::vector<double> bestValues;
        double bestOverall = std::numeric_limits<double>::max();
        double worstOverall = std::numeric_limits<double>::lowest();
        double totalFitness = 0.0;
        Problem problem("../problems/" + file);

        for (int run = 0; run < 10; ++run) {
            EvolutionaryAlgorithmSex ea(config, problem, "../list5/sex_" + file + ".csv");
            Individual bestSolution = ea.run();

            if (bestSolution.fitness < bestOverall) {
                bestOverall = bestSolution.fitness;
            }
            if (bestSolution.fitness > worstOverall) {
                worstOverall = bestSolution.fitness;
            }
            totalFitness += bestSolution.fitness;
            bestValues.push_back(bestSolution.fitness);
        }

        double avgFitness = totalFitness / bestValues.size();
        double stdDev = std::sqrt(std::accumulate(bestValues.begin(), bestValues.end(), 0.0,
                                                  [avgFitness](double sum, double val) {
                                                      return sum + (val - avgFitness) * (val - avgFitness);
                                                  }) / bestValues.size());

        std::ofstream summaryFile("../list5/sex_summary.csv", std::ios::app);
        summaryFile << file << "," << bestOverall << "," << worstOverall << "," << avgFitness << "," << stdDev << "\n";
        summaryFile.close();

        totalBest += bestOverall;
        totalWorst += worstOverall;
        totalAvg += avgFitness;
    }

    std::cout << "Total:"<<totalBest+totalAvg+totalWorst<<std::endl;
}
void testOnAllFilesTabu() {
    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };
    int fileNumber = 6;
    int iterations = 1500;
    int tabuListSize = 10;
    int neighborCount = 67;
    std::string initializationMethod = "greedy";
    std::string neighborhoodOperator = "inverse";
//    Problem problem("../problems/"+problemFiles[fileNumber]);
    Problem problem("../problems/" + problemFiles[fileNumber]);
    std::ostringstream filePath;

    filePath << "../summaries/" << "t" << tabuListSize << "i" << iterations << "n" << neighborhoodOperator
             << initializationMethod << neighborhoodOperator << ".csv";

    Logger logger(filePath.str(), "best,worst,avg,std");

    std::cout << "../problems/" + problemFiles[fileNumber] << std::endl;
    TabuConfig config(iterations, tabuListSize, neighborCount, initializationMethod, neighborhoodOperator, 0.001);

    std::vector<double> bestValues;
    double bestOverall = std::numeric_limits<double>::max();
    double worstOverall = std::numeric_limits<double>::lowest();
    double totalFitness = 0.0;

//    auto startTime = std::chrono::high_resolution_clock::now();

    for (auto &file: problemFiles) {
        std::vector<double> bestValues;
        double bestOverall = std::numeric_limits<double>::max();
        double worstOverall = std::numeric_limits<double>::lowest();
        double totalFitness = 0.0;
        Problem problem("../problems/" + file);

        for (int run = 0; run < 10; ++run) {
            TabuSearch ts(config, problem, "../summaries/tabu"+file+".csv");
            Individual bestSolution = ts.run();

            if (bestSolution.fitness < bestOverall) {
                bestOverall = bestSolution.fitness;
            }
            if (bestSolution.fitness > worstOverall) {
                worstOverall = bestSolution.fitness;
            }
            totalFitness += bestSolution.fitness;
            bestValues.push_back(bestSolution.fitness);
        }

        //    auto endTime = std::chrono::high_resolution_clock::now();
        //    std::chrono::duration<double> elapsed = endTime - startTime;

        double avgFitness = totalFitness / bestValues.size();
        double stdDev = std::sqrt(std::accumulate(bestValues.begin(), bestValues.end(), 0.0,
                                                  [avgFitness](double sum, double val) {
                                                      return sum +
                                                             (val - avgFitness) *
                                                             (val - avgFitness);
                                                  }) / bestValues.size());

        logger.logStats(std::make_tuple(bestOverall, worstOverall, avgFitness, stdDev));


    }
}

void testOnAllFilesSA() {
    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };
    int fileNumber = 2;
    int iterations = 1000;
    double initialTemperature = 1000.0;
    double finalTemperature = 0.5;
    double coolingRate = 0.96;
    int neighborCount = 100;

    std::string initializationMethod = "greedy";
    std::string neighborhoodOperator = "inverse";
    Problem problem("../problems/" + problemFiles[fileNumber]);
    std::ostringstream filePath;

    filePath << "../summaries/" << "cr" << coolingRate << ".csv";

    Logger logger(filePath.str(), "best,worst,avg,std");

    std::cout << "../problems/" + problemFiles[fileNumber] << std::endl;

    std::vector<double> bestValues;
    double bestOverall = std::numeric_limits<double>::max();
    double worstOverall = std::numeric_limits<double>::lowest();
    double totalFitness = 0.0;

    auto startTime = std::chrono::high_resolution_clock::now();

    for (auto &file: problemFiles) {
        std::vector<double> bestValues;
        double bestOverall = std::numeric_limits<double>::max();
        double worstOverall = std::numeric_limits<double>::lowest();
        double totalFitness = 0.0;
        Problem problem("../problems/" + file);
        SAConfig config(iterations, initialTemperature, finalTemperature, coolingRate, neighborCount, initializationMethod,
                        neighborhoodOperator, 0.001);


        for (int run = 0; run < 10; ++run) {
            SimulatedAnnealing sa(config, problem, "../summaries/SA"+file+".csv");
            Individual bestSolution = sa.run();

            if (bestSolution.fitness < bestOverall) {
                bestOverall = bestSolution.fitness;
            }
            if (bestSolution.fitness > worstOverall) {
                worstOverall = bestSolution.fitness;
            }
            totalFitness += bestSolution.fitness;
            bestValues.push_back(bestSolution.fitness);
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;

        double avgFitness = totalFitness / bestValues.size();
        double stdDev = std::sqrt(std::accumulate(bestValues.begin(), bestValues.end(), 0.0,
                                                  [avgFitness](double sum, double val) {
                                                      return sum +
                                                             (val - avgFitness) *
                                                             (val - avgFitness);
                                                  }) / bestValues.size());
        std::cout<<"without threads time: "<<elapsed.count()<< std::endl;
        logger.logStats(std::make_tuple(bestOverall, worstOverall, avgFitness, stdDev));

    }
}
//void testOnAllFilesSAThreads() {
//    std::vector<std::string> problemFiles = {
//            "toy.vrp",
//            "A-n32-k5.vrp",
//            "A-n37-k6.vrp",
//            "A-n39-k5.vrp",
//            "A-n45-k6.vrp",
//            "A-n48-k7.vrp",
//            "A-n54-k7.vrp",
//            "A-n60-k9.vrp"
//    };
//    int fileNumber = 2;
//    int iterations = 2000;
//    double initialTemperature = 500;
//    double finalTemperature = 0.1;
//    double coolingRate = 0.995;
//    int neighborCount = 20;
//    std::string initializationMethod = "random";
//    std::string neighborhoodOperator = "inverse";
//    Problem problem("../problems/" + problemFiles[fileNumber]);
//    std::ostringstream filePath;
//
//    filePath << "../summaries/" << "cr" << coolingRate << ".csv";
//
//    Logger logger(filePath.str(), "best,worst,avg,std");
//
//    std::cout << "../problems/" + problemFiles[fileNumber] << std::endl;
//
//    std::vector<double> bestValues;
//    double bestOverall = std::numeric_limits<double>::max();
//    double worstOverall = std::numeric_limits<double>::lowest();
//    double totalFitness = 0.0;
//
//    auto startTime = std::chrono::high_resolution_clock::now();
//
//    for (auto &file: problemFiles) {
//        std::vector<double> bestValues;
//        double bestOverall = std::numeric_limits<double>::max();
//        double worstOverall = std::numeric_limits<double>::lowest();
//        double totalFitness = 0.0;
//        Problem problem("../problems/" + file);
//        SAConfig config(iterations, initialTemperature, finalTemperature, coolingRate, neighborCount, initializationMethod,
//                        neighborhoodOperator, 0.001);
//
//
//        for (int run = 0; run < 10; ++run) {
//            SA sa(config, problem, "../summaries/sa" + std::to_string(fileNumber) + ".csv");
//            Individual bestSolution = sa.run();
//
//            if (bestSolution.fitness < bestOverall) {
//                bestOverall = bestSolution.fitness;
//            }
//            if (bestSolution.fitness > worstOverall) {
//                worstOverall = bestSolution.fitness;
//            }
//            totalFitness += bestSolution.fitness;
//            bestValues.push_back(bestSolution.fitness);
//        }
//
//        auto endTime = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> elapsed = endTime - startTime;
//
//        double avgFitness = totalFitness / bestValues.size();
//        double stdDev = std::sqrt(std::accumulate(bestValues.begin(), bestValues.end(), 0.0,
//                                                  [avgFitness](double sum, double val) {
//                                                      return sum +
//                                                             (val - avgFitness) *
//                                                             (val - avgFitness);
//                                                  }) / bestValues.size());
//        std::cout<<"with threads time: "<<elapsed.count()<< std::endl;
//        logger.logStats(std::make_tuple(bestOverall, worstOverall, avgFitness, stdDev));
//
//    }
//}

double test() {
    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };
    int fileNumber = 7;
    Problem problem("../problems/" + problemFiles[fileNumber]);
    Config config(80, 1250, 0.93, 0.8, 15, "OX", "inverse");
    std::vector<double> bestValues;
    double bestOverall = std::numeric_limits<double>::max();
    double worstOverall = std::numeric_limits<double>::lowest();
    double totalFitness = 0.0;

//    auto startTime = std::chrono::high_resolution_clock::now();

    for (int run = 0; run < 3; ++run) {
//        config.seed = std::chrono::system_clock::now().time_since_epoch().count();
        EvolutionaryAlgorithm ea(config, problem);
        Individual bestSolution = ea.run();

        if (bestSolution.fitness < bestOverall) {
            bestOverall = bestSolution.fitness;
        }
        if (bestSolution.fitness > worstOverall) {
            worstOverall = bestSolution.fitness;
        }
        totalFitness += bestSolution.fitness;
        bestValues.push_back(bestSolution.fitness);
        std::cout << bestSolution.fitness;
    }

//    auto endTime = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> elapsed = endTime - startTime;

    double avgFitness = totalFitness / bestValues.size();
    double stdDev = std::sqrt(std::accumulate(bestValues.begin(), bestValues.end(), 0.0,
                                              [avgFitness](double sum, double val) {
                                                  return sum + (val - avgFitness) *
                                                               (val - avgFitness);
                                              }) / bestValues.size());

//    std::ostringstream filePath;
//    filePath << "../summaries"
//             << problemFiles[fileNumber]<<generations<<"summary.csv";
//
//    bool fileExists = std::filesystem::exists(filePath.str());
//
//    std::ofstream summaryFile(filePath.str(), std::ios::app);
//
////    if (!fileExists) {
////        summaryFile << "Best,Worst,Avg,Std,Time,crossOperator,crossProb,MutOperator,MutProb, popSize,tourSize\n";
////    }
////    summaryFile<< bestOverall << "," << worstOverall
////               << "," << avgFitness << "," << stdDev << "," << elapsed.count() << ","<<crossoverOperator<<","<<crossProb<<","<<mutationOperator<<","<<mutProb<<","<<popSize<<","<<tournamentSize<< "\n";
////    summaryFile.close();

//    std::cout << "Najlepsze rozwiazanie: " << bestOverall << "\n";
//    std::cout << "Najgorsze rozwiazanie: " << worstOverall << "\n";
//    std::cout << "Srednie rozwiazanie: " << avgFitness << "\n";
//    std::cout << "Odchylenie standardowe: " << stdDev << "\n";
//    std::cout << "Czas dzialania: " << elapsed.count() << " sekund\n";

    return avgFitness;
}
void runGA(int fileNumber, int popSize, int generations, double crossProb, double mutProb, int tournamentSize, const std::string& crossoverOperator, const std::string& mutationOperator, int runs, const std::string& outputFile) {
    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };
    Problem problem("../problems/" + problemFiles[fileNumber]);
    Config config(popSize, generations, crossProb, mutProb, tournamentSize, crossoverOperator, mutationOperator);
    std::vector<double> bestValues;
    double bestOverall = std::numeric_limits<double>::max();
    double worstOverall = std::numeric_limits<double>::lowest();
    double totalFitness = 0.0;

    for (int run = 0; run < runs; ++run) {
        EvolutionaryAlgorithm ea(config, problem);
        Individual bestSolution = ea.run();

        if (bestSolution.fitness < bestOverall) {
            bestOverall = bestSolution.fitness;
        }
        if (bestSolution.fitness > worstOverall) {
            worstOverall = bestSolution.fitness;
        }
        totalFitness += bestSolution.fitness;
        bestValues.push_back(bestSolution.fitness);
    }

    double avgFitness = totalFitness / bestValues.size();
    double stdDev = std::sqrt(std::accumulate(bestValues.begin(), bestValues.end(), 0.0,
                                              [avgFitness](double sum, double val) {
                                                  return sum + (val - avgFitness) * (val - avgFitness);
                                              }) / bestValues.size());

    std::ofstream summaryFile("..problems/"+outputFile, std::ios::app);
    summaryFile << bestOverall << "," << worstOverall << "," << avgFitness << "," << stdDev << "\n";
    summaryFile.close();
}

void runTabu(int fileNumber, int iterations, int tabuListSize, int neighborCount, const std::string& initializationMethod, const std::string& neighborhoodOperator, int runs, const std::string& outputFile) {
    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };
    Problem problem("../problems/" + problemFiles[fileNumber]);
    TabuConfig config(iterations, tabuListSize, neighborCount, initializationMethod, neighborhoodOperator, 0.001);
    std::vector<double> bestValues;
    double bestOverall = std::numeric_limits<double>::max();
    double worstOverall = std::numeric_limits<double>::lowest();
    double totalFitness = 0.0;

    for (int run = 0; run < runs; ++run) {
        TabuSearch ts(config, problem, "../summaries/tabu" + problemFiles[fileNumber]+ ".csv");
        Individual bestSolution = ts.run();

        if (bestSolution.fitness < bestOverall) {
            bestOverall = bestSolution.fitness;
        }
        if (bestSolution.fitness > worstOverall) {
            worstOverall = bestSolution.fitness;
        }
        totalFitness += bestSolution.fitness;
        bestValues.push_back(bestSolution.fitness);
    }

    double avgFitness = totalFitness / bestValues.size();
    double stdDev = std::sqrt(std::accumulate(bestValues.begin(), bestValues.end(), 0.0,
                                              [avgFitness](double sum, double val) {
                                                  return sum + (val - avgFitness) * (val - avgFitness);
                                              }) / bestValues.size());

    std::ofstream summaryFile("..problems/"+outputFile, std::ios::app);
    std::cout<<outputFile<<std::endl;
    summaryFile << bestOverall << "," << worstOverall << "," << avgFitness << "," << stdDev << "\n";
    summaryFile.close();
}

void runSA(int fileNumber, int iterations, double initialTemperature, double coolingRate, int neighborCount, const std::string& initializationMethod, const std::string& neighborhoodOperator, int runs, const std::string& outputFile) {
    std::cout<<"wfunkcji"<<std::endl;

    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };
    std::cout<<"dalej"<<problemFiles[fileNumber]<<std::endl;
    std::cout<<basePath+"/problems/"+problemFiles[fileNumber]<<std::endl;
    Problem problem(basePath+"/problems/"+problemFiles[fileNumber]);
    std::cout<<"dalej"<<std::endl;
    SAConfig config(iterations, initialTemperature, 0.0, coolingRate, neighborCount, initializationMethod, neighborhoodOperator, 0.001);
    std::cout<<"dalej"<<std::endl;

    std::vector<double> bestValues;
    double bestOverall = std::numeric_limits<double>::max();
    double worstOverall = std::numeric_limits<double>::lowest();
    double totalFitness = 0.0;
    for (int run = 0; run < runs; ++run) {
        std::cout<<run<<std::endl;
        SimulatedAnnealing sa(config, problem, "../summaries/sa_progress.csv");
        Individual bestSolution = sa.run();

        if (bestSolution.fitness < bestOverall) {
            bestOverall = bestSolution.fitness;
        }
        if (bestSolution.fitness > worstOverall) {
            worstOverall = bestSolution.fitness;
        }
        totalFitness += bestSolution.fitness;
        bestValues.push_back(bestSolution.fitness);
    }

    double avgFitness = totalFitness / bestValues.size();
    double stdDev = std::sqrt(std::accumulate(bestValues.begin(), bestValues.end(), 0.0,
                                              [avgFitness](double sum, double val) {
                                                  return sum + (val - avgFitness) * (val - avgFitness);
                                              }) / bestValues.size());

    std::ofstream summaryFile(outputFile, std::ios::app);

    summaryFile << bestOverall << "," << worstOverall << "," << avgFitness << "," << stdDev << "\n";
    summaryFile.close();
}
void runAlgorithmWithTimeLimit(double timeLimitSeconds) {
    auto startTime = std::chrono::high_resolution_clock::now();
//    while (/* cond */) {
//
//        auto currentTime = std::chrono::high_resolution_clock::now();
//        double elapsedSeconds = std::chrono::duration<double>(currentTime - startTime).count();
//        if (elapsedSeconds >= timeLimitSeconds) {
//            break;
//        }
//    }
}
int main(int argc, char *argv[]) {
    std::vector<std::string> problemFiles = {
            "toy.vrp",
            "A-n32-k5.vrp",
            "A-n37-k6.vrp",
            "A-n39-k5.vrp",
            "A-n45-k6.vrp",
            "A-n48-k7.vrp",
            "A-n54-k7.vrp",
            "A-n60-k9.vrp"
    };
    setBasePath();
    if (argc < 3) {
//        std::cout<<basePath<<std::endl;
//testOnAllFilesTabu();
//tabuTest();
//        tabuTest();
//test();
//    testOnAllFilesSA();
//    testOnAllFilesTabu();
//    testOnAllFiles();
//        runSA(7, 1000.0, 1000, 0.96, 100, "greedy", "inverse", 10, "../summaries/sa_progress.csv");
//        testOnAllFilesSA();
//        saTest();
//            testOnAllFilesSex();
            testOnAllFilesRoulette();
        auto startTime = std::chrono::high_resolution_clock::now();
//        testAlgorithms();
//
//        std::cout<<"start";
//        testall();
//        auto endTime = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> elapsed = endTime - startTime;
//        std::cout << "time in seconds: " << elapsed.count();
    }}
////        testOnAllFilesSA();
////        testOnAllFilesSAThreads();
////        testOnAllFilesTabu();
//        return 0;
//    }
//    int runs;
//    std::string algorithm = argv[1];
//    std::cout<<algorithm<<std::endl;
//    if (algorithm == "ga") {
//        if (argc < 11) {
//            std::cout << "Proper usage for GA: " << argv[0] << " ga <file_number> <pop_size> <generations> <cross_prob> <mut_prob> <tournament_size> <crossover_operator> <mutation_operator> <runs>\n";
//            return 1;
//        }
//        int fileNumber = std::stoi(argv[3]);
//        int popSize = std::stoi(argv[4]);
//        int generations = std::stoi(argv[5]);
//        double crossProb = std::stod(argv[6]);
//        double mutProb = std::stod(argv[7]);
//        int tournamentSize = std::stoi(argv[8]);
//        std::string crossoverOperator = argv[9];
//        std::string mutationOperator = argv[10];
//        int runs =10;
//        std::string outputFile = std::to_string(popSize * generations) + ".csv";
//        runGA(fileNumber, popSize, generations, crossProb, mutProb, tournamentSize, crossoverOperator, mutationOperator, runs, outputFile);
//    } else if (algorithm == "tabu") {
//        if (argc < 10) {
//            std::cout << "Proper usage for Tabu: " << argv[0] << " tabu <file_number> <iterations> <tabu_list_size> <neighbor_count> <initialization_method> <neighborhood_operator> <runs>\n";
//            return 1;
//        }
//        int fileNumber = std::stoi(argv[3]);
//        int iterations = std::stoi(argv[4]);
//        int tabuListSize = std::stoi(argv[5]);
//        int neighborCount = std::stoi(argv[6]);
//        std::string initializationMethod = argv[7];
//        std::string neighborhoodOperator = argv[8];
//        runs=10;
//        std::string outputFile = "../summaries/"+std::to_string(neighborCount * iterations) + ".csv";
//        runTabu(fileNumber, iterations, tabuListSize, neighborCount, initializationMethod, neighborhoodOperator, runs, outputFile);
//    } else if (algorithm == "sa") {
////        if (argc < 11) {
////            std::cout << "Proper usage for SA: " << argv[0] << " sa <file_number> <iterations> <initial_temperature> <cooling_rate> <neighbor_count> <initialization_method> <neighborhood_operator> <runs>\n";
////            return 1;
////        }
//        std::cout<<"algorithm";
//
//        int fileNumber = std::stoi(argv[2]);
//        int iterations = std::stoi(argv[3]);
//        double initialTemperature = std::stod(argv[4]);
//        double coolingRate = std::stod(argv[5]);
//        int neighborCount = std::stoi(argv[6]);
//        std::string initializationMethod = argv[7];
//        std::string neighborhoodOperator = argv[8];
//        runs = std::stoi(argv[9]);
//        std::cout<<fileNumber<<" "<<iterations<<" "<<initialTemperature<<" "<< coolingRate<<neighborCount<<initializationMethod<<neighborhoodOperator<<runs;
//        std::string outputFile = basePath+"dziala.csv";
//        std::cout<<outputFile<<std::endl;

//    } else {
//        std::cout << "Unknown algorithm: " << algorithm << "\n";
//        std::cout << "Algorithms: ga (Genetic Algorithm), tabu (Tabu Search), sa (Simulated Annealing)\n";
//        return 1;
//    }
////    testOnAllFilesSA();
////    testOnAllFilesTabu();
////    tabuTest();
////    testOnAllFiles();
////    test();
////    if (argc < 8) {
////        std::cout << "Proper usage: " << argv[0]
////                  << " <numer_pliku> <pop_size> <generations> <cross_prob> <mut_prob> <tournament_size> <crossover_operator> <mutation_operator>\n";
////        return 1;
////    }
//
////    if (argc > 1) {
////        int fileNumber = std::stoi(argv[1]);
////        int popSize = std::stoi(argv[2]);
////        int generations = std::stoi(argv[3]);
////        std::string crossoverOperator = argv[4];
////        double crossProb = std::stod(argv[5]);
////        std::string mutationOperator = argv[6];
////        double mutProb = std::stod(argv[7]);
////        int tournamentSize = std::stoi(argv[8]);
////        int iterations = std::stoi(argv[9]);
////        Problem problem("../problems/" + problemFiles[fileNumber]);
////        Config config(popSize, generations, crossProb, mutProb, tournamentSize, crossoverOperator, mutationOperator);
////        std::vector<double> bestValues;
////        double bestOverall = std::numeric_limits<double>::max();
////        double worstOverall = std::numeric_limits<double>::lowest();
////        double totalFitness = 0.0;
////
//////    auto startTime = std::chrono::high_resolution_clock::now();
////
////        for (int run = 0; run < iterations; ++run) {
//////        config.seed = std::chrono::system_clock::now().time_since_epoch().count();
////            EvolutionaryAlgorithm ea(config, problem);
////            Individual bestSolution = ea.run();
////
////            if (bestSolution.fitness < bestOverall) {
////                bestOverall = bestSolution.fitness;
////            }
////            if (bestSolution.fitness > worstOverall) {
////                worstOverall = bestSolution.fitness;
////            }
////            totalFitness += bestSolution.fitness;
////            bestValues.push_back(bestSolution.fitness);
////            // std::cout<<bestSolution.fitness;
////        }
////
//////    auto endTime = std::chrono::high_resolution_clock::now();
//////    std::chrono::duration<double> elapsed = endTime - startTime;
////
////        double avgFitness = totalFitness / bestValues.size();
////        double stdDev = std::sqrt(std::accumulate(bestValues.begin(), bestValues.end(), 0.0,
////                                                  [avgFitness](double sum, double val) {
////                                                      return sum +
////                                                             (val - avgFitness) *
////                                                             (val - avgFitness);
////                                                  }) / bestValues.size());
////
////        std::ostringstream filePath;
////        filePath << "../summaries"
////                 << problemFiles[fileNumber] << generations << "summary.csv";
////
////        bool fileExists = std::filesystem::exists(filePath.str());
////
////        std::ofstream summaryFile(filePath.str(), std::ios::app);
////
////        if (!fileExists) {
////            summaryFile << "Best,Worst,Avg,Std,crossOperator,crossProb,MutOperator,MutProb, popSize,tourSize\n";
////        }
////        summaryFile << bestOverall << "," << worstOverall
////                    << "," << avgFitness << "," << stdDev << "," << crossoverOperator << "," << crossProb << ","
////                    << mutationOperator << "," << mutProb << "," << popSize << "," << tournamentSize << "\n";
////        summaryFile.close();
////        return avgFitness;
////    }
//
//
//
//
//
//    //    Logger logGreedy("../summaries/Greedy.csv", "best,worst,avg,std");
////
////    for(auto & file: problemFiles) {
////        Problem problem("../problems/"+file);
////        problem.evaluateGreedyFitnessForAll();
////        std::cout<<file<<std::endl;
////        logGreedy.logStats(problem.evaluateGreedyFitnessForAll());
////    }
////    Logger logRand("../summaries/Random100000.csv", "best,worst,avg,std");
////    for(auto & file: problemFiles) {
////        Problem problem("../problems/"+file);
////        std::cout<<file<<std::endl;
////        logRand.logStats(problem.evaluateRandomIndividuals(100000));
////    }
//
//
//
//}

