#include "mathhelper.h"

namespace Helper
{
template<typename T>
T abs(T number)
{
    return (number >= 0 ? number : -number);
}

template<typename T>
T sgn(T number)
{
    return (number == 0 ? number : number/abs(number));
}

float degToRad(float deg)
{
    return deg/180.0f * PI;
}

float radToDeg(float rad)
{
    return rad * 180.0f/PI;
}

template<typename T>
std::array<sf::Vector2<T>, 4> rectCorners(const sf::Rect<T> &rect)
{
    std::array<sf::Vector2<T>, 4> array;

    array[0] = {rect.left, rect.top};
    array[1] = {rect.left + rect.width, rect.top};
    array[2] = {rect.left + rect.width, rect.top + rect.height};
    array[3] = {rect.left, rect.top + rect.height};

    return array;
}

template<typename T>
sf::Vector2<T> rectCenter(const sf::Rect<T> &rect)
{
    return {rect.left + rect.width/2, rect.top + rect.height/2};
}

sf::FloatRect viewToRect(const sf::View &view)
{
    sf::FloatRect rect;

    rect.left = view.getCenter().x - view.getSize().x/2;
    rect.top  = view.getCenter().y - view.getSize().y/2;

    rect.width  = view.getSize().x;
    rect.height = view.getSize().y;

    return rect;
}

template<typename T>
T randRange(T min, T max, bool included)
{
    return static_cast<T>(static_cast<int>(randRangeR(min, max, included)));
}

template<typename T>
T randRangeR(T min, T max, bool included)
{
    if (max == min) return max;

    if (max < min) randRange(min, max, included);

    return static_cast<T>(((double) rand() / ((double) RAND_MAX + 1.0)) * (max - min + (included ? 1 : 0)) + min);
}

void randomSeed(void)
{
    srand(time(0));
}
}

