#include "neighborhood.h"


// Rectangle implementation
Rectangle ::Rectangle(){

}

Rectangle::Rectangle(size_t id, unsigned int width, unsigned int height){
    this->id = id;
    this->x = 0;
    this->y = 0;
    this->width = width;
    this->height = height;
	updateArea();

}


Rectangle::Rectangle(const Rectangle& obj){
    this->id = obj.id;
    this->x = obj.x;
    this->y = obj.y;
    this->width = obj.width;
    this->height = obj.height;
    this->area = obj.area;
}

Rectangle::~Rectangle(){

}

void Rectangle::updateArea()
{
	this->area = this->width * this->height;
}

void Rectangle::rotate()
{
    std::swap(this->height, this->width);
}

bool Rectangle::canContain(const Rectangle& rectangle)
{
    return (this->height >= rectangle.height && this->width >= rectangle.width);
}

bool Rectangle::contains(const Rectangle &rectangle)
{
    // check if overlapping is complete
    if ( this->x <= rectangle.x
         && this->y <= rectangle.y
         && this->x + this->width >= rectangle.x
         && this->y + this->width >= rectangle.y)
        return true;
    else
        return false;
}

std::vector<unsigned int> Rectangle::getSideLengths(){
    return {width, height};
}

void Rectangle::modifySideLength(const int position, const unsigned int value){
    if (position == 0)
        width = value;
    else
        height = value;
	updateArea();
}

void Rectangle::modifyStartPoint(const int position, const unsigned int value){
    if (position == 0)
        x = value;
    else
        y = value;
}

std::vector<unsigned int> Rectangle::getStartPoint(){
    return {x, y};
}

/*
*   Box implementation
*/
Box::Box()
{
    Rectangle tempRect(0, 0, 0);

    _containedRectangles.clear();

    _freePlaceForRectangles.clear();
    _freePlaceForRectangles.push_back(tempRect);
}

Box::Box(size_t boxId, int boxSide)
{
    _boxId = boxId;
    _boxHeigth = boxSide;
    _boxWidth = boxSide;

    _containedRectangles.clear();
    _freePlaceForRectangles.clear();
    Rectangle tempRect(0, _boxWidth, _boxHeigth);
    _freePlaceForRectangles.push_back(tempRect);
    _countRectagles = 0;
    _placedRectanglesArea = 0;
    _overlappedRectanglesArea = 0;
    _freeArea = _boxHeigth*_boxWidth;

}

Box::Box(std::vector<Rectangle> rectangles, size_t boxId, int boxSide)
{
    _boxId = boxId;
    _boxHeigth = boxSide;
    _boxWidth = boxSide;

    _containedRectangles.clear();
    _freePlaceForRectangles.clear();
    Rectangle tempRect(0, _boxWidth, _boxHeigth);
    _freePlaceForRectangles.push_back(tempRect);

    //TODO: perform best placement and freePlace management + coveredArea
    _placedRectanglesArea = 0;
    for (auto &rectangle : rectangles){
        this->moveRectangle(rectangle);
    }
    /*
     *TODO calculate overlap possible
     */
    _overlappedRectanglesArea = 0;
}

Box::Box(const Box &box)
{
    _boxHeigth = box._boxHeigth;
    _boxWidth = box._boxWidth;
    _containedRectangles = box._containedRectangles;
    _freePlaceForRectangles = box._freePlaceForRectangles;
    _countRectagles = box._countRectagles;
    _placedRectanglesArea = box._placedRectanglesArea;
    _overlappedRectanglesArea = box._overlappedRectanglesArea;

}

Box::~Box()
{

}

void Box::deleteRectangle(const size_t position)
{
    _placedRectanglesArea -= (*(_containedRectangles.begin() + position)).area;
    _containedRectangles.erase(_containedRectangles.begin() + position);
    _countRectagles--;
}

std::vector<Rectangle> Box::getSortedRectangles()
{
    std::sort(_containedRectangles.begin(), _containedRectangles.end(), [ ]( const Rectangle& lhs, const Rectangle& rhs )
    {
       return lhs.area < rhs.area;
    });
    return _containedRectangles;
}

