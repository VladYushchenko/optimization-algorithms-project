#include "generator.h"


// Generator implementation

Generator::Generator(){
    _instanceNumber = 0;
    _lowerBound = 0;
    _upperBound = 0;
}

Generator::Generator(const unsigned int instanceNumber, const unsigned int lowerLengthLimit,
                     const unsigned int upperLengthLimit, const unsigned int dim){

    _instanceNumber = instanceNumber;
    _dim = dim;
    if (lowerLengthLimit < upperLengthLimit){
    _lowerBound = lowerLengthLimit;
    _upperBound = upperLengthLimit;
    }
    else {
        _lowerBound = upperLengthLimit;
        _upperBound = lowerLengthLimit;
    }

}

Generator::~Generator(){

}

void Generator::generateData(){
    /*if (!std::is_unsigned<size_t>::value)
        throw std::runtime_error("Wrong generator type. Should be unsigned");
    else{*/
        std::random_device rd;
        // seed generator
        std::mt19937 rng(rd());
        std::uniform_int_distribution<unsigned int> uniform(_lowerBound, _upperBound);

        for (size_t id = 0; id < _instanceNumber; id++) {
            //create 1 rectangle and push it to data
            Rectangle tempRectangle(id, uniform(rng), uniform(rng));
            _data.push_back(tempRectangle);
        }

}


void Generator::generateData2(unsigned int boxLength){
    std::vector<Rectangle> tempData = std::vector<Rectangle>(1, Rectangle(0, boxLength, boxLength));
    std::random_device rd;
    // seed generator
    std::mt19937 rng(rd());

    for (size_t i = 0; i < _instanceNumber; i++){
        // choose number of rectangle to perform split
        std::uniform_int_distribution<unsigned int> uniform(0, tempData.size() - 1);

        unsigned int rectangleNumber = uniform(rng);

        Rectangle tempRectangle = tempData[rectangleNumber];
        // create weights for choosing side of rectangle
        std::vector<unsigned int> weights = tempRectangle.getSideLengths();
        // normalize weights
        //weights /= std::accumulate(v.begin(), v.end(), 0);
            /*for (auto i = 0; i < tempData[rectangleNumber].getDimension(); i++) {
                weights.push_back(tempData[rectangleNumber].getSideLengths()[i]);
            }*/
        std::discrete_distribution<int> disc(weights.begin(), weights.end());
        int sideNumber = disc(rng);

        size_t sideLength = tempData[rectangleNumber].getSideLengths()[sideNumber];
        if (sideLength == 1)
            continue;

        std::uniform_int_distribution<unsigned int> sideSplitUniform (1, sideLength-1);
        unsigned int splitLength = sideSplitUniform(rng);

        //splitRectangle(tempData[rectangleNumber], sideNumber, splitLength);
        Rectangle rightRectangle = tempRectangle;
        Rectangle leftRectangle = tempRectangle;

        // for one rectangle start point (upper left corner) stays same
        // modify length only
        leftRectangle.modifySideLength(sideNumber, splitLength);
        // change start point and length
        rightRectangle.modifyStartPoint(sideNumber, tempRectangle.getStartPoint()[sideNumber] + splitLength);
        rightRectangle.modifySideLength(sideNumber, tempRectangle.getSideLengths()[sideNumber] - splitLength);

        tempData.erase(tempData.begin() + rectangleNumber);
        tempData.insert(tempData.end(), {leftRectangle, rightRectangle});
    }
}

std::vector<Rectangle> Generator::getGeneratedData()
{
    return _data;
}
