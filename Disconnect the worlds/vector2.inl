#include "vector2.h"

namespace Helper
{
template<typename T>
sf::Vector2<T> angleToVector(float angle)
{
    return {std::cos(angle), std::sin(angle)};
}

template<typename T>
float vectorToAngle(const sf::Vector2<T>& vector)
{
    return std::atan2(vector.x, vector.y); //Rad
}

template<typename T>
float length(const sf::Vector2<T>& vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

template<typename T>
sf::Vector2<T> normalize(const sf::Vector2<T>& vector)
{
    if (length(vector) == 0.0f) return {0, 0};

    return vector / length(vector);
}

template<typename T>
sf::Vector2<T> getNormal(const sf::Vector2<T>& vector)
{
    return {-vector.y, vector.x};
}
}