bool Box::moveRectangle(const Rectangle& rect){
    // Find position to place rectangle
    Rectangle positionedRectangle = rect;
    bool canMoveRectangle = false;

    // Recalculate free place if we can move rectangle
    if (findPositionToPlaceInBox(positionedRectangle)){
        canMoveRectangle = true;
        size_t numRectanglesToProcess = _freePlaceForRectangles.size();
        for(size_t i = 0; i < numRectanglesToProcess; ++i)
        {
            if (splitFreePlace(_freePlaceForRectangles[i], positionedRectangle))
            {
                _freePlaceForRectangles.erase(_freePlaceForRectangles.begin() + i);
                --i;
                --numRectanglesToProcess;
                break;
            }
        }

        PruneFreeList();
        _placedRectanglesArea += positionedRectangle.area;
        _containedRectangles.push_back(positionedRectangle);
        _countRectagles++;
    }
    return canMoveRectangle;
}

bool Box::findPositionToPlaceInBox(Rectangle& rectangle)
{
    Rectangle tempRectangle = rectangle;
    Rectangle tempRotatedRectangle = rectangle;
    tempRotatedRectangle.rotate();

    unsigned int bestX = this->_boxWidth;
    unsigned int bestY = this->_boxHeigth;

    bool wasRectanglePlaced = false;

    for(auto it = _freePlaceForRectangles.begin(); it < _freePlaceForRectangles.end(); it++)
    {
        // Try to place the rectangle or rotated one in upper left corner with preserving OY growth.
        //
        if ((*it).canContain(tempRectangle) )
        {
            unsigned int topSideY = (*it).y + tempRectangle.height;
            unsigned int topSideX = (*it).x + tempRectangle.width;
            if (topSideY < bestY || (topSideY == bestY && topSideX < bestX ))
            {
                rectangle.x = (*it).x;
                rectangle.y = (*it).y;
                rectangle.width = tempRectangle.width;
                rectangle.height = tempRectangle.height;
                bestY = topSideY;
                bestX = topSideX;
                wasRectanglePlaced = true;
            }
        }
        if ((*it).canContain(tempRotatedRectangle))
        {
            unsigned int topSideY = (*it).y + tempRotatedRectangle.height;
            unsigned int topSideX = (*it).x + tempRotatedRectangle.width;
            if (topSideY < bestY || (topSideY == bestY && (*it).x < bestX))
            {
                rectangle.x = (*it).x;
                rectangle.y = (*it).y;
                rectangle.width = tempRotatedRectangle.width;
                rectangle.height = tempRotatedRectangle.height;
                bestY = topSideY;
                bestX = topSideX;
                wasRectanglePlaced = true;
            }
        }
    }
    return wasRectanglePlaced;
}


bool Box::splitFreePlace(Rectangle freePlace, const Rectangle &candidateRectangle)
{
    // Check if the rectangles not intersect at all.
    if (  candidateRectangle.x >= freePlace.x + freePlace.width
       || candidateRectangle.x + candidateRectangle.width <= freePlace.x
       || candidateRectangle.y >= freePlace.y + freePlace.height
       || candidateRectangle.y + candidateRectangle.height <= freePlace.y )
        return false;

    if (candidateRectangle.x < freePlace.x + freePlace.width
            && candidateRectangle.x + candidateRectangle.width > freePlace.x)
    {
        // New node at the top side of the used node.
        if (candidateRectangle.y > freePlace.y && candidateRectangle.y < freePlace.y + freePlace.height)
        {
            Rectangle newNode = freePlace;
            newNode.height = candidateRectangle.y - newNode.y;
            _freePlaceForRectangles.push_back(newNode);
        }

        // New node at the bottom side of the used node.
        if (candidateRectangle.y + candidateRectangle.height < freePlace.y + freePlace.height)
        {
            Rectangle newNode = freePlace;
            newNode.y = candidateRectangle.y + candidateRectangle.height;
            newNode.height = freePlace.y + freePlace.height - (candidateRectangle.y + candidateRectangle.height);
            _freePlaceForRectangles.push_back(newNode);
        }
    }

    if (candidateRectangle.y < freePlace.y + freePlace.height
            && candidateRectangle.y + candidateRectangle.height > freePlace.y)
    {
        // New node at the left side of the used node.
        if (candidateRectangle.x > freePlace.x && candidateRectangle.x < freePlace.x + freePlace.width)
        {
            Rectangle newNode = freePlace;
            newNode.width = candidateRectangle.x - newNode.x;
            _freePlaceForRectangles.push_back(newNode);
        }

        // New node at the right side of the used node.
        if (candidateRectangle.x + candidateRectangle.width < freePlace.x + freePlace.width)
        {
            Rectangle newNode = freePlace;
            newNode.x = candidateRectangle.x + candidateRectangle.width;
            newNode.width = freePlace.x + freePlace.width - (candidateRectangle.x + candidateRectangle.width);
            _freePlaceForRectangles.push_back(newNode);
        }
    }

    return true;
}

