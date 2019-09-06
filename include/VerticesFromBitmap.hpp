#ifndef VERTS_FROM_BIT_HPP
#define VERTS_FROM_BIT_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

using Image = sf::Image;
//using Vec2u = sf::Vector2u;
using Color = sf::Color;

struct Vec2
{
    double x;
    double y;

    static float length(Vec2 const & v)
    {
        return sqrtf(v.x*v.x + v.y*v.y);
    }

    static Vec2 norm(Vec2 const & v)
    {
        if(length(v) < 1e-30)
            return {0.0f, 0.0f};

        return v / Vec2::length(v);
    }

    static float dot(Vec2 const & v1, Vec2 const & v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    static float square(Vec2 const & v)
    {
        return dot(v,v);
    }

    static float cross(Vec2 const & v1, Vec2 const & v2)
    {
        return v1.x*v2.y - v1.y*v2.x;
    }

    Vec2 operator - (Vec2 const & v1) const
    {
        return {x - v1.x, y - v1.y};
    }

    Vec2 operator + (Vec2 const & v1) const
    {
        return {x + v1.x, y + v1.y};
    }

    Vec2 operator * (float const & f) const
    {
        return {f*x, f*y};
    }

    Vec2 operator / (float const & f) const
    {
        return {x/f, y/f};
    }

    static float getSignedArea(Vec2 const & v1,
                               Vec2 const & v2)
    {
        return (v2.x - v1.x) * (v2.y + v1.y);
    }
};


struct Vec2u
{
    unsigned int x;
    unsigned int y;

    bool const operator==(const Vec2u &o) const
    {
        return x == o.x && y == o.y;
    }

    bool const operator!=(const Vec2u &o) const
    {
        return !operator==(o);
    }

    bool const operator<(const Vec2u &o) const
    {
        return x < o.x || (x == o.x && y < o.y);
    }

