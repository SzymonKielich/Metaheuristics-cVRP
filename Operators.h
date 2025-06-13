//
// Created by szymo on 12/10/2024.
//
#ifndef METAHEURYSTYKI_CVRP_OPERATORS_H
#define METAHEURYSTYKI_CVRP_OPERATORS_H
#include "Individual.h"
#include "Problem.h"
#include <random>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <unordered_set>
class Operators {
public:
    static Individual crossPMX(const Individual &parent1, const Individual &parent2, std::mt19937 &rng) {
        int size = parent1.route.size();
        std::uniform_int_distribution<> dist(0, size - 1);
        int start = dist(rng);
        int end = dist(rng);
        while (start == end) {
            end = dist(rng);
        }
        if (start > end) std::swap(start, end);

        // std::cout<<"st end: "<<start<<", "<<end<<std::endl;

        std::vector<int> child(size, -1);
        std::unordered_map<int, int> mapping;

        for (int i = start; i <= end; ++i) {
            child[i] = parent1.route[i];
            mapping[parent1.route[i]] = parent2.route[i];
        }

        for (int i = 0; i < size; ++i) {
            if (i >= start && i <= end) continue;
            int city = parent2.route[i];
            while (mapping.find(city) != mapping.end()) {
                city = mapping[city];
            }
            child[i] = city;
        }
        return Individual(std::move(child), 0);
    }

    static Individual crossCX(const Individual &parent1, const Individual &parent2) {
        int size = parent1.route.size();
        std::vector<int> child1(size, -1);

        std::vector<bool> visited(size, false);
        int index = 0;
        while (!visited[index]) {
            child1[index] = parent1.route[index];
            visited[index] = true;
            index = std::distance(parent1.route.begin(),
                                  std::find(parent1.route.begin(), parent1.route.end(), parent2.route[index]));
            // std::cout<<"indexx: "<<index<<std::endl;

        }
        for (int i = 0; i < size; ++i) {
            if (!visited[i]) {
                child1[i] = parent2.route[i];
            }
        }


        return Individual(std::move(child1), 0);
    }

    static Individual crossOX(const Individual &parent1, const Individual &parent2, std::mt19937 &rng) {

        int size = parent1.route.size();
        std::uniform_int_distribution<> dist(0, size - 1);
        int start = dist(rng);
        int end = dist(rng);
//     std::cout<<"st end: "<<start<<", "<<end<<std::endl;
        if (start > end) std::swap(start, end);

        std::vector<int> child(size, -1);
        std::copy(parent1.route.begin() + start, parent1.route.begin() + end + 1, child.begin() + start);

        int current = 0;
        for (int i = 0; i < size; ++i) {
            int city = parent2.route[current];

            while (std::find(child.begin(), child.end(), city) != child.end()) {
                current = current + 1;
                city = parent2.route[current];
            }
            if (child[i] == -1) {
                child[i] = city;

            }


        }
//    for(auto & elem: child) {
//         std::cout<<elem<<",,, ";
//    }
        return Individual(std::move(child), 0);
    }

    static void mutationSwapIdx(int idx, Individual &ind, std::mt19937 &rng) {
        int size = ind.route.size();
        std::uniform_int_distribution<> dist(0, size - 1);
        int i = idx;
        int j = dist(rng);
        std::swap(ind.route[i], ind.route[j]);
    }

    static void mutationSwapOneRandom(Individual &ind, std::mt19937 &rng) {
        int size = ind.route.size();
        std::uniform_int_distribution<> dist(0, size - 1);
        int i = dist(rng);
        int j = dist(rng);
        std::swap(ind.route[i], ind.route[j]);

    }

    static void mutationInverse(Individual &ind, std::mt19937 &rng) {
        int size = ind.route.size();
        std::uniform_int_distribution<> dist(0, size - 1);
        int i = dist(rng);
        int j = dist(rng);
        if (i > j) std::swap(i, j);
        // std::cout<<"fromto"<<i<<", "<<j<<std::endl;
        std::reverse(ind.route.begin() + i, ind.route.begin() + j + 1);
    }