bool Box::isEmpty()
{
    return _containedRectangles.empty();
}

void Box::PruneFreeList()
{
    for(size_t i = 0; i < _freePlaceForRectangles.size(); ++i)
        for(size_t j = i + 1; j < _freePlaceForRectangles.size(); ++j)
            {
                if (_freePlaceForRectangles[j].contains(_freePlaceForRectangles[i]))
                {
                    _freePlaceForRectangles.erase(_freePlaceForRectangles.begin() + i);
                    --i;
                    break;
                }
                if (_freePlaceForRectangles[i].contains(_freePlaceForRectangles[j]))
                {
                    _freePlaceForRectangles.erase(_freePlaceForRectangles.begin() + j);
                    --j;
                }
            }
}


// Neigborhoood
template <typename T>
Neighborhood<T>::Neighborhood(){

}

template <typename T>
Neighborhood<T>::Neighborhood(const Neighborhood &obj)
{
    _neighbors = obj._neighbors;
}

template <typename T>
Neighborhood<T>::~Neighborhood()
{

}

template <typename T>
void Neighborhood<T>::update(T currentSolution, int optionCase)
{
    switch (optionCase) {
    case 1:
        calculateRuleBasedNeighbors(currentSolution);
        break;
    case 0:
        calculateGeometricBasedNeighbors(currentSolution);
        break;
    default:
        calculateGeometricBasedNeighbors(currentSolution);
    }
}

template <typename T>
std::vector<T> Neighborhood<T>::getNeighbors() const
{
    return _neighbors;
}

template <typename T>
void Neighborhood<T>::calculateGeometricBasedNeighbors(T currentSolution){
    /// Try to place rectangle to box that is not filled
    /// Choose the biggest element possible
    /// and place it in optimal way
    ///

    _neighbors.clear();
    // sort boxes by load

    std::sort(currentSolution.begin(), currentSolution.end(), [](const Box &a, const Box &b){
        return a._placedRectanglesArea < b._placedRectanglesArea;
    });

    // choose box with min load
    Box minLoadBox = *(currentSolution.begin());

    // select rectangles to move
    // possibly select all/not the one with max area
    std::vector <Rectangle> minLoadBoxRectangles = minLoadBox.getSortedRectangles();

    auto maxRectangleFromMinLoadBox = minLoadBoxRectangles.end() - 1;
    // Rectangle maxAreaRectangleFromMinLoadBox = minLoadBox.getSortedRectangles();

    // attemp insert rectangle to max load possible
    for (size_t i = currentSolution.size() - 1; i >= 1; i--){
        Box insertMaxLoadBox = currentSolution[i];

        if (insertMaxLoadBox.moveRectangle(*maxRectangleFromMinLoadBox))
        {
            minLoadBox.deleteRectangle(std::distance(maxRectangleFromMinLoadBox, minLoadBoxRectangles.begin()));

            // create neighborhood
            T candidateSolution;
            if (!minLoadBox.isEmpty())
                candidateSolution.push_back(minLoadBox);


            candidateSolution.insert(candidateSolution.end(), currentSolution.begin() + 1, currentSolution.begin() + i);

            if ( i != currentSolution.size() - 1 )
                candidateSolution.insert(candidateSolution.end(), currentSolution.begin() + i + 1, currentSolution.end());

            candidateSolution.push_back(insertMaxLoadBox);

            std::cout << candidateSolution.size() << std::endl;

            _neighbors.push_back(candidateSolution);
            break;
        }
    }
}


template <typename T>
void Neighborhood<T>::calculateRuleBasedNeighbors(T currentSolution){
    size_t i = currentSolution.size();
    i++;
}


template class Neighborhood<std::vector<Box>>;