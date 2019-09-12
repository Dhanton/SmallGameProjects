#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>

#include <cmath>
#include <array>
#include <cstdlib>
#include <ctime>
#include <cassert>

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

template<typename T> T randRange(T min, T max);
template<typename T> T randRangeR(T min, T max);           //Real random number
inline bool            randPercentage(float number);       //You have a 'number' chance to return true (between 0.0 and 1.0)
inline void            randomSeed(time_t seed = time(0));
}

#include "mathhelper.inl"

#endif // MATHHELPER_H

