#include "optimizationproblem.h"

OptimizationProblem::OptimizationProblem()
{
}

OptimizationProblem::OptimizationProblem(unsigned int rectangleCount, unsigned int boxLength, unsigned int minSideLength, unsigned int maxSideLength)
{
    _rectangleCount = rectangleCount;
    _boxLength = boxLength;
    _minSideLength = minSideLength;
    _maxSideLength = maxSideLength;

    Generator gen(_rectangleCount, _boxLength, _minSideLength, _maxSideLength);
    gen.generateData();
    std::vector<Rectangle> rects = gen.getGeneratedData();

    for (size_t i = 0; i < rects.size(); i++)
        _startSolution.push_back(Box(std::vector<Rectangle>(1, rects[i]), i + 1, _boxLength));
}

OptimizationProblem::OptimizationProblem(const OptimizationProblem &obj)
{
    _rectangleCount = obj._rectangleCount;
    _boxLength = obj._boxLength;
    _minSideLength = obj._minSideLength;
    _maxSideLength = obj._maxSideLength;
    _startSolution = obj._startSolution;
    _solutionHistory = obj._solutionHistory;
}



OptimizationProblem::~OptimizationProblem()
{

}

void OptimizationProblem::solveOptimizationProblem(int choice)
{
    iSolver<std::vector<Box>>* solver;

    switch (choice) {
        case 0: {
            solver = new LocalSearchSolver<std::vector<Box>> (_startSolution);
            std::cout << "Started Local Search..." << std::endl;
            break;
        }
        case 1: {
            solver = new SimulatedAnnealingSolver<std::vector<Box>> (_startSolution);
            std::cout << "Started Simulated Annealing..." << std::endl;
            break;
        }
        case 2: {
            solver = new TabuSearchSolver<std::vector<Box>> (_startSolution);
            std::cout << "Started Tabu Search..." << std::endl;
            break;
        }
        default:{
            solver = new LocalSearchSolver<std::vector<Box>> (_startSolution);
            std::cout << "Started Local Search..." << std::endl;
            break;
        }
    }
    _solutionHistory = solver->solve();
}

std::list<std::vector<Box> > OptimizationProblem::getSolutionHistory()
{
    return _solutionHistory;
}

std::vector<Box> OptimizationProblem::getStartSolution()
{
    return _startSolution;
}

double OptimizationProblem::targetFunction(std::vector<Box> solution)
{
    // TODO: move outside this class
    double binWeight = 0;
    for (const Box & item : solution)
        binWeight += item._placedRectanglesArea * item._placedRectanglesArea
                - item._overlappedRectanglesArea * item._boxHeigth * item._boxWidth;

    return binWeight;
}





