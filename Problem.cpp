//
// Created by szymo on 06/10/2024.
//

#include "Problem.h"
#include "Individual.h"
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <set>

Problem::Problem(const std::string &filename) {
//    std::cout<<"Zczytuje definicje problemu z pliku: "<<filename<<"bbb"<<std::endl;
    loadFromFile(filename);
}

void Problem::loadFromFile(const std::string &filename) {

    std::ifstream file(filename);
//    std::cout<<"Zczytuje definicje problemu z pliku: "<<filename<<std::endl;

    if (file.is_open()) {

        std::string line;
        while (std::getline(file, line)) {
            if (line.find("DIMENSION") != -1) {
                dimension = std::stoi(line.substr(line.find(":") + 1));
                // resize wektora 2d
                distances.resize(dimension, std::vector<int>(dimension));
                coordinates.resize(dimension);
                demands.resize(dimension);
            } else if (line.find("CAPACITY") != -1) {
                capacity = std::stoi(line.substr(line.find(":") + 1));
            } else if (line.find("NODE_COORD_SECTION") != -1) {

                for (int i = 0; i < dimension; i++) {
                    int id, x, y;

                    file >> id >> x >> y;
                    coordinates[id - 1] = {x, y};

                }
                for (int i = 0; i < dimension; i++) {
                    for (int j = 0; j < dimension; j++) {
                        int dx = coordinates[i].first - coordinates[j].first;
                        int dy = coordinates[i].second - coordinates[j].second;
                        distances[i][j] = (int) ((sqrt(dx * dx + dy * dy)) + 0.5);
                    }
                }
            } else if (line.find("DEMAND_SECTION") != -1) {
                for (int i = 0; i < dimension; i++) {
                    int id, demand;
                    file >> id >> demand;
                    demands[id - 1] = demand;
                }
            } else if (line.find("DEPOT_SECTION") != -1) {
                std::getline(file, line);
                depot = std::stoi(line) - 1;
            }
        }
    } else {
        throw std::invalid_argument("Could not open the file");
    }

}

void Problem::printProblem() const {
    std::cout << "Dimension: " << dimension << std::endl;
    std::cout << "Capacity: " << capacity << std::endl;

    std::cout << "Cords: " << std::endl;
    for (int i = 0; i < dimension; i++) {
        std::cout << "Node " << i + 1 << ":" << coordinates[i].first << ", " << coordinates[i].second << std::endl;
    }

    std::cout << "Distances: " << std::endl;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            std::cout << distances[i][j] << " ";
        }
        std::cout << "" << std::endl;
    }

    std::cout << "Demands: " << std::endl;
    for (int i = 0; i < dimension; i++) {
        std::cout << "Node " << i + 1 << ": " << demands[i] << std::endl;
    }

    std::cout << "Depot: " << depot + 1 << std::endl;
}

// depot nie uwzgledniony na trasie (podobnie jak w plikach testowych)
int Problem::eval(const std::vector<int> &route) const {
//    Individual tempIndividual(route, 0);
//    size_t hashCode = tempIndividual.getHashCode();
//
//    auto it = evaluatedIndividuals.find(hashCode);
//    if (it != evaluatedIndividuals.end()) {
//        return it->second;
//    }
    int totalCost = 0;
    int currentLoad = 0;
    int currentCity = depot;
    for (int nextCity: route) {
        if (currentLoad + demands[nextCity] > capacity) {
            totalCost += distances[currentCity][depot];
            totalCost += distances[depot][nextCity];
            currentLoad = demands[nextCity];
        } else {
            totalCost += distances[currentCity][nextCity];
            currentLoad += demands[nextCity];
        }
        currentCity = nextCity;

    }
    totalCost += distances[currentCity][depot];
    return totalCost;
}

void Problem::printRoute(const std::vector<int> &route) const {
    std::cout << "Route: ";
    for (int city: route) {
        std::cout << city + 1 << " ";
    }
    std::cout << std::endl;
}

