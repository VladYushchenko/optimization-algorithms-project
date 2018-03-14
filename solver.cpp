#include "solver.h"
#include <numeric>

template<typename T>
iSolver<T>* iSolver<T>::makeSolver(int choice)
{
    if (choice == 1)
        return new SimulatedAnnealingSolver<T>;
      else if (choice == 2)
        return new TabuSearchSolver<T>;
      else
        return new LocalSearchSolver<T>;
}


///
/// Local Search Class
///
template<typename T>
LocalSearchSolver<T>::LocalSearchSolver()
{

}

template<typename T>
LocalSearchSolver<T>::LocalSearchSolver( T startSolution){
    _solution = startSolution;
    //_targetFunction = targetFunction;
}

template<typename T>
LocalSearchSolver<T>::~LocalSearchSolver()
{

}

template<typename T>
std::list<T> LocalSearchSolver<T>::solve(){
    //int optionCase = 0;
    bool isOptimumFound = false;
    bool isBetterFound = false;
    T currentSolution = _solution;
    std::list<T> solutionHistory;
    solutionHistory.push_back(currentSolution);

    T betterSolution;
    Neighborhood<T> currentNeighborhood;

    while (!isOptimumFound)
    {
        isBetterFound = false;
        currentNeighborhood.update(currentSolution, 0);

        std::vector<T> currentNeighbors = currentNeighborhood.getNeighbors();

        for (auto it = currentNeighbors.begin(); it != currentNeighbors.end(); it++)
        {

            if (targetFunction(currentSolution) < targetFunction(*it))
            {
                isBetterFound = true;
                betterSolution = *it;
            }
        }

        if (isBetterFound){
            currentSolution = betterSolution;
            solutionHistory.push_back(currentSolution);
        }
        else
            isOptimumFound = true;
    }
    _solution = currentSolution;
    return solutionHistory;

}

template<typename T>
double LocalSearchSolver<T>::targetFunction(T solution)
{
    // TODO: move outside this class
    double binWeight = 0;
    for (const Box & item : solution)
        binWeight += item._placedRectanglesArea * item._placedRectanglesArea
                - item._overlappedRectanglesArea * item._boxHeigth * item._boxWidth;

    return binWeight;
}



///
/// Simulated Annealing Class
///
template<typename T>
SimulatedAnnealingSolver<T>::SimulatedAnnealingSolver()
{

}

template<typename T>
SimulatedAnnealingSolver<T>::SimulatedAnnealingSolver(T startSolution)
{
    _solution = startSolution;
    std::random_device rd;
    _gen.seed(rd());

    _temperature = std::accumulate(startSolution.begin(), startSolution.end(), 0.0 ,
                   [](double val, decltype(*std::begin(startSolution)) item) {
        return val + item._placedRectanglesArea;
    } );
    std::cout << "Start temperature for Simulated Annealing is: " << _temperature << std::endl;
}

template<typename T>
SimulatedAnnealingSolver<T>::~SimulatedAnnealingSolver()
{

}

template<typename T>
T SimulatedAnnealingSolver<T>::chooseRandomNeighbor(std::vector<T> neighborhood)
{
    return *(neighborhood.begin());
}

template<typename T>
bool SimulatedAnnealingSolver<T>::makeProbabilisticDesision(double optimalValue, double candidateValue)
{
    double probability = exp((candidateValue - optimalValue) / _temperature);
    std::bernoulli_distribution dist(probability);

    return dist(_gen);
}

template<typename T>
std::list<T> SimulatedAnnealingSolver<T>::solve(){
    int optionCase = 0;
    // set termination criteria
    const int NO_IMPROVEMENT_COUNT = 25;
    bool isOptimumFound = false;
    T currentSolution = _solution;
	T nextSolution;

    std::list<T> solutionHistory;
    solutionHistory.push_back(currentSolution);

    Neighborhood<T> currentNeighborhood;
    while (!isOptimumFound)
    {
        currentNeighborhood.update(currentSolution, optionCase);
        std::vector<T> currentNeighbors = currentNeighborhood.getNeighbors();
		
		if (!currentNeighbors.empty()) 
		{
			nextSolution = chooseRandomNeighbor(currentNeighbors);
		}
		else
		{
			return solutionHistory;
		}

        double currentTargetFunction = targetFunction(currentSolution);
        double nextTargetFunction = targetFunction(nextSolution);

        if (currentTargetFunction < nextTargetFunction)
            currentSolution = nextSolution;
        else
        {
            if (makeProbabilisticDesision(currentTargetFunction, nextTargetFunction))
                currentSolution = nextSolution;
        }
        solutionHistory.push_back(currentSolution);
        _temperature *= 0.8;
    }
    _solution = currentSolution;
    return solutionHistory;
}

template<typename T>
double SimulatedAnnealingSolver<T>::targetFunction(T solution)
{
    // TODO: move outside this class
    double binWeight = 0;
    for (const Box & item : solution)
        binWeight += item._placedRectanglesArea * item._placedRectanglesArea
                - item._overlappedRectanglesArea * item._boxHeigth * item._boxWidth;

    return binWeight;
}


//////////////////////////////////////////
/// Tabu Solver Class
///
template<typename T>
TabuSearchSolver<T>::TabuSearchSolver()
{

}

template<typename T>
TabuSearchSolver<T>::TabuSearchSolver(T startSolution)
{
    _solution = startSolution;
    //_targetFunction = targetFunction;
}

template<typename T>
TabuSearchSolver<T>::~TabuSearchSolver()
{

}

template<typename T>
std::list<T> TabuSearchSolver<T>::solve(){
    // set termination criteria
    int optionCase = 0;
    const int NO_IMPROVEMENT_COUNT = 25;
    const int MAX_TABU_SIZE = 10;

    //bool isOptimumFound = false;
    T bestSolution = _solution;
    T currentSolution = _solution;
    std::list<T> tabuList;
    std::list<T> solutionHistory;
    solutionHistory.push_back(currentSolution);

    Neighborhood<T> currentNeighborhood;
    int noImprovementCount = 0;

    while (noImprovementCount < NO_IMPROVEMENT_COUNT)
    {
        currentNeighborhood.update(currentSolution, optionCase);
        std::vector<T> currentNeighbors = currentNeighborhood.getNeighbors();

		if (currentNeighbors.empty())
			return solutionHistory;	

		// find will work?
        for (auto it = currentNeighbors.begin(); it != currentNeighbors.end(); it++){
            if ((std::find(tabuList.begin(),tabuList.end(),*it) == tabuList.end())
                    && (targetFunction(currentSolution) < targetFunction(*it)))
            {
                currentSolution = *it;
            }
        }
        solutionHistory.push_back(currentSolution);

        if (targetFunction(bestSolution) < targetFunction(currentSolution))
        {
            bestSolution = currentSolution;
            noImprovementCount = 0;
        }
        else
            noImprovementCount++;


        if (tabuList.size() > MAX_TABU_SIZE)
            tabuList.pop_front();

        tabuList.push_back(currentSolution);

    }
   _solution = bestSolution;
   return solutionHistory;
}


template<typename T>
double TabuSearchSolver<T>::targetFunction(T solution)
{
    // TODO: move outside this class
    double binWeight = 0;
    for (const Box & item : solution)
        binWeight += item._placedRectanglesArea * item._placedRectanglesArea
                - item._overlappedRectanglesArea * item._boxHeigth * item._boxWidth;

    return binWeight;
}


template class LocalSearchSolver<std::vector<Box>>;
template class TabuSearchSolver<std::vector<Box>>;
template class SimulatedAnnealingSolver<std::vector<Box>>;
