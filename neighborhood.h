#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>


class Rectangle{
    private:

    public:
        size_t id;
        unsigned int x;
        unsigned int y;
        unsigned int width;
        unsigned int height;
        double area;

        Rectangle();
        Rectangle(size_t id, unsigned int width, unsigned int height);
        Rectangle(const Rectangle& obj);
        ~Rectangle();
        //unsigned int getDimension();
        std::vector<unsigned int> getSideLengths();
        std::vector<unsigned int> getStartPoint();
        void modifyStartPoint(const int position, const unsigned int value);
        void modifySideLength(const int position, const unsigned int value);
        void rotate();
        bool canContain(const Rectangle & rectangle);
        bool contains(const Rectangle & rectangle);
        void moveToPosition(int x, int y);

};

class Box{
public:
    size_t _boxId;
    int _boxWidth;
    int _boxHeigth;
    std::vector<Rectangle> _containedRectangles;
    std::vector<Rectangle> _freePlaceForRectangles;

    double _placedRectanglesArea;
    double _freeArea;
    double _overlappedRectanglesArea;
    size_t _countRectagles;

    Box();
    Box(size_t boxId, int boxSide);
    Box(std::vector<Rectangle> rectangles, size_t boxId, int boxSide);
    Box(const Box& box);
    ~Box();
    //
    bool operator==(const Box& rhs) const { return this->_boxId == rhs._boxId;}
    // place methods
    void deleteRectangle(const size_t position);
    std::vector<Rectangle> getSortedRectangles();
    bool moveRectangle(const Rectangle& rect);
    bool findPositionToPlaceInBox(Rectangle& rectangle);
    bool splitFreePlace(Rectangle freeRectangle, const Rectangle &candidateRectangle);
    bool isEmpty();
    void PruneFreeList();
};


template <typename T>
class Neighborhood
{
private:
    std::vector<T> _neighbors;
    void calculateGeometricBasedNeighbors(T currentSolution);
    void calculateRuleBasedNeighbors(T currentSolution);
public:
    Neighborhood();
    Neighborhood(const Neighborhood& obj);
    ~Neighborhood();

    void update(T currentSolution, int optionCase = 0);
    std::vector<T> getNeighbors() const;
};


#endif // NEIGHBORHOOD_H
