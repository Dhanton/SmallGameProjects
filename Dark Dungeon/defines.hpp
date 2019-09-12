#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <memory>
#include <cstdint>

template<class T, class D = std::default_delete<T>> using Ptr = std::unique_ptr<T, D>;
template<class T> using Ptr_vector = std::vector<Ptr<T>>;

using Vector2 = sf::Vector2f;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

#define M_PI 3.14159265358979323846

inline double clamp(double x, double upper, double lower)
{
    return std::min(upper, std::max(x, lower));
}

#endif // DEFINES_HPP
