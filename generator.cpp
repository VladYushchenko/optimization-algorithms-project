#include "generator.h"
#include <unordered_map>
#include <algorithm>


/// Generator implementation

Generator::Generator(){
    _instanceNumber = 0;
    _lowerBound = 0;
    _upperBound = 0;
}

Generator::Generator(const unsigned int instanceNumber, const unsigned int boxLength, 
	const unsigned int lowerLengthLimit, const unsigned int upperLengthLimit, const unsigned int dim){

    _instanceNumber = instanceNumber;
    _dim = dim;
	_boxLength = boxLength;

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

void Generator::generateData(int option)
{
	switch (option)
	{
	case 0:
		generateRectangles();
		break;
	case 1:
		generateRectanglesFromBox();
		break;
	default:
		generateRectangles();
		break;
	}
}

void Generator::generateRectangles()
{
    std::random_device rd;
    // seed generator
    std::mt19937 rng(rd());
    std::uniform_int_distribution<unsigned int> uniform(_lowerBound, _upperBound);

    for (size_t id = 0; id < _instanceNumber; id++) {
        //create rectangle and push it to data
        Rectangle tempRectangle(id, uniform(rng), uniform(rng));
        _data.push_back(tempRectangle);
    }
}


void Generator::generateRectanglesFromBox(){
	std::unordered_map<unsigned int, Rectangle> tempData = { { 0u, Rectangle(0, _boxLength, _boxLength) } };
	unsigned int maxKey = 0;
	// seed generator
    std::random_device rd;
    std::mt19937 rng(rd());

    for (size_t i = 0; i < _instanceNumber; i++)
	{	
		std::vector<unsigned int> map_keys;
		for (auto const & item : tempData)
			map_keys.push_back(item.first);
        // choose number of rectangle to perform split
        std::uniform_int_distribution<unsigned int> uniform(0, map_keys.size() - 1);
        auto map_key_index = uniform(rng);

		unsigned int rectangleNumber = map_keys[map_key_index];
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

        // split rectangle
        Rectangle rightRectangle = tempRectangle;
        Rectangle leftRectangle = tempRectangle;

        // left rectangle -> origin (upper left corner) stays same, modify length only
        leftRectangle.modifySideLength(sideNumber, splitLength);
        // modify right
        rightRectangle.modifyStartPoint(sideNumber, tempRectangle.getStartPoint()[sideNumber] + splitLength);
        rightRectangle.modifySideLength(sideNumber, tempRectangle.getSideLengths()[sideNumber] - splitLength);

		//update map
        tempData.erase(rectangleNumber);
		tempData[maxKey + 1] = leftRectangle;
		tempData[maxKey + 2] = rightRectangle;
		maxKey += 2;
    }
	// update data
	for (auto const & item : tempData)
		_data.push_back(item.second);
}

std::vector<Rectangle> Generator::getGeneratedData()
{
    return _data;
}
