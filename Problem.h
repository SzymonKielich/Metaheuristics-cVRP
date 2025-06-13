//
// Created by szymo on 06/10/2024.
//

#ifndef METAHEURYSTYKI_CVRP_PROBLEM_H
#define METAHEURYSTYKI_CVRP_PROBLEM_H
#include <vector>
#include <string>
#include <unordered_map>

class Problem {
public:
    Problem(const std::string& filename);
    int eval(const std::vector<int>&route) const;
    void loadFromFile( const std::string& filename);
//    int evaluateRoute(const std::vector<int>& route) const;
    void printRoute(const std::vector<int> &route) const;

    void printProblem() const;
    std::tuple<int, int, double, double> evaluateGreedyFitnessForAll();
    std::tuple<int, int, double, double> evaluateRandomIndividuals(int x);
    std::vector<int> greedyAlgorithm(int startFrom);

    bool isRouteValid(const std::vector<int>& route);
    int getDistance(int from, int to) const { return distances[from][to]; }
    int getDemand(int city) const { return demands[city]; }

    int getDimension() const { return dimension; }
    int getCapacity() const { return capacity; }
    int getDepot() const { return depot; }

    std::vector<std::pair<int, int>> coordinates;


    std::vector<int> getBestGreedy();
//    mutable std::unordered_map<size_t, int> evaluatedIndividuals;

private:

    std::vector<std::vector<int>> distances;
    std::vector<int> demands;
    int capacity;
    int dimension;
    int depot;


};


#endif //METAHEURYSTYKI_CVRP_PROBLEM_H