    static Individual tourSelection(const std::vector<Individual> &population, int tournamentSizePercent, std::mt19937 &rng) {
        std::uniform_int_distribution<> dist(0, population.size() - 1);
        Individual best = population[dist(rng)];
        double tournamentSize = (population.size() * tournamentSizePercent) / 100.0;
        int zaokraglony_wynik = static_cast<int>(std::ceil(tournamentSize));

        for (int i = 1; i < tournamentSize; ++i) {
            Individual contender = population[dist(rng)];
            if (contender < best) {
                best = contender;
            }
        }
        return best;
    }

//Individual rouletteScaledSelection(const std::vector<Individual>& population, std::mt19937& rng) {
////todo przeskalowac
//
//    double totalFitness = 0.0;
//    for (const auto& ind : population) {
//        totalFitness += 1.0 / ind.fitness;
//    }
//
//    std::uniform_real_distribution<> dist(0.0, totalFitness);
//    double randValue = dist(rng);
//    double cumulative = 0.0;
//    for (const auto& ind : population) {
//        cumulative += 1.0 / ind.fitness;
//        if (cumulative >= randValue) {
//            return ind;
//        }
//    }
//    return population.back();
//}
    static Individual rouletteScaledSelection(const std::vector<Individual> &population, std::mt19937 &rng) {
        int maxFitness = std::numeric_limits<int>::min();
        for (const auto &ind: population) {
            if (ind.fitness > maxFitness) {
                maxFitness = ind.fitness;
            }
        }

        double totalScaledFitness = 0.0;
        for (const auto &ind: population) {
            totalScaledFitness += (maxFitness - ind.fitness);
        }

        std::uniform_real_distribution<> dist(0.0, totalScaledFitness);
        double randValue = dist(rng);
        double cumulative = 0.0;

        for (const auto &ind: population) {
            cumulative += (maxFitness - ind.fitness);
            if (cumulative >= randValue) {
                return ind;
            }
        }

        return population.back();
    }
};

static std::pair<Individual, Individual> crossPositionBased(const Individual& parent1, const Individual& parent2, std::mt19937& rng) {
    int size = parent1.route.size();
    std::uniform_int_distribution<> dist(0, size - 1);
    int numPositions = dist(rng) % size + 1;

    std::unordered_set<int> positions;
    while (positions.size() < numPositions) {
        positions.insert(dist(rng));
    }

    std::vector<int> offspring1(size, -1);
    std::vector<int> offspring2(size, -1);

    for (int pos : positions) {
        offspring1[pos] = parent1.route[pos];
        offspring2[pos] = parent2.route[pos];
    }

    auto fillOffspring = [](const std::vector<int>& parent, std::vector<int>& offspring) {
        int idx = 0;
        for (int gene : parent) {
            if (std::find(offspring.begin(), offspring.end(), gene) == offspring.end()) {
                while (offspring[idx] != -1) idx++;
                offspring[idx] = gene;
            }
        }
    };

    fillOffspring(parent2.route, offspring1);
    fillOffspring(parent1.route, offspring2);

    return {Individual(offspring1, 0), Individual(offspring2, 0)};
}

static void mutationScramble(Individual& ind, std::mt19937& rng) {
    int size = ind.route.size();
    if (size <= 1) return;
    std::uniform_int_distribution<> dist(0, size - 1);
    int i = dist(rng);
    int j = dist(rng);
    if (i > j) std::swap(i, j);
    std::shuffle(ind.route.begin() + i, ind.route.begin() + j + 1, rng);
}

static void mutationInsertion(Individual& ind, std::mt19937& rng) {
    int size = ind.route.size();
    if (size <= 1) return;
    std::uniform_int_distribution<> dist(0, size - 1);
    int from = dist(rng);
    int to = dist(rng);
    while (from == to) {
        to = dist(rng);
    }
    int city = ind.route[from];
    ind.route.erase(ind.route.begin() + from);
    ind.route.insert(ind.route.begin() + to, city);
}

#endif // METAHEURYSTYKI_CVRP_OPERATORS_H
