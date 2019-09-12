#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>

#include <cmath>
#include <array>
#include <cstdlib>
#include <ctime>

namespace Helper
{
constexpr float PI{3.14159265f};

template<typename T> T abs(T number);
template<typename T> T sgn(T number);

inline float degToRad(float deg);
inline float radToDeg(float rad);

template<typename T> std::array<sf::Vector2<T>, 4> rectCorners(const sf::Rect<T>& rect);
template<typename T> sf::Vector2<T>                rectCenter(const sf::Rect<T>& rect);
inline               sf::FloatRect                 viewToRect(const sf::View& view);

template<typename T> T randRange(T min, T max, bool included = false);  //Set included to true if you want the max to be included
template<typename T> T randRangeR(T min, T max, bool included = false); //Real random number
inline void            randomSeed(void);
}

#include "mathhelper.inl"

#endif // MATHHELPER_H