std::tuple<int, int, double, double> Problem::evaluateRandomIndividuals(int x) {
    std::vector<int> fitnesses;
    std::vector<int> bestRoute;
    int bestFitness = INT_MAX;
    int worstFitness = INT_MIN;
    double sumFitness = 0;

    for (int i = 0; i < x; i++) {
        Individual individual;
        individual.generateRandomRoute(*this);

        int fitness = individual.getFitness();
        fitnesses.push_back(fitness);
        sumFitness += fitness;

        if (fitness < bestFitness) {
            bestFitness = fitness;
            bestRoute = individual.getRouteCopy();
        }

        if (fitness > worstFitness) {
            worstFitness = fitness;
        }

        // std::cout << "Losowe rozwiązanie " << i + 1 << " koszt: " << fitness << std::endl;
    }

    double averageFitness = sumFitness / fitnesses.size();
    double variance = 0;
    for (int fitness: fitnesses) {
        variance += std::pow(fitness - averageFitness, 2);
    }
    double stdDeviation = std::sqrt(variance / fitnesses.size());


    return std::make_tuple(bestFitness, worstFitness, averageFitness, stdDeviation);


}

std::vector<int> Problem::greedyAlgorithm(int startFrom) {
    std::vector<bool> visited;
    visited.resize(dimension);
    for (bool elem: visited) {
        elem = false;
    }
    visited[depot] = true; // nie dodawany do trasy
    int currentCity = startFrom;
    int currentLoad = demands[currentCity];
    visited[currentCity] = true;
    std::vector<int> route;
    route.push_back(currentCity);
    while (route.size() < dimension - 1) { // -1 bo trasa bez depot
        int closestCity = -1;
        int smallestDistance = INT_MAX;
        for (int city = 0; city < dimension; city++) { //zmienic flage false na usuwanie odwiedzonych z wektora?
            if (!(visited[city])) {
                if (currentLoad + demands[city] <= capacity) {
                    int distance = distances[currentCity][city];
                    if (distance < smallestDistance) {
                        smallestDistance = distance;
                        closestCity = city;
                    }
                }
            }
        }
        if (closestCity == -1) {
            currentCity = depot;
            currentLoad = 0;
            //najblizsze miasto
            for (int city = 0; city < dimension; city++) {
                if (!visited[city]) {
                    int distance = distances[depot][city];
                    if (distance < smallestDistance) {
                        smallestDistance = distance;
                        closestCity = city;
                    }
                }
            }
        }
        currentCity = closestCity;
        if (currentCity == depot) {
            currentLoad = 0;
        } else {
            currentLoad += demands[currentCity];
            visited[currentCity] = true;
            route.push_back(currentCity);
        }

    }

    return route;
}

std::vector<int> Problem::getBestGreedy() {

    int bestFitness = INT_MAX;
    std::vector<int> bestRoute;
    std::vector<int> route;
    int currentFitness = INT_MAX;
    for (int i = 0; i < dimension; i++) {
        if (i == depot) continue;
        route = greedyAlgorithm(i);
        currentFitness = eval(route);
        if (currentFitness < bestFitness) {
            bestFitness = currentFitness;
            bestRoute = route;
        }

    }
    return bestRoute;
}

std::tuple<int, int, double, double> Problem::evaluateGreedyFitnessForAll() {
    std::vector<int> fitnesses;
    int bestFitness = INT_MAX;
    int worstFitness = INT_MIN;
    double sumFitness = 0;

    for (int i = 0; i < dimension; i++) {
        if (i == depot) continue;
        std::vector<int> route = greedyAlgorithm(i);
        int fitness = eval(route);
        fitnesses.push_back(fitness);
        sumFitness += fitness;

        if (fitness < bestFitness) bestFitness = fitness;
        if (fitness > worstFitness) worstFitness = fitness;

//        std::cout << "Greedy z miasta " << i + 1 << ", fitness: " << fitness << std::endl;
    }

    double averageFitness = sumFitness / fitnesses.size();
    double variance = 0;
    for (int fitness: fitnesses) {
        variance += std::pow(fitness - averageFitness, 2);
    }
    double stdDeviation = std::sqrt(variance / fitnesses.size());
    std::cout << bestFitness << worstFitness << std::endl;
    return std::make_tuple(bestFitness, worstFitness, averageFitness, stdDeviation);
}



