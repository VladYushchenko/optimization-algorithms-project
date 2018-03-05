#ifndef OPTIMIZATIONPROBLEM_H
#define OPTIMIZATIONPROBLEM_H

#pragma once
#include "solver.h"
#include "generator.h"

class OptimizationProblem
{
private:
    std::vector<Box> _startSolution;
    std::list<std::vector<Box>> _solutionHistory;
    unsigned int _rectangleCount;
    unsigned int _boxLength;
    unsigned int _minSideLength;
    unsigned int _maxSideLength;

public:
    OptimizationProblem();
    OptimizationProblem(unsigned int rectangleCount, unsigned int boxLength, unsigned int minSideLength, unsigned int maxSideLength);
    OptimizationProblem(const OptimizationProblem& obj);
    ~OptimizationProblem();
    void solveOptimizationProblem(int choice);
    std::list<std::vector<Box>> getSolutionHistory();
    std::vector<Box> getStartSolution();

private:
    double targetFunction(std::vector<Box> solution);

};

#endif // OPTIMIZATIONPROBLEM_H
