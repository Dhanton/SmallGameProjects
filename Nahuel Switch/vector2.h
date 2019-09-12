#ifndef VECTOR2_H
#define VECTOR2_H

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace Helper
{
template<typename T> sf::Vector2<T> angleToVector(float angle);
template<typename T> float          vectorToAngle(const sf::Vector2<T>& vector);

template<typename T> float          length(const sf::Vector2<T>& vector);
template<typename T> sf::Vector2<T> normalize(const sf::Vector2<T>& vector);
template<typename T> sf::Vector2<T> getNormal(const sf::Vector2<T>& vector);
}

typedef sf::Vector2<float> Vector2;

#include "vector2.inl"

#endif // VECTOR2_H