    void print() const
    {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

struct Vec2i
{
    int x;
    int y;

    bool const operator==(const Vec2i &o) const
    {
        return x == o.x && y == o.y;
    }

    bool const operator<(const Vec2i &o) const
    {
        return x < o.x || (x == o.x && y < o.y);
    }
};

struct Island
{
    std::vector<Vec2 > vertexPositions;
};

class VerticesFromBitmap
{
    const Color unoccupiedColor = Color::White;
    const Color occupiedColor = Color::Black;
    const Color scannedColor  = Color::Red;
    const Color outOfBoundsColor = Color::Green;
    const float coLinearThreshold = 0.807106f;
    const float occupancyThreshold = 0.1f;

    Image bitmap;

    std::vector<Island > islands;

    Color getPixel(Vec2u const & coordinate,
                   Image const & _bitmap)
    {
        if(coordinate.x < 0 || coordinate.x >= _bitmap.getSize().x ||
           coordinate.y < 0 || coordinate.y >= _bitmap.getSize().y)
            return outOfBoundsColor;

        return _bitmap.getPixel(coordinate.x, coordinate.y);
    }

    bool isPixelOnABorder(Vec2u const & coordinate,
                          Image const & _bitmap)
    {
        Color neighbour = getPixel({coordinate.x, coordinate.y-1}, _bitmap);

        if(neighbour == unoccupiedColor)
            return true;

        neighbour = getPixel({coordinate.x, coordinate.y+1}, _bitmap);

        if(neighbour == unoccupiedColor)
            return true;

        neighbour =  getPixel({coordinate.x-1, coordinate.y}, _bitmap);

        if(neighbour == unoccupiedColor)
            return true;

        neighbour =  getPixel({coordinate.x+1, coordinate.y}, _bitmap);

        if(neighbour == unoccupiedColor)
            return true;

        return false;
    }

    Vec2u getBestNearestNeighbour(Vec2u const & coordinate,
                                  Image const & _bitmap)
    {
        std::map<Vec2i, Color > neighbours;

        for(int y=-1; y<=1; ++y)
            for(int x=-1; x<=1; ++x)
            {
                Color temp = getPixel({coordinate.x+x, coordinate.y+y}, _bitmap);
                neighbours.insert({Vec2i{x,y}, temp});
            }

        //check diagonal nearest neighbours
        for(int x=-1; x<=1; x+=2)
            for(int y=-1; y<=1; y+=2)
            {
                if(neighbours[{x,y}] == occupiedColor)
                {
                    bool xOccupied = neighbours[{x,0}] == occupiedColor &&
                                     neighbours[{0,y}] == unoccupiedColor;
                    bool yOccupied = neighbours[{x,0}] == unoccupiedColor &&
                                     neighbours[{0,y}] == occupiedColor;
                    if(xOccupied || yOccupied)
                        return {coordinate.x + x, coordinate.y + y};
                }
            }

        //check horizontal neighbours
        for(int x=-1; x<=1; x+=2)
        {
            if(neighbours[{x,0}] == occupiedColor)
            {
                bool xOccupied = neighbours[{x,1}] == occupiedColor &&
                                 neighbours[{x,-1}] == unoccupiedColor;
                bool x2Occupied = neighbours[{x,1}] == unoccupiedColor &&
                                 neighbours[{x,-1}] == occupiedColor;
                if(xOccupied || x2Occupied)
                    return {coordinate.x + x, coordinate.y};
            }
        }

        //check vertical neighbours
        for(int y=-1; y<=1; y+=2)
        {
            if(neighbours[{0,y}] == occupiedColor)
            {
                bool yOccupied = neighbours[{1,y}] == occupiedColor &&
                                 neighbours[{-1,y}] == unoccupiedColor;
                bool y2Occupied = neighbours[{1,y}] == unoccupiedColor &&
                                 neighbours[{-1,y}] == occupiedColor;
                if(yOccupied || y2Occupied)
                    return {coordinate.x, coordinate.y + y};
            }
        }

        return coordinate;
    }

    void generateIslands(Image & _bitmap)
    {
        for(unsigned int y=0; y<_bitmap.getSize().y; ++y)
            for(unsigned int x=0; x<_bitmap.getSize().x; ++x)
            {
                Island tempIsland;
                generateSingleIsland({x,y}, _bitmap, tempIsland);
                if(tempIsland.vertexPositions.size() > 2)
                {
                    islands.push_back(tempIsland);
                }
            }
    }

    void removeCoLinear(Island & _island,
                        float const & coLinearThreshold)
    {
        if(_island.vertexPositions.size() > 2)
        {
            int firstIndex = _island.vertexPositions.size() - 3;
            int secondIndex = _island.vertexPositions.size() - 2;
            int thirdIndex = _island.vertexPositions.size() - 1;

            Vec2 firstEdge = Vec2::norm(_island.vertexPositions[secondIndex] -
                                        _island.vertexPositions[firstIndex]);
            Vec2 secondEdge = Vec2::norm(_island.vertexPositions[thirdIndex] -
                                         _island.vertexPositions[secondIndex]);

            if(Vec2::dot(firstEdge, secondEdge) > coLinearThreshold)
                _island.vertexPositions.erase(_island.vertexPositions.begin() + secondIndex);
        }
    }

    void generateSingleIsland(Vec2u const & startingPixel,
                              Image & _bitmap,
                              Island & _returnIsland)
    {
        Color currPixel = _bitmap.getPixel(startingPixel.x, startingPixel.y);

        if(currPixel == unoccupiedColor ||
           currPixel == scannedColor)
        {
            //std::cout << "pixel scanned or unoccupied\n";
            return;
        }
        if(!isPixelOnABorder(startingPixel, _bitmap))
        {
            _bitmap.setPixel(startingPixel.x, startingPixel.y, scannedColor);
            return;
        }



        _bitmap.setPixel(startingPixel.x, startingPixel.y, scannedColor);

        float floatCoordX = (float)startingPixel.x;
        float floatCoordY = (float)startingPixel.y;

        _returnIsland.vertexPositions.push_back({floatCoordX, floatCoordY});

        removeCoLinear(_returnIsland, coLinearThreshold);

        Vec2u nextPixel = getBestNearestNeighbour(startingPixel, _bitmap);


        if(nextPixel != startingPixel)
            generateSingleIsland(nextPixel, _bitmap, _returnIsland);

    }

    bool aboveThreshold(Color const & color,
                        Color const & occColor,
                        float _occThreshold,
                        char channel = 'g')
    {
        switch(channel)
        {
            case 'r':
            {
                if((float)color.r > ((float)occColor.r * _occThreshold))
                    return true;
                break;
            }
            case 'g':
            {
                if((float)color.g > (float)occColor.g * _occThreshold)
                    return true;
                break;
            }
            case 'b':
            {
                if((float)color.b > (float)occColor.b * _occThreshold)
                    return true;
                break;
            }
            case 'a':
            {
                if((float)color.a > (float)occColor.a * _occThreshold)
                    return true;
                break;
            }
            default:
                break;
        }

        return false;
    }

public:
    VerticesFromBitmap(Image const & _bitmap) : bitmap{_bitmap}
    {
        for(unsigned int y=0; y<bitmap.getSize().y; ++y)
            for(unsigned int x=0; x<bitmap.getSize().x; ++x)
            {
                if(aboveThreshold(bitmap.getPixel(x,y), unoccupiedColor, occupancyThreshold, 'r'))
                    bitmap.setPixel(x,y, unoccupiedColor);
                else
                    bitmap.setPixel(x,y, occupiedColor);
            }
    }

    std::vector<Island > generateIslands()
    {
        generateIslands(bitmap);
        return islands;
    }
};

#endif // VERTS_FROM_BIT_HPP