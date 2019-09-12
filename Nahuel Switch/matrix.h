#ifndef MATRIX_H
#define MATRIX_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>

/*
 *
 * Simple class to handle 2D maps and levels (with const size)
 *
 */

template<class T>
class Matrix
{
public:
    Matrix(size_t width, size_t height);
    Matrix(size_t width, size_t height, const T& value);

    /*
     * To iterate the sub_matrix:
     *
     *      rect = matrix.sub_matrix(...);
     *
     *      for (size_t i = rect.left; i < rect.width; ++i)
     *          for (size_t j = rect.top; j < rect.height; ++j)
     *              ...
     */
    sf::Rect<size_t> sub_matrix(sf::FloatRect rect, size_t size = 1) const;

    T& at(size_t i, size_t j);
    const T& at(size_t i, size_t j) const;

    T& at(sf::Vector2<size_t> pos);
    const T& at(sf::Vector2<size_t> pos) const;

    sf::Vector2<size_t> size() const;

private:
    std::vector<std::vector<T>> elems;

    const sf::Vector2<size_t> m_size;
};

#include "matrix.inl"

#endif // MATRIX_H

