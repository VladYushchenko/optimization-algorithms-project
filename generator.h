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
    std::vector<Rectangle> _data;
    unsigned int _dim;
public:
    Generator();
    Generator(const unsigned int instanceNumber, const unsigned int lowerLengthLimit,
              const unsigned int upperLengthLimit, const unsigned int dim = 2);
    ~Generator();

    void generateData();
    void generateData2(unsigned int boxLength);
    std::vector<Rectangle> getGeneratedData();
};
#endif // GENERETAROR_H
