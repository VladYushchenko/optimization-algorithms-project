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
        auto instanceCount = atoi(argv[1]);
		auto rectangleCount = atoi(argv[2]);
		auto minSideLength = atoi(argv[3]);
		auto maxSideLength = atoi(argv[4]);
		auto boxLength = atoi(argv[5]);
		auto genType = 0;

        std::clock_t c_start;
        std::clock_t c_end;

        for (auto i = 0; i < instanceCount; i++){
            for (int j = 0; j < 3; j++)
            {

                c_start = std::clock();

                OptimizationProblem problem(rectangleCount, boxLength, minSideLength, maxSideLength, genType);
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
