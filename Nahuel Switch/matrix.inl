#include "matrix.h"

template<class T>
Matrix<T>::Matrix(size_t width, size_t height):
    m_size(width, height)
{
    elems = {width, std::vector<T>(height)};
}

template<class T>
Matrix<T>::Matrix(size_t width, size_t height, const T &value):
    m_size(width, height)
{
    elems = {width, std::vector<T>(height, value)};
}

template<class T>
sf::Rect<size_t> Matrix<T>::sub_matrix(sf::FloatRect rect, size_t size) const
{
    sf::Rect<size_t> matrix_rect;

    if (rect.left < 0.0f) matrix_rect.left = 0;
    else matrix_rect.left = std::floor(rect.left / size);

    if (rect.top < 0.0f) matrix_rect.top = 0;
    else matrix_rect.top = std::floor(rect.top / size);

    /* Error, rect out of bounds */
    if (matrix_rect.left >= m_size.x || matrix_rect.top >= m_size.y) return {0, 0, 0, 0};

    matrix_rect.width = std::ceil((rect.width + rect.left) / size);
    if (matrix_rect.width > m_size.x) matrix_rect.width = m_size.x;

    matrix_rect.height = std::ceil((rect.height + rect.top)/ size);
    if (matrix_rect.height > m_size.y) matrix_rect.height = m_size.y;

    return matrix_rect;
}

template<class T>
T &Matrix<T>::at(size_t i, size_t j)
{
    return elems.at(i).at(j);
}

template<class T>
const T &Matrix<T>::at(size_t i, size_t j) const
{
    return elems.at(i).at(j);
}

template<class T>
const T &Matrix<T>::at(sf::Vector2<size_t> pos) const
{
    return at(pos.x, pos.y);
}

template<class T>
T &Matrix<T>::at(sf::Vector2<size_t> pos)
{
    return at(pos.x, pos.y);
}

template<class T>
sf::Vector2<size_t> Matrix<T>::size() const
{
    return m_size;
}

