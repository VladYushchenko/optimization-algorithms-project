#ifndef SOLVER_H
#define SOLVER_H

#include <functional>
#include <list>
#include <random>
#include "neighborhood.h"

template <typename T>
class iSolver
{

public:
    static iSolver* makeSolver(int choice);
    virtual ~iSolver() {}
    virtual std::list<T> solve() = 0;
    virtual double targetFunction(T solution) = 0;
};

template <typename T>
class LocalSearchSolver: public iSolver<T>{
private:
    T _solution;
    //std::function<double(T)> _targetFunction;
    //std::vector<T> _neighborhood;
    //void updateNeighborhood(T currentSolution);
public:
    LocalSearchSolver();
    LocalSearchSolver(T startSolution);
    ~LocalSearchSolver();
    std::list<T> solve();
    double targetFunction(T solution);
};

template <typename T>
class SimulatedAnnealingSolver: public iSolver<T>{
private:
    T _solution;
    std::mt19937 _gen;
    double _temperature;
    //std::function<double(T)> _targetFunction;
    //std::vector<T> _neighborhood;
    //void updateNeighborhood(T currentSolution);
    T chooseRandomNeighbor(std::vector<T> neighborhood);
    bool makeProbabilisticDesision(double optimalValue, double candidateValue);
public:
    SimulatedAnnealingSolver();
    SimulatedAnnealingSolver(T startSolution);
    ~SimulatedAnnealingSolver();
    std::list<T> solve();
    double targetFunction(T solution);
};

template <typename T>
class TabuSearchSolver: public iSolver<T>{
private:
    T _solution;
    //std::function<double(T)> _targetFunction;
    //std::vector<T> _neighborhood;
    //std::list<T> _tabuList;
public:
    TabuSearchSolver();
    TabuSearchSolver(T startSolution);
    ~TabuSearchSolver();
    std::list<T> solve();
    double targetFunction(T solution);
};

#endif // SOLVER_H
