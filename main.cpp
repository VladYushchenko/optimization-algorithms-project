#include "widget.h"
#include <QApplication>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <optimizationproblem.h>

int main(int argc, char *argv[])
{
    std::cout << "Have " << argc << " arguments:" << std::endl;
        for (int i = 0; i < argc; ++i) {
            std::cout << argv[i] << std::endl;
        }
    if (argc == 6){
        size_t instanceCount = atoi(argv[1]);
        size_t rectangleCount = atoi(argv[2]);
        size_t minSideLength = atoi(argv[3]);
        size_t maxSideLength = atoi(argv[4]);
        size_t boxLength = atoi(argv[5]);


        std::clock_t c_start;
        std::clock_t c_end;

        for (size_t i = 0; i < instanceCount; i++){
            for (int j = 0; j < 3; j++)
            {

                c_start = std::clock();

                OptimizationProblem problem(rectangleCount, boxLength, minSideLength, maxSideLength);
                problem.solveOptimizationProblem(j);
                std::cout << "Solved with #boxes: " << problem.getStartSolution().size() << std::endl;

                c_end = std::clock();


                std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
                         << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms\n";
            }
        }

    }
    else
    {
        QApplication a(argc, argv);
        Widget w;
        w.show();

        return a.exec();
    }
	return 0;
}
