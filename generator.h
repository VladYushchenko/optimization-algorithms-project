#ifndef GENERETAROR_H
#define GENERETAROR_H

#include <iostream>
#include <stdexcept>
#include <random>
#include <cmath>
#include "neighborhood.h"

class Generator {
private:
    unsigned int _lowerBound;
    unsigned int _upperBound;
    unsigned int _instanceNumber;
	unsigned int _boxLength;
    std::vector<Rectangle> _data;
    unsigned int _dim;
public:
    Generator();
    Generator(const unsigned int instanceNumber, const unsigned int boxLength,
		const unsigned int lowerLengthLimit, const unsigned int upperLengthLimit, const unsigned int dim = 2);
    ~Generator();

	void generateData(int option = 1);
	void generateRectangles();
    void generateRectanglesFromBox();
    std::vector<Rectangle> getGeneratedData();
};
#endif // GENERETAROR_H
